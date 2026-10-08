# "RR" game executable (was RoombaRampage/RR.vcxproj).

add_executable(RR WIN32
    Application/Application.cpp
    Application/main.cpp
    Application/Window.cpp
)
set_target_properties(RR PROPERTIES OUTPUT_NAME "R00M13A-OS")
target_include_directories(RR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(RR PRIVATE GLEW_STATIC)
target_link_libraries(RR PRIVATE kos_options Engine)

add_custom_command(TARGET RR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${KOS_RUNTIME_DLLS} $<TARGET_FILE_DIR:RR>
)
