#version 330 core

in vec3 position;

uniform mat4 mv_matrix; // Model-view matrix
uniform mat4 proj_matrix; // Projection matrix

void main() {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0); // Transformation totale
}
