#version 430 core

// Uniformes
uniform vec3 LightColor;  // Couleur de la lumière
uniform vec3 LightPos;  // Position de la lumière
uniform vec3 LightDir;  // Direction de la lumière
uniform float maxy;
// Entrées
in vec3 frag_pos;         // Position du fragment dans l'espace monde
in vec3 cam_pos;

// Sortie
out vec4 fragColor;       // Couleur finale du fragment

void main(void)
{

    float coefLight = abs(dot( normalize(LightPos - frag_pos) , LightDir ) ) ;

    vec3 couleurciel = mix(vec3(0.2 , 0.2 , 0.2)  ,  vec3(0.5, 0.8, 0.9) ,  smoothstep(0.1, 0.3 , sqrt((LightPos.y/maxy))));
    vec3 mixcolor2 = mix(LightColor , couleurciel  ,  smoothstep(0.1, 0.3 , min(coefLight , (LightPos.y/maxy))));


    fragColor = vec4((couleurciel *  mixcolor2) , 1.0); // Couleur RGB et alpha
}

