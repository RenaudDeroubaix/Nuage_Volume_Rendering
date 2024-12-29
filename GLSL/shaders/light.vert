#version 430 core

// Uniformes
uniform mat4 mv_matrix;   // Matrice modèle-vue
uniform mat4 proj_matrix; // Matrice de projection
uniform vec3 LightPos;    // Position de la lumière
uniform float rayon;      // Rayon du plan (distance à la lumière)

// Entrée du sommet
in vec3 position;  // Position du sommet en espace local

// Sorties
out vec3 frag_pos;     // Position du fragment dans l'espace monde
out vec3 lightposition; // Position de la lumière dans l'espace monde

void main()
{


    // Vecteurs de base pour la caméra
    vec3 right = normalize(vec3(mv_matrix[0][0], mv_matrix[1][0], mv_matrix[2][0]));
    vec3 up = normalize(vec3(mv_matrix[0][1], mv_matrix[1][1], mv_matrix[2][1]));
    vec3 front = normalize(vec3(mv_matrix[0][2], mv_matrix[1][2], mv_matrix[2][2]));

    // Calcul de la position du sommet sur le plan
    vec3 world_position =  position.x * right + position.y * up + position.z * front  ;
    vec4 finalpos = proj_matrix * mv_matrix * vec4(world_position, 1.0);
    frag_pos = position;
    lightposition = LightPos;

    // Projeter la position dans l'espace de clip
    gl_Position = finalpos;
}
