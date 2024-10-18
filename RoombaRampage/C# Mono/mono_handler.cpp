#include "mono_handler.h"
#include <iostream>

namespace mono {

    MonoScriptHandler::MonoScriptHandler() {}

    MonoScriptHandler::~MonoScriptHandler() {
        m_Cleanup();
    }

    bool MonoScriptHandler::m_InitMono(const std::string& assemblyPath) {
        // Set Mono path
        mono_set_dirs("Dependencies/mono/lib", "Dependencies/mono/etc");

        // Initialize Mono runtime
        m_monoDomain = mono_jit_init("MonoDomain");
        if (!m_monoDomain) {
            std::cout << "Mono domain not initialized." << std::endl;
            return false;
        }

        // Load the assembly
        m_assembly = mono_domain_assembly_open(m_monoDomain, assemblyPath.c_str());
        if (!m_assembly) {
            std::cout << "Mono Assenb not initialized." << std::endl;
            return false;
        }

        // Get the Mono image (Obj that contain all the info of the assembly)
        m_image = mono_assembly_get_image(m_assembly);
        if (!m_image) {
            std::cout << "Failed to load Mono image from the assembly." << std::endl;
            return false;
        }

        //Load the method here
        if (!m_LoadMethod("ExampleScript", "HelloWorld", 0)) {
            std::cout << "Failed to load HelloWorld method." << std::endl;
            return false;
        }

        if (!m_LoadMethod("ExampleScript", "PrintMessage", 2)) {
            std::cout << "Failed to load PrintMessage method." << std::endl;
            return false;
        }
        return true;
    }

    bool MonoScriptHandler::m_LoadMethod(const std::string& className, const std::string& methodName, int paramCount) {

        // Find the class inside the assembly
        m_testClass = mono_class_from_name(m_image, "", className.c_str());
        if (!m_testClass) {
            return false;
        }

        // Find the method inside the class
        m_method = mono_class_get_method_from_name(m_testClass, methodName.c_str(), paramCount);
        if (!m_method) {
            return false;
        }
        return true;
    }

    void MonoScriptHandler::m_InvokeMethod(const std::string& className, const std::string& methodName, void** args, int paramCount) {

        MonoClass* monoClass = mono_class_from_name(m_image, "", className.c_str());
        MonoMethod* method = mono_class_get_method_from_name(monoClass, methodName.c_str(), paramCount);

        if (!m_method) {
            std::cout << "No method loaded to invoke." << std::endl;
            return;
        }

        // Invoke the method with arguments
        MonoObject* exception = nullptr;
        mono_runtime_invoke(method, nullptr, args, &exception);

        if (exception) {
            MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
            const char* messageStr = mono_string_to_utf8(exceptionMessage);
            mono_free((void*)messageStr);
        }
        else {
            std::cout << "Method invoked successfully." << std::endl;
        }
    }



    void MonoScriptHandler::m_Cleanup() {
        if (m_monoDomain) {
            mono_jit_cleanup(m_monoDomain);
        }
    }

    MonoDomain* MonoScriptHandler::m_GetMonoDomain() const {
        return m_monoDomain;
    }
}
