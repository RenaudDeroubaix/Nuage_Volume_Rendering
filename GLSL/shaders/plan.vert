#version 430 core

uniform mat4 mv_matrix;   // Model-view matrix
uniform mat4 proj_matrix; // Projection matrix

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragNormal;      // Normal for fragment shader
out vec2 fragTexCoord;    // UV for fragment shader

void main() {
    vec3 scale = vec3(7. , 3. , 7. );
    vec3 translation = vec3(0 ,0 , 0 );

    fragNormal = vec3(mv_matrix * vec4(normal,0.0));  // Transform normal to view space

    fragTexCoord = texCoord;               // Pass UV to fragment shader

    mat4 modelmat = transpose(
                 mat4(scale.x,   0   ,   0   ,translation.x,
                          0  ,scale.y,   0   ,translation.y,
                          0  ,   0   ,scale.z,translation.z,
                          0  ,   0   ,   0   ,     1.        )
                    );

    vec4 POS = modelmat * vec4(position, 1.0);


    gl_Position = proj_matrix * mv_matrix * POS;
}
