#version 430 core

uniform mat4 mv_matrix;   // Model-view matrix
uniform mat4 proj_matrix; // Projection matrix

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragNormal;      // Normal for fragment shader
out vec2 fragTexCoord;    // UV for fragment shader

void main() {
    fragNormal = mat3(mv_matrix) * normal;  // Transform normal to view space
    fragTexCoord = texCoord;               // Pass UV to fragment shader
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}
