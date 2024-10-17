#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <string>

namespace mono {

	class MonoScriptHandler {
	public:
		MonoScriptHandler();
		~MonoScriptHandler();

		// Initialize Mono and load C#
		bool m_InitMono(const std::string& assemblyPath);

		// Find method in the C# Script
		bool m_LoadMethod(const std::string& className, const std::string& methodName, int paramCount);

		// Invoke method
		void m_InvokeMethod(const std::string& className, const std::string& methodName, void** args, int paramCount);


		void m_Cleanup();

		// getter for m_monoDomain
		MonoDomain* m_GetMonoDomain() const;

	private:
		MonoDomain* m_monoDomain = nullptr;
		MonoAssembly* m_assembly = nullptr;
		MonoImage* m_image = nullptr;
		MonoClass* m_testClass = nullptr;

		MonoMethod* m_method = nullptr;
	};

	/*--------------------------------------------------------------
	TUTORIAL
	----------------------------------------------------------------

		1.  * C# Script *
			- Create your C# file (ExampleScript.cs) with the methods you want to call in C++

		2.  * Compile the C# into DLL *
			- View > Terminal (Open VS terminal)
			- cd "G:\Documents\RoombaOS_ForkCloneRemote\RoombaRampage\C# Mono" (Example to point to your C# file)
			- csc /target:library ExampleScript.cs (This will compile your C# script into DLL)

		3.  * Load and Invoke C# Method *
			- use 'm_LoadMethod()' to load the method
			- use 'm_InvokeMethod()' to invoke the method
			- Example to load and invoke the `HelloWorld` method:
				```
				monoManager.m_LoadMethod("ExampleScript", "HelloWorld", 0);
				monoManager.m_InvokeMethod("ExampleScript", "HelloWorld", nullptr, 0);
				```
		4. * Always remember to clean up Mono Runtime*
			-use 'm_Cleanup()'

	--------------------------------------------------------------*/

}