#version 330 core

out vec4 FragColor;    // Couleur de sortie
uniform vec3 lightColor; // Couleur de la lumière (fournie depuis le CPU)

void main() {
    FragColor = vec4(lightColor, 1.0); // Applique une couleur uniforme
}
