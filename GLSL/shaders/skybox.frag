#version 430 core

// Uniformes
uniform vec3 LightColor;  // Couleur de la lumière
uniform vec3 LightPos;  // Position de la lumière
uniform vec3 LightDir;  // Direction de la lumière
// Entrées
in vec3 frag_pos;         // Position du fragment dans l'espace monde
//in vec3 cam_pos;
// Sortie
out vec4 fragColor;       // Couleur finale du fragment
void main()

{

    float Hauteur = max(dot(LightDir ,  vec3(0,1,0) ) , 0.0 ); //hauteur de la lumiere
    vec3 projH = LightDir - dot(LightDir , vec3(0,1,0)) * vec3(0,1,0) ;
    float blend = max (dot(projH,   LightDir) - 0.2  ,0.0) ;
    float horizonsurskybox = max(dot(normalize(-frag_pos) , vec3(0,1,0)) + 0.2  , 0.);


    vec3 couleurciel = mix( 0.2 * vec3(0.5, 0.8, 0.9)  ,  vec3(0.5, 0.8, 0.9) ,   Hauteur);
    couleurciel = mix(couleurciel , vec3(1)  ,  smoothstep(0.0 , 0.3 , horizonsurskybox * blend) );
    vec3 couleurhorizon = mix (LightColor, vec3(1.0, 0.7, 0.3 ) , smoothstep(0.0 , 0.4 , horizonsurskybox * blend));

   // couleurhorizon = smoothstep(0.2 , 0.8 , horizonsurskybox * blend);
    fragColor = vec4(  couleurciel * couleurhorizon , 1.0); // Couleur RGB et alpha

}
