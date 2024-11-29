#version 430 core

// Uniformes
uniform vec3 LightColor;  // Couleur de la lumière

// Entrées
in vec3 lightposition;    // Position de la lumière dans l'espace monde
in vec3 frag_pos;         // Position du fragment dans l'espace monde

// Sortie
out vec4 fragColor;       // Couleur finale du fragment

void main(void)
{
    // Distance entre le fragment et la position de la lumière
    float dist = length(frag_pos - lightposition);

    // Transition lissée : entre une distance de 0.2 et 0.7
    float smoothtransition = smoothstep(0.2, 0.7, dist);

    // Calcul de la couleur finale avec une transparence basée sur la distance
    fragColor = vec4(LightColor,1.0 - smoothtransition); // Couleur RGB et alpha
}
