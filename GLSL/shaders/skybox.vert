#version 430 core

uniform mat4 mv_matrix;   // Matrice modèle-vue
uniform mat4 proj_matrix; // Matrice de projection


// Entrée du sommet
in vec3 position;  // Position du sommet en espace local

// Sorties
out vec3 frag_pos;     // Position du fragment dans l'espace monde
out vec3 cam_pos;

void main()
{
    cam_pos = -vec3(mv_matrix[3][0], mv_matrix[3][1], mv_matrix[3][2]) * mat3(mv_matrix);

    frag_pos = position;     // Passer la position du fragment dans l'espace monde

    // Projeter la position dans l'espace de clip
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

}

