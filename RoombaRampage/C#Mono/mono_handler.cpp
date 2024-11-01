#include "mono_handler.h"
#include "InternalCall.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace script {

    ScriptHandler::ScriptHandler() {
        // Set Mono path
        mono_set_dirs("Dependencies/mono/lib", "Dependencies/mono/etc");


        // Initialize Mono runtime
        m_monoDomain = mono_jit_init("MonoDomain");
        if (!m_monoDomain) {
            std::cout << "Mono domain not initialized." << std::endl;
            return;
        }

        // initialize internal call
        InternalCall::m_RegisterInternalCalls();

    }

    ScriptHandler::~ScriptHandler() {
        m_Cleanup();
    }

    void ScriptHandler::m_ReloadAssembly(const std::string& assemblypath)
    {

        // Release previous assembly references (if any)
        if (m_images.find(assemblypath) != m_images.end()) {
            m_images.erase(assemblypath);  // Remove from map to prevent re-use of the old reference
        }

        // Load the new assembly
        MonoAssembly* assembly = mono_domain_assembly_open(mono_domain_get(), assemblypath.c_str());
        if (!assembly) {
            std::cerr << "Failed to load assembly: " << assemblypath << std::endl;
            return;
        }

        // Store the new assembly
        m_images[assemblypath] = mono_assembly_get_image(assembly);

        // Re-fetch class and method references as needed
        m_testClass = mono_class_from_name(m_images[assemblypath], "Namespace", "ExampleScriptB");
        if (!m_testClass) {
            std::cerr << "Failed to find class ExampleScriptB after reload" << std::endl;
        }




    }

    void ScriptHandler::m_CompileAllCsharpFile()
    {
        // load all .cs file in /Assests/Script
        for (auto& directoryPath : std::filesystem::directory_iterator("Assets/Scripts/ScriptsCS")) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            if (directoryPath.path().filename().extension().string() == ".cs") {
                m_CompileCSharpFile(filepath);
            }

        }


    }

    void ScriptHandler::m_CompileCSharpFile(const std::filesystem::path& filePath)
    {
        std::filesystem::path projectBasePath = std::filesystem::current_path();

        std::string compilepath = projectBasePath.string() + "\\C#Mono\\CompilerCSC\\bin\\csc";

        std::filesystem::path referenceDLL = projectBasePath / "ScriptLibrary" / "GameScript" / "ScriptCoreDLL" / "GameScript.dll";

        std::string test = filePath.filename().stem().string();

        std::filesystem::path outputDLL = projectBasePath / "Assets" / "Scripts" / "ScriptsDLL" / (filePath.filename().stem().string() + ".dll");


        std::string command = compilepath + " /target:library /out:" + outputDLL.string() + " /reference:" + referenceDLL.string() + " " + filePath.string();
        std::cout << command << std::endl;
        //// Execute the command
        int result = system(command.c_str());

        // Check the result of the command
        if (result == 0)
        {
            std::cout << "Compilation successful: " + filePath.filename().stem().string() + ".dll" << std::endl;
        }
        else
        {
            std::cerr << "Compilation failed!" << std::endl;
        }
    }




    void ScriptHandler::m_AddScripts(const std::filesystem::path& scriptpath) {

            
            // Load assembly
            std::string assemblyPath = scriptpath.string();
            MonoAssembly* assembly = mono_domain_assembly_open(m_monoDomain, assemblyPath.c_str());
            if (!assembly) {
                std::cout << "Failed to load assembly: " << assemblyPath << std::endl;
                return;
            }

            // Get Mono image
            MonoImage* image = mono_assembly_get_image(assembly);
            if (!image) {
                std::cout << "Failed to load Mono image from assembly: " << assemblyPath << std::endl;
                return;
            }

            // Store the assembly and image
            std::string scriptname = scriptpath.filename().stem().string();

            if (m_images.find(scriptname) != m_images.end()) {
                m_images.find(scriptname)->second = image;
                LOGGING_DEBUG("Successfully Reload Script");
            }
            else {
                m_images[scriptname] = image;
                m_scriptNames.push_back(scriptname);
                LOGGING_DEBUG("Successfully Added Script");
            }

    }

    bool ScriptHandler::m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, int paramCount) {
        // Find the assembly image for the script
        MonoImage* image = m_images[scriptName];
        if (!image) {
            std::cout << "Failed to find image for script: " << scriptName << std::endl;
            return false;
        }

        // Find the class inside the assembly
        m_testClass = mono_class_from_name(image, "Namespace", className.c_str());
        if (!m_testClass) {
            std::cout << "Failed to find class: " << className << " in script: " << scriptName << std::endl;
            return false;
        }

        // Find the method inside the class
        MonoMethod* method = mono_class_get_method_from_name(m_testClass, methodName.c_str(), paramCount);
        if (!method) {
            std::cout << "Failed to find method: " << methodName << " in class: " << className << std::endl;
            return false;
        }

        m_methods[scriptName] = method;
        return true;
    }

    void ScriptHandler::m_InvokeMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, void** args, int paramCount) {

        // Check if the method exists
        MonoMethod* method = m_methods[scriptName];
        if (!method) {
            std::cerr << "No method loaded to invoke for script: " << scriptName << std::endl;
            return;
        }

        // Invoke the method with the instance
        MonoObject* exception = nullptr;
        MonoObject* instance = mono_object_new(m_GetMonoDomain(), mono_class_from_name(m_images[scriptName], "Namespace", className.c_str()));
        mono_runtime_object_init(instance);
        mono_runtime_invoke(method, instance, args, &exception);

        if (exception) {
            MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
            const char* messageStr = mono_string_to_utf8(exceptionMessage);
            std::cerr << "Exception in C# method invocation: " << messageStr << std::endl;
            mono_free((void*)messageStr);
        }
        //else {
        //    std::cout << "Method invoked successfully for script: " << scriptName << std::endl;
        //}
    }



    void ScriptHandler::m_Cleanup() {
        if (m_monoDomain) {
            mono_jit_cleanup(m_monoDomain);
        }
    }

    MonoDomain* ScriptHandler::m_GetMonoDomain() const {
        return m_monoDomain;
    }
}
