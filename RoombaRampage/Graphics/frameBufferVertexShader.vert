/** 
 * @file frameBufferShader.vert
 * @version 460 core
 * 
 * @brief Vertex shader for rendering a framebuffer quad.
 *
 * This shader is designed to render a quad that will be used to display the contents
 * of a framebuffer. It accepts vertex positions and texture coordinates as inputs.
 * The positions define the corners of the quad, and the texture coordinates map
 * the framebuffer's texture to the quad.
 *
 * @input
 * - `in vec4 vertex (location = 0)`: 
 *   - xy: Position of the vertex in normalized device coordinates (NDC).
 *   - zw: Corresponding texture coordinates for mapping the framebuffer texture.
 * 
 * @output
 * - `out vec2 texCoords`: The texture coordinates passed to the fragment shader.
 *
 * @details
 * The shader extracts the vertex's xy components for the position and zw components
 * for the texture coordinates. It outputs the texture coordinates to the fragment shader
 * and sets the vertex position in NDC space.
 */

R"( #version 460 core

layout (location = 0) in vec4 vertex;

out vec2 texCoords;

void main()
{
    texCoords = vertex.zw;
    gl_Position = vec4(vertex.xy, 0.0, 1.0); 
}  


)"