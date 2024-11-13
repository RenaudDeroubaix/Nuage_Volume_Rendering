#version 330 core

// --------------------------------------------------
// Shader definition
// --------------------------------------------------

uniform mat4 mv_matrix; // Model-view matrix
uniform mat4 proj_matrix; // Projection matrix

// --------------------------------------------------
// In/Out variables
// --------------------------------------------------
in vec3 position;


out vec3 fragPosition;  // Output vertex position to fragment shader
out vec3 fragTexCoord;  // Output texture coordinates to fragment shader
out mat4 view_mat;  // View matrix to pass to fragment shader
out mat4 prof_mat;

// --------------------------------------------------
// Vertex-Shader
// --------------------------------------------------

uniform float xMax;  // Max value for the X axis
uniform float yMax;  // Max value for the Y axis
uniform float zMax;  // Max value for the Z axis

void main()
{
    // Pass the model-view matrix to the fragment shader
    view_mat = mv_matrix;
    prof_mat = proj_matrix;

    // Calculate the final position for the vertex in clip space
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

    // Pass the position and texture coordinates to the fragment shader
    fragPosition = position;

    // Normalize the texture coordinates based on the maximum values
    fragTexCoord.x = position.x / xMax;
    fragTexCoord.y = position.y / yMax;
    fragTexCoord.z = position.z / zMax;
}
