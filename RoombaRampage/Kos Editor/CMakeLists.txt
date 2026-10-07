# "Kos Engine" editor executable (was Kos Editor/Kos Engine.vcxproj).

add_executable(KosEditor
    Application/Application.cpp
    Application/main.cpp
    Application/Window.cpp
    Dependencies/imgui/imgui.cpp
    Dependencies/imgui/ImGuizmo.cpp
    Dependencies/imgui/imgui_draw.cpp
    Dependencies/imgui/imgui_impl_glfw.cpp
    Dependencies/imgui/imgui_impl_opengl3.cpp
    Dependencies/imgui/imgui_stdlib.cpp
    Dependencies/imgui/imgui_tables.cpp
    Dependencies/imgui/imgui_widgets.cpp
    Dependencies/imgui/implot.cpp
    Dependencies/imgui/implot_items.cpp
    Editor/EditorCamera.cpp
    "Editor/ImGui Panels/Editor.cpp"
    "Editor/ImGui Panels/imgui_component_window.cpp"
    "Editor/ImGui Panels/imgui_contentBrowser.cpp"
    "Editor/ImGui Panels/imgui_editor_window.cpp"
    "Editor/ImGui Panels/imgui_game_scene_window.cpp"
    "Editor/ImGui Panels/imgui_Gizmo.cpp"
    "Editor/ImGui Panels/imgui_global_settings_panel.cpp"
    "Editor/ImGui Panels/imgui_hierarchy_window.cpp"
    "Editor/ImGui Panels/imgui_layer_panel.cpp"
    "Editor/ImGui Panels/imgui_logs_window.cpp"
    "Editor/ImGui Panels/imgui_menubar.cpp"
    "Editor/ImGui Panels/imgui_performance_window.cpp"
    "Editor/ImGui Panels/imgui_play_pause.cpp"
    "Editor/ImGui Panels/imgui_test_panel.cpp"
    "Editor/ImGui Panels/imgui_tile_picker.cpp"
    "Editor/ImGui Panels/imgui__input_window.cpp"
    "Editor/ImGui Panels/ScriptVariable.cpp"
    Editor/TilemapCalculations.cpp
    Editor/WindowFile.cpp
)
set_target_properties(KosEditor PROPERTIES OUTPUT_NAME "Kos Engine")
target_include_directories(KosEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui
)
target_link_libraries(KosEditor PRIVATE kos_options Engine)

add_custom_command(TARGET KosEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${KOS_RUNTIME_DLLS} $<TARGET_FILE_DIR:KosEditor>
)
