cmake_minimum_required(VERSION 3.25)

# Mirrors Kos.sln: "Engine lib" (static library), "Kos Engine" (editor) and "RR" (game).
project(Kos LANGUAGES C CXX)

if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Kos only builds with MSVC for x64 (the bundled GLFW/GLEW/FMOD/mono libs are MSVC x64 binaries).")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Embed debug info in the .obj files (/Z7) instead of a shared compiler PDB:
# parallel builds inside a OneDrive folder otherwise fail with C1090.
cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")

# Binaries land in build/bin/<Config>, like x64/<Config> did under Visual Studio.
foreach(_dir RUNTIME LIBRARY ARCHIVE)
    set(CMAKE_${_dir}_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endforeach()

set(KOS_DEPS ${CMAKE_SOURCE_DIR}/Engine/Dependencies)

# Settings shared by every C++ project in the solution.
add_library(kos_options INTERFACE)
target_compile_definitions(kos_options INTERFACE
    UNICODE _UNICODE
    _CRT_SECURE_NO_WARNINGS
    _CONSOLE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)
target_compile_options(kos_options INTERFACE
    /W4 /sdl /permissive- /Zc:preprocessor
    /wd4005 /wd4245 /wd4083 /wd4244 /wd4706 /wd4267 /wd4305
)
target_link_options(kos_options INTERFACE /IGNORE:4098,4099,4286)

add_subdirectory(Engine)
add_subdirectory("Kos Editor")
add_subdirectory(RoombaRampage)

