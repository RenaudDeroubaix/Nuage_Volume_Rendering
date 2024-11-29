#version 430 core

// Uniformes
uniform mat4 mv_matrix;   // Matrice modèle-vue
uniform mat4 proj_matrix; // Matrice de projection
uniform vec3 LightPos;    // Position de la lumière
uniform float rayon;      // Rayon de la lumière

// Entrée du sommet
in vec3 position;  // Position du sommet en espace local

// Sorties
out vec3 frag_pos;     // Position du fragment dans l'espace monde
out vec3 lightposition; // Position de la lumière dans l'espace monde

void main()
{
    // Vecteurs de base pour la caméra
    vec3 right = normalize(vec3(mv_matrix[0][0], mv_matrix[1][0], mv_matrix[2][0])); // Vecteur vers la droite
    vec3 up = normalize(vec3(mv_matrix[0][1], mv_matrix[1][1], mv_matrix[2][1]));    // Vecteur vers le haut
    vec3 front = -normalize(vec3(mv_matrix[0][2], mv_matrix[1][2], mv_matrix[2][2]));
    // Calcul de la position du plan en monde
    vec3 camPos = -vec3(mv_matrix[3][0], mv_matrix[3][1], mv_matrix[3][2]) * mat3(mv_matrix);

    vec3 world_position = LightPos + right * position.x + up * position.y + front * position.z  ; // Calcul de la position du plan

    frag_pos = position;     // Passer la position du fragment dans l'espace monde
    lightposition = LightPos;      // Passer la position de la lumière dans l'espace monde

    // Projeter la position dans l'espace de clip
    gl_Position = proj_matrix * mv_matrix * vec4(world_position, 1.0);

}


