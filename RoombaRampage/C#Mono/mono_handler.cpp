/********************************************************************/
/*!
\file      mono_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 11, 2024
\brief     This header file implements functions and structures for handling Mono runtime operations
           for scripting integration in the ECS framework.
           - ScriptMonoInfo: Struct to store script metadata, including the script path, filename, image, and assembly.
           - m_LoadSecondaryDomain: Loads an additional domain for isolated script compilation.
           - m_UnloadSecondaryDomain: Unloads the secondary domain.
           - m_UnloadDomain: Unloads a specified domain by file path.
           - m_CompileCSharpFile: Compiles a C# script into a DLL file.
           - m_AddScripts: Adds and loads assemblies for multiple scripts.
           - m_LoadMethod: Loads a specified method from a C# script, preparing it for invocation.
           - m_CreateObjectInstance: Creates a Mono object instance from a specified script and class.
           - m_InvokeMethod: Invokes a previously loaded method within a specified C# script.
           - m_Cleanup: Cleans up and unloads the Mono runtime, releasing resources.
           - m_HotReloadCompileAllCsharpFile: Recompiles all C# files for hot reloading.
           - m_ReloadAllDLL: Reloads all compiled DLLs for updated scripting.

This file enables C++ and C# interoperation by using Mono�s runtime for handling and executing C# scripts
within the game�s ECS framework. It supports functions for compiling scripts, creating object instances,
hot reloading scripts, and invoking script methods, which allows dynamic behavior customization.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "mono_handler.h"
#include "mono/metadata/mono-gc.h"
#include "mono/metadata/threads.h"
#include "InternalCall.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <thread>

namespace script {

    ScriptHandler::ScriptHandler() {
        // Set Mono path
        mono_set_dirs("Dependencies/mono/lib", "Dependencies/mono/etc");

        // need for internal call during run time
        m_rootDomain = mono_jit_init("MonoDomain");

        mono_thread_set_main(mono_thread_current());

        // initialize internal call
        InternalCall::m_RegisterInternalCalls();

        m_LoadSecondaryDomain();
    }

    void ScriptHandler::m_LoadSecondaryDomain()
    {
        m_AppDomain = mono_domain_create_appdomain((char*)"AppDomainRuntime", nullptr);
        mono_domain_set(m_AppDomain, true);

        //load gamescript assembly
        m_AddScripts("ScriptLibrary/GameScript/ScriptCoreDLL/GameScript.dll");
    }

    void ScriptHandler::m_UnloadSecondaryDomain()
    {
        //reset any references to the MonoAssembly
        mono_domain_set(m_rootDomain, false);
        mono_domain_unload(m_AppDomain);
        m_AppDomain = nullptr;

        //clear entire script map
        m_ScriptMap.clear();
    }

    ScriptHandler::~ScriptHandler() {

        if (m_rootDomain) {
            mono_jit_cleanup(m_rootDomain);
        }
    }

    void ScriptHandler::m_UnloadDomain(const std::filesystem::path& filePath) {
        std::string filename = filePath.filename().stem().string();
        auto scriptEntry = m_ScriptMap.find(filename);
        if (scriptEntry == m_ScriptMap.end()) {
            LOGGING_ERROR("Script does not exist");
            return;
        }
        mono_gc_collect(2);

        // Free methods and image
        auto& scriptData = scriptEntry->second;
        scriptData.m_Methods.clear();
        if (scriptData.m_image) {
            mono_image_close(scriptData.m_image);
            scriptData.m_image = nullptr;
        }

        // Remove from map
        m_ScriptMap.erase(scriptEntry);
    }

    void ScriptHandler::m_AddScripts(const std::filesystem::path& scriptpath) {   
        //check if dll is alread in 
        std::string filename = scriptpath.filename().stem().string();
        if (m_ScriptMap.find(filename) != m_ScriptMap.end()) {
            LOGGING_ERROR("Script Already loaded");
            return;
        }
        //create new script
        m_ScriptMap[filename].m_fileName = filename;
        m_ScriptMap[filename].m_scriptPath = scriptpath;

        // Load assembly
        std::string assemblyPath = scriptpath.string();
        m_ScriptMap[filename].m_assembly = mono_domain_assembly_open(m_AppDomain, assemblyPath.c_str());
        if (!m_ScriptMap[filename].m_assembly) {
            std::cout << "Failed to load assembly: " << assemblyPath << std::endl;
            return;
        }

        // Get Mono image
        MonoImage* image = mono_assembly_get_image(m_ScriptMap[filename].m_assembly);
        if (!image) {
            std::cout << "Failed to load Mono image from assembly: " << assemblyPath << std::endl;
            return;
        }

        m_ScriptMap[filename].m_image = image;
        image = nullptr;

        LOGGING_DEBUG("Successfully Added Script");
    }

    bool ScriptHandler::m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, int paramCount) {
        // Find the assembly image for the script
        MonoImage* image = m_ScriptMap.find(scriptName)->second.m_image;
        if (!image) {
            std::cout << "Failed to find image for script: " << scriptName << std::endl;
            return false;
        }

        // Find the class inside the assembly
        MonoClass* m_testClass = mono_class_from_name(image, "Namespace", className.c_str());
        if (!m_testClass) {// give exception for gamescript
            std::cout << "Failed to find class: " << className << " in script: " << scriptName << std::endl;
            return false;
        }

        // Find the method inside the class
        MonoMethod* method = mono_class_get_method_from_name(m_testClass, methodName.c_str(), paramCount);
        if (!method) {
            std::cout << "Failed to find method: " << methodName << " in class: " << className << std::endl;
            return false;
        }

        if (m_ScriptMap.find(scriptName)->second.m_Methods.find(methodName) != m_ScriptMap.find(scriptName)->second.m_Methods.end()) {
            LOGGING_ERROR("METHOD ALREADY LOADED");
        }
        //create new method
        m_ScriptMap.find(scriptName)->second.m_Methods[methodName] = method;
        method = nullptr;

        return true;
    }

    MonoObject* ScriptHandler::m_CreateObjectInstance(const std::string& scriptName, const std::string& className)
    {
        MonoObject* inst = mono_object_new(m_AppDomain, mono_class_from_name(m_ScriptMap.find(scriptName)->second.m_image, "Namespace", className.c_str()));

        if (inst == nullptr) {
            LOGGING_ERROR("Fail to create instance");
            return inst;
        }

        //run the construct for the object
        mono_runtime_object_init(inst);

        return inst;
    }

    void ScriptHandler::m_InvokeMethod(const std::string& scriptName, const std::string& methodName, MonoObject* objInstance, void** args) {

        // Check if the method exists
        MonoMethod* method = m_ScriptMap.find(scriptName)->second.m_Methods.find(methodName)->second;
        if (!method) {
            std::cerr << "No method loaded to invoke for script: " << scriptName << std::endl;
            return;
        }

        // Invoke the method with the instance
        MonoObject* exception = nullptr;
        
        mono_runtime_invoke(method, objInstance, args, &exception);

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

    void ScriptHandler::m_ReloadAllDLL()
    {
        //reload all .dll file
        for (auto& directoryPath : std::filesystem::directory_iterator("Assets/Scripts/ScriptsDLL")) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            if (directoryPath.path().filename().extension().string() == ".dll") {
                m_AddScripts(directoryPath);

                //load all newly reloaded method

                std::string filename = directoryPath.path().filename().stem().string();
                m_LoadMethod(filename, filename, "Start", 0);
                m_LoadMethod(filename, filename, "Update", 0);
                m_LoadMethod(filename, filename, "GetEntityID", 1);
            }
        }
    }

    void ScriptHandler::m_CompileCSharpFile(const std::filesystem::path& filePath)
    {
        //IF COMPILE ERROR, MAKE SURE TO UNLOAD ASSEMBLY AND APPDOMAIN
        std::filesystem::path projectBasePath = std::filesystem::current_path();

        std::string compilepath = "\"" + projectBasePath.string() + "\\C#Mono\\CompilerCSC\\bin\\csc\"";

        std::filesystem::path referenceDLL = projectBasePath / "ScriptLibrary" / "GameScript" / "ScriptCoreDLL" / "GameScript.dll";

        std::string test = filePath.filename().stem().string();

        std::filesystem::path outputDLL = projectBasePath / "Assets" / "Scripts" / "ScriptsDLL" / (filePath.filename().stem().string() + ".dll");

        std::string command = compilepath + " /target:library \"/out:" + outputDLL.string() + "\" \"/reference:" + referenceDLL.string() + "\" \"" + filePath.string() + "\"";
        //std::string command = compilepath + " /target:library /out:" + outputDLL.string() + " C:\\Users\\ngjaz\\OneDrive\\Documents\\roombarampage\\GreyGooseWorkspace\\RRR\\RoombaRampage\\ScriptLibrary\\GameScript\\ScriptCore\\ScriptBase.cs "  + filePath.string();
        
        // Write the command to a batch file
        std::ofstream batchFile("run_command.bat");
        batchFile << command << std::endl;
        batchFile.close();

        std::cout << command << std::endl;
        //// Execute the command
        int result = system("run_command.bat");

        //remove bat file
        std::remove("run_command.bat");

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

    void ScriptHandler::m_HotReloadCompileAllCsharpFile()
    {
        // load all .cs file in /Assests/Script
        for (auto& directoryPath : std::filesystem::directory_iterator("Assets/Scripts/ScriptsCS")) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            m_UnloadSecondaryDomain();

            if (directoryPath.path().filename().extension().string() == ".cs") {

                m_CompileCSharpFile(filepath);
            }

            // did u call unload secodnary domain before this?
            if (m_AppDomain == nullptr) {
                m_LoadSecondaryDomain();
            }
        }
    }
}
