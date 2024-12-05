#version 430 core

// Uniformes
uniform vec3 LightColor;  // Couleur de la lumière
uniform vec3 LightPos;  // Position de la lumière
uniform vec3 LightDir;  // Direction de la lumière

// Entrées
in vec3 frag_pos;         // Position du fragment dans l'espace monde
in vec3 cam_pos;

// Sortie
out vec4 fragColor;       // Couleur finale du fragment

void main(void)
{

    float coefLight = max(dot(normalize(cam_pos-frag_pos) , LightDir ) , 0.0) ;

    vec3 mixcolor = mix(LightColor  ,  vec3(0.5, 0.8, 0.9) ,  smoothstep(0.2, 0.7 , coefLight));




    fragColor = vec4(mixcolor * (LightPos.y  / 25.0) , 1.0); // Couleur RGB et alpha
}

