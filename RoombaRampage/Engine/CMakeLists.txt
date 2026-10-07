# "Engine lib" static library (was Engine/Engine.vcxproj).

# Pass object lists via response files: the "&" in De&Serialization otherwise
# splits the librarian command line when Ninja runs it through cmd.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE ON)

set(ENGINE_SOURCES
    Actions/ActionManager.cpp
    "Asset Manager/AssetManager.cpp"
    "Asset Manager/Audio.cpp"
    "Asset Manager/Image.cpp"
    "Asset Manager/Prefab.cpp"
    "Asset Manager/SceneManager.cpp"
    "Asset Manager/Text.cpp"
    "Asset Manager/Video.cpp"
    "C#Mono/InternalCall.cpp"
    "C#Mono/mono_handler.cpp"
    Dependencies/backward/backward.cpp
    "De&Serialization/json_handler.cpp"
    Debugging/Logging.cpp
    Debugging/Performance.cpp
    ECS/ECS.cpp
    ECS/Hierachy.cpp
    ECS/Layers.cpp
    ECS/System/AnimationSystem.cpp
    ECS/System/AudioSystem.cpp
    ECS/System/ButtonSystem.cpp
    ECS/System/CameraSystem.cpp
    ECS/System/CollisionResponseSystem.cpp
    ECS/System/CollisionSystem.cpp
    ECS/System/GridSystem.cpp
    ECS/System/LightingSystem.cpp
    ECS/System/LogicSystem.cpp
    ECS/System/ParticleSystem.cpp
    ECS/System/PathfindingSystem.cpp
    ECS/System/PhysicsSystem.cpp
    ECS/System/RayCastSystem.cpp
    ECS/System/RenderDebugSystem.cpp
    ECS/System/RenderSystem.cpp
    ECS/System/RenderTextSystem.cpp
    ECS/System/TilemapSystem.cpp
    ECS/System/TransformSystem.cpp
    ECS/System/UIRenderSystem.cpp
    ECS/System/VideoSystem.cpp
    Events/Event.cpp
    Events/EventsDragFloat.cpp
    Events/EventsEventHandler.cpp
    Events/EventsListeners.cpp
    Events/MessagingMessageSystem.cpp
    Graphics/GraphicsBuffers.cpp
    Graphics/GraphicsCamera.cpp
    Graphics/GraphicsDraw.cpp
    Graphics/GraphicsFrameBufferUsage.cpp
    Graphics/GraphicsMatrix.cpp
    Graphics/GraphicsParticles.cpp
    Graphics/GraphicsPipe.cpp
    Graphics/GraphicsShader.cpp
    Graphics/GraphicsVAO.cpp
    Inputs/Input.cpp
    Math/Mat2x2.cpp
    Math/Mat3x3.cpp
    Math/Mathlib.cpp
    Math/Vector2.cpp
    Math/Vector3.cpp
    Pathfinding/AStarPathfinding.cpp
    Physics/Physics.cpp
    Physics/PhysicsCollisionResponse.cpp
    Physics/PhysicsLayer.cpp
)

add_library(Engine STATIC ${ENGINE_SOURCES})
set_target_properties(Engine PROPERTIES OUTPUT_NAME "Engine lib")
target_link_libraries(Engine PRIVATE kos_options)

# Engine-internal include paths (the engine's own sources were built with these).
target_include_directories(Engine PRIVATE
    ${KOS_DEPS}/rapidjson
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${CMAKE_CURRENT_SOURCE_DIR}/Json Texts"
)

# Include paths, defines and libraries every consumer of the engine needs.
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS
    ${KOS_DEPS}/Freetype_Font/include/freetype/config
    ${KOS_DEPS}/Freetype_Font/include
    ${KOS_DEPS}/mono/include
    ${KOS_DEPS}/mono/include/mono-2.0
    ${KOS_DEPS}/pl_mpeg
    ${KOS_DEPS}/FMOD_API/core/inc
    ${KOS_DEPS}/FMOD_API/studio/inc
    ${KOS_DEPS}/GLFW/include
    ${KOS_DEPS}/glm/include
    ${KOS_DEPS}/stb_image/include
    ${KOS_DEPS}/GLEW/include/GL
)
target_compile_definitions(Engine PRIVATE GLEW_STATIC)
target_link_libraries(Engine PUBLIC
    ${KOS_DEPS}/GLEW/lib/Release/x64/glew32s.lib
    ${KOS_DEPS}/GLFW/lib-vc2022/glfw3.lib
    ${KOS_DEPS}/Freetype_Font/lib/freetype.lib
    ${KOS_DEPS}/mono/lib/mono-2.0-sgen.lib
    ${KOS_DEPS}/FMOD_API/core/lib/x64/fmod_vc.lib
    opengl32 User32 Gdi32 Shell32 Comdlg32
)

# Precompiled header (Config/pch.h). These four were "NotUsing" in the vcxproj.
target_precompile_headers(Engine PRIVATE Config/pch.h)
set_source_files_properties(
    "Asset Manager/AssetManager.cpp"
    "Asset Manager/Audio.cpp"
    "C#Mono/InternalCall.cpp"
    "C#Mono/mono_handler.cpp"
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

# Runtime DLLs the executables need next to them.
set(KOS_RUNTIME_DLLS
    ${KOS_DEPS}/FMOD_API/core/lib/x64/fmod.dll
    ${KOS_DEPS}/FMOD_API/core/lib/x64/fmodL.dll
    ${KOS_DEPS}/Freetype_Font/lib/freetype.dll
    ${KOS_DEPS}/mono/mono-2.0-sgen.dll
    PARENT_SCOPE
)

# GameScript.dll (the C# ScriptCore the engine loads through mono). Visual Studio
# built this from GameScript.csproj; here it's compiled with the bundled mono csc.
set(GAMESCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ScriptLibrary/GameScript)
file(GLOB GAMESCRIPT_SOURCES CONFIGURE_DEPENDS ${GAMESCRIPT_DIR}/ScriptCore/*.cs)
set(GAMESCRIPT_DLL ${GAMESCRIPT_DIR}/ScriptCoreDLL/GameScript.dll)
add_custom_command(
    OUTPUT ${GAMESCRIPT_DLL}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GAMESCRIPT_DIR}/ScriptCoreDLL
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/C#Mono/CompilerCSC/bin/csc.bat"
            -nologo -target:library -out:${GAMESCRIPT_DLL} ${GAMESCRIPT_SOURCES}
    DEPENDS ${GAMESCRIPT_SOURCES}
    COMMENT "Compiling GameScript.dll"
    VERBATIM
)
add_custom_target(GameScript ALL DEPENDS ${GAMESCRIPT_DLL})
add_dependencies(Engine GameScript)
