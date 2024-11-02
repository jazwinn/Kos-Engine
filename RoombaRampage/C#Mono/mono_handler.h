#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>


#include "../ECS/ECS.h"
#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace script {

	// 1 struct, 1 dll
	struct ScriptMonoInfo {
		std::filesystem::path m_scriptPath;
		std::string m_fileName;
		MonoImage* m_image;
		MonoAssembly* m_assembly;
		std::unordered_map<std::string, MonoMethod*> m_Methods;
	};

	class ScriptHandler {
	public:
		// Initialize Mono and load C#
		ScriptHandler();

		void m_LoadSecondaryDomain();

		void m_UnloadSecondaryDomain();

		~ScriptHandler();

		void m_UnloadDomain(const std::filesystem::path& filePath);


		void m_CompileCSharpFile(const std::filesystem::path& filePath);

		//Add multiple scripts and load their individual assembly
		void m_AddScripts(const std::filesystem::path& scriptpath);

		// Find method in the C# Script
		bool m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, int paramCount);


		MonoObject* m_CreateObjectInstance(const std::string& scriptName, const std::string& className);
		// Invoke method
		void m_InvokeMethod(const std::string& scriptName, const std::string& methodName, MonoObject* objInstance, void** args, int paramCount);

		void m_Cleanup();



		void m_HotReloadCompileAllCsharpFile();

		void m_ReloadAllDLL();

		/*************************************/
		std::vector<std::string> m_CSScripts; // store all scripts stored in Assets/Scripts/ScriptsCS

		// key is filename, second is script mono data
		std::unordered_map<std::string, ScriptMonoInfo> m_ScriptMap;


	private:
		//root domain
		MonoDomain* m_rootDomain = nullptr;

		MonoDomain* m_AppDomain = nullptr;


		
	};

	/*--------------------------------------------------------------
	TUTORIAL
	----------------------------------------------------------------

		1.  * C# Script *
			- Create your C# file (ExampleScript.cs) with the methods you want to call in C++

		2.  * Compile the C# into DLL *
			- View > Terminal (Open VS terminal)
			- cd "G:\Documents\RoombaOS_ForkCloneRemote\RoombaRampage\C#Mono" (Example to point to your C# file)
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