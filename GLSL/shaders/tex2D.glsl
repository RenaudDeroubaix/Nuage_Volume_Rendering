#version 460 core
layout(local_size_x = 8,local_size_y = 8) in;

layout(rgba32f, binding = 1) uniform image2D img_out2D;


uniform float u_time;
uniform vec2 resolution;
uniform vec4 frequenceCurl;

// Constante pour le générateur pseudo-aléatoire
const uint SEED = 12345u;

// Fonction pour générer des nombres pseudo-aléatoires (basée sur les coordonnées)
float rand(vec2 co) {
    uint hash = uint(co.x) * 73856093u ^ uint(co.y) * 19349663u ^ SEED;
    hash = (hash >> 16u) ^ hash;
    return float(hash & 0xFFFFFFu) / float(0xFFFFFFu);
}

void main() {
    // Récupérer les coordonnées globales dans la texture
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    // Dimensions du bruit
    vec2 u_resolution = resolution;


    // Générer des couleurs aléatoires (R, G, B)
    float r = rand(vec2(coords) + 0.1);
    float g = rand(vec2(coords) + 0.2);
    float b = rand(vec2(coords) + 0.3);
    float a=1.0;

    // Stocker la couleur dans l'image
    imageStore(img_out2D, coords, vec4(r, g, b, a));
}
