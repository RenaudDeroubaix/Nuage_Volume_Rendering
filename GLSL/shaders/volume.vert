// --------------------------------------------------
// shader definition
// --------------------------------------------------

#version 130


uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

// --------------------------------------------------
// varying variables
// --------------------------------------------------
varying vec3 position;
varying vec3 textCoord;
varying vec3 view_mat;
// --------------------------------------------------
// Vertex-Shader
// --------------------------------------------------
uniform float xMax;
uniform float yMax;
uniform float zMax;


void main()
{
        view_mat = mv_matrix;
	gl_Position = proj_matrix * mv_matrix * gl_Vertex;
	position = gl_Vertex.xyz;

        textCoord.x = position.x / xMax;
        textCoord.y = position.y / yMax;
        textCoord.z = position.z / zMax;

}
