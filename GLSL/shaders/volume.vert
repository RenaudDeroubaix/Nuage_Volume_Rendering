#version 430 core

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

out mat4 view_mat;  // View matrix to pass to fragment shader
out mat4 prof_mat;

// --------------------------------------------------
// Vertex-Shader
// --------------------------------------------------

uniform vec3 BBmin;
uniform vec3 BBmax;


void main()
{
    // Pass the model-view matrix to the fragment shader




    view_mat = mv_matrix;
    prof_mat = proj_matrix;




    // Calculate the final position for the vertex in clip space
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

    // Pass the position and texture coordinates to the fragment shader
    fragPosition = position;


}
