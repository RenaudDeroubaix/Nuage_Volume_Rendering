#version 330 core

// --------------------------------------------------
// Uniforms
// --------------------------------------------------
uniform sampler3D tex;
uniform int segment;

// --------------------------------------------------
// In/Out Variables
// --------------------------------------------------

in mat4 view_mat;  // View matrix passed from the vertex shader
in vec3 fragPosition;  // Output vertex position to fragment shader
in vec3 fragTexCoord;  // Output texture coordinates to fragment shader

out vec4 fragColor;  // The output color of the fragment

// --------------------------------------------------
// Functions
// --------------------------------------------------

float beersLaw(float distance, float absorption) {
    return exp(-distance * absorption);
}

vec3 ray(vec3 inpos) {
    vec3 colorWithTransparency;
    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);
    vec3 dir = normalize(inpos - camPos);
    return dir;
}

vec3 point_i_rayon(int i, int nb_segment, vec3 dir, vec3 campos, float dist_max) {
    // Normaliser la direction pour obtenir un vecteur unitaire
    vec3 dir_normalized = normalize(dir);

    // Calculer la distance entre les points sur le rayon
    float distance = (dist_max * float(i)) / float(nb_segment);

    // Calculer le point en fonction de la distance
    vec3 point = campos + (dir_normalized * distance);

    return point;
}



// vec3 rayTrace(vec3 inpos){
//         vec3 camPos = (inverse(view_mat) * vec4(0, 0, 0, 1)).xyz;//apparament inverse() existe pas en glsl
//         vec3 dir = normalize(inpos - camPos);
//         float intensity = 0.0;
//         for (int i = 0 ; i <10 ; i++ )
//         {
//             intensity += i * texture(tex , point_i_rayon(i ,10,dir,camPos,128. ) ).r;


//         }
//         float absorption =0.9;//a rendre parametrable plus tard
//         intensity = beersLaw(intensity , absorption);



//         return vec3(1,  intensity, 1);
// }

// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------
void main() {
    //vec4 color = vec4 (rayTrace(position),1.0);
    vec4 color = texture(tex, fragTexCoord);

    // Apply the color to the output fragment color
    fragColor = color;
}
