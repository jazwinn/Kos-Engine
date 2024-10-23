#include "mono_handler.h"
#include <iostream>

namespace Script {

    ScriptHandler::ScriptHandler() {}

    ScriptHandler::~ScriptHandler() {
        m_Cleanup();
    }

    bool ScriptHandler::m_InitMono(const std::string& assemblyPath) {
        // Set Mono path
        mono_set_dirs("Dependencies/mono/lib", "Dependencies/mono/etc");

        // Initialize Mono runtime
        m_monoDomain = mono_jit_init("MonoDomain");
        if (!m_monoDomain) {
            std::cout << "Mono domain not initialized." << std::endl;
            return false;
        }

        return true;
    }

    void ScriptHandler::m_AddScripts(const std::vector<std::string>& scriptNames) {
        for (const auto& scriptName : scriptNames) {
            m_scriptNames.push_back(scriptName);

            // Load assembly
            std::string assemblyPath = "C# Mono/" + scriptName + ".dll";
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
            m_assemblies[scriptName] = assembly;
            m_images[scriptName] = image;

            std::cout << "Successfully added script: " << scriptName << std::endl;
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
        m_testClass = mono_class_from_name(image, "", className.c_str());
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

        // Invoke the method with arguments
        MonoObject* exception = nullptr;
        mono_runtime_invoke(method, nullptr, args, &exception);

        if (exception) {
            MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
            const char* messageStr = mono_string_to_utf8(exceptionMessage);
            std::cerr << "Exception in C# method invocation: " << messageStr << std::endl;
            mono_free((void*)messageStr);
        }
        else {
            std::cout << "Method invoked successfully for script: " << scriptName << std::endl;
        }
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
