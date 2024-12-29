#version 460 core

layout(local_size_x = 8,local_size_y = 8, local_size_z = 8) in;

layout(rgba32f, binding = 0) uniform image3D img_out;

uniform float u_time;
uniform vec3 resolution;
uniform vec4 frequenceWorley;

//gl_LocalInvocationID
//gl_GlobalInvocationID
//gl_WorkGroupID


/////////////////////////////BRUIT DE PERLIN https://stegu.github.io/webgl-noise/webdemo/
//
// GLSL textureless classic 3D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2024-11-07
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise
//

vec3 mod289(vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+10.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));

  float n000 = norm0.x * dot(g000, Pf0);
  float n010 = norm0.y * dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n100 = norm0.z * dot(g100, vec3(Pf1.x, Pf0.yz));
  float n110 = norm0.w * dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = norm1.x * dot(g001, vec3(Pf0.xy, Pf1.z));
  float n011 = norm1.y * dot(g011, vec3(Pf0.x, Pf1.yz));
  float n101 = norm1.z * dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n111 = norm1.w * dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
  return 2.2 * n_xyz;
}

// Classic Perlin noise, periodic variant
float pnoise(vec3 P, vec3 rep)
{
  vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
  vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));

  float n000 = norm0.x * dot(g000, Pf0);
  float n010 = norm0.y * dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n100 = norm0.z * dot(g100, vec3(Pf1.x, Pf0.yz));
  float n110 = norm0.w * dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = norm1.x * dot(g001, vec3(Pf0.xy, Pf1.z));
  float n011 = norm1.y * dot(g011, vec3(Pf0.x, Pf1.yz));
  float n101 = norm1.z * dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n111 = norm1.w * dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
  return 2.2 * n_xyz;
}


/////////////////////////////BRUIT WORLEY LYGIA LIB

// Paramètres de bruit de Worley
#ifndef FNC_WORLEY
#define FNC_WORLEY

#ifndef WORLEY_JITTER
#define WORLEY_JITTER 1.0

/*
Si WORLEY_JITTER = 0 : Le point est toujours au même endroit dans la cellule.
Si WORLEY_JITTER = 1 : Le point est déplacé dans toute la cellule sans contrainte.
Si WORLEY_JITTER > 1 : Le point peut être déplacé au-delà des limites de la cellule, introduisant un bruit encore plus dispersé.
*/
#endif

#ifndef WORLEY_DIST_FNC
#define WORLEY_DIST_FNC distEuclidean
#endif

#define RANDOM_SCALE vec4(443.897, 441.423, .0973, .1099)

// Distance euclidienne
float distEuclidean(vec3 a, vec3 b) { return distance(a, b); }

// Générateur aléatoire 3D
//vec3 random3(vec3 p) {
//    p = fract(p * RANDOM_SCALE.xyz);
//    p += dot(p, p.yzx + 19.19);
//    return fract((p.xxy + p.yzz) * p.zyx);
//}

vec3 random3(vec3 c) {
    float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
    vec3 r;
    r.z = fract(512.0*j);
    j *= .125;
    r.x = fract(512.0*j);
    j *= .125;
    r.y = fract(512.0*j);
    return r-0.5;
}

// Calcul du bruit de Worley en 3D
vec2 worley3(vec3 p) {
    vec3 n = floor(p);  // Cellule actuelle
    vec3 f = fract(p);  // Position relative à l'intérieur de la cellule

    float distF1 = 1.0; // Première distance minimale
    float distF2 = 1.0; // Deuxième distance minimale
    vec3 off1 = vec3(0.0);
    vec3 pos1 = vec3(0.0);
    vec3 off2 = vec3(0.0);
    vec3 pos2 = vec3(0.0);

    // Parcours des cellules voisines
    for (int k = -1; k <= 1; k++) {
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                vec3 g = vec3(i, j, k);                     // Offset de la cellule voisine
                vec3 o = random3(n + g) * WORLEY_JITTER;   // Position aléatoire dans la cellule
                vec3 p = g + o;                            // Position globale
                float d = WORLEY_DIST_FNC(p, f);           // Distance

                if (d < distF1) {
                    distF2 = distF1;
                    distF1 = d;
                    off2 = off1;
                    off1 = g;
                    pos2 = pos1;
                    pos1 = p;
                } else if (d < distF2) {
                    distF2 = d;
                    off2 = g;
                    pos2 = p;
                }
            }
        }
    }

    return vec2(distF1, distF2); // Retourne les deux distances minimales
}

// Génère un bruit de Worley basé sur la première distance
float worley(vec3 p) {
    return 1.0 - worley3(p).x;
}
float perlin(vec3 p) {
    return cnoise(p);
}
float perlin_worley(vec3 p){
    float perlinNoise=perlin(p);
    float worleyNoise=worley(p);
    return mix(perlinNoise, worleyNoise, 0.5);




}

#endif

float sdCapsule(vec3 p, vec3 a, vec3 b, float r) {
  vec3 ab = b - a;
  vec3 ap = p - a;

  float t = dot(ab, ap) / dot(ab, ab);
  t = clamp(t, 0.0, 1.0);

  vec3 c = a + t * ab;

  float d = length(p - c) - r;

  return d;
}

float sdSphere(vec3 p, float radius) {
  return length(p) - radius;
}

float sdTorus(vec3 p, vec2 r) {
  float x = length(p.xz) - r.x;
  return length(vec2(x, p.y)) - r.y;
}

float sdCross(vec3 p, float s) {
  float da = max(abs(p.x), abs(p.y));
  float db = max(abs(p.y), abs(p.z));
  float dc = max(abs(p.z), abs(p.x));

  return min(da, min(db, dc)) - s;
}

float smooth_edge(float distance){
    return smoothstep (0,0.6,distance);
    return 1*(distance*distance*distance * (distance * (distance *6 -15)+10) );//+ 0.01 *sin(6.28 * 64 * distance);
}
void main() {
    // Coordonnées globales dans la texture
    ivec3 coords = ivec3(gl_GlobalInvocationID);

    // Dimensions du bruit
    vec3 u_resolution = resolution;

    // Normaliser les coordonnées pour rester entre 0 et 1
    vec3 st = vec3(coords) / u_resolution;

    // Calcul du bruit de Worley en 3D
    float r = perlin_worley(st * frequenceWorley[0]);
    float g = worley(st * frequenceWorley[1]+ vec3(u_time));
    float b = worley(st * frequenceWorley[2]+ vec3(u_time));
    float a = worley(st * frequenceWorley[3]+ vec3(u_time));

    float attenuation_zone = 0.2;
    vec3 to_edge = min(st, 1.0 - st);
    float edge_distance = min(min(to_edge.x, to_edge.y), to_edge.z);
    float attenuation = smooth_edge(clamp(edge_distance / attenuation_zone, 0.0, 1.0));

    // Initialisation de la couleur
    vec4 color = vec4(r, g, b, a);

    // Position dans l'espace normalisé pour les SDF
    vec3 p = (st - 0.5) * 2.0; // Centrer à l'origine et passer à [-1, 1]
    float min_dimension = min(u_resolution.x, min(u_resolution.y, u_resolution.z));
    float max_radius = min_dimension / u_resolution.x; // Rayon max basé sur l'axe X

    // Appliquer la SDF en fonction de la forme_bruit
    int forme_bruit=4;
    if(forme_bruit ==0){
            // Aucun SDF, juste une atténuation sur les bords
        ;
    }
    else if (forme_bruit == 1) {
        // Sphère
        float sphere_radius = max_radius * 0.5; // Rayon à 50% de la texture
        float sphere_distance = sdSphere(p, sphere_radius);
        attenuation *= smoothstep(0.0, 0.1, -sphere_distance);
    }
    else if (forme_bruit == 2) {
        // Tore
        float major_radius = max_radius * 0.5; // Rayon majeur à 50% de la texture
        float minor_radius = max_radius * 0.2; // Rayon mineur à 20% de la texture
        float torus_distance = sdTorus(p, vec2(major_radius, minor_radius));
        attenuation *= smoothstep(0.0, 0.1, -torus_distance);
    }
    else if (forme_bruit == 3) {
        // Capsule
        vec3 start = vec3(-0.5, 0.0, 0.0); // Début de la capsule
        vec3 end = vec3(0.5, 0.0, 0.0);    // Fin de la capsule
        float capsule_radius = max_radius * 0.2; // Rayon à 20% de la texture
        float capsule_distance = sdCapsule(p, start, end, capsule_radius);
        attenuation *= smoothstep(0.0, 0.1, -capsule_distance);
    }
    else if (forme_bruit == 4) {
        // Croix
        float cross_size = max_radius * 0.3; // Taille des barres
        float cross_distance = sdCross(p, cross_size);
        attenuation *= smoothstep(0.0, 0.1, -cross_distance);
    }


    // Appliquer l'atténuation au bruit
    color *= attenuation;

    // Stocker la couleur dans l'image 3D
    imageStore(img_out, coords, color);
}

/* ancien test a suppr si on y reviens pas
    vec3 to_edge = min(st, 1.0 - st);
    //float edge_distance_1 = min(min(to_edge.x, to_edge.y), to_edge.z);
    //float edge_distance_2 = min(min(to_edge.x, to_edge.y), to_edge.z);
    //float edge_distance = (to_edge.x+to_edge.y+to_edge.z)/3.0;//(edge_distance_1 + edge_distance_2) /2.0;
    //float edge_distance = (to_edge.x+to_edge.y+to_edge.z - max(max(to_edge.x, to_edge.y), to_edge.z) )/2.0;
    float edge_distance = min(min(to_edge.x, to_edge.y), to_edge.z);

    float edge_distance;
    float min_distance = min(min(to_edge.x, to_edge.y), to_edge.z);
    float max_distance = max(max(to_edge.x, to_edge.y), to_edge.z);
    float moy_distance = (to_edge.x + to_edge.y + to_edge.z ) / 3.0;

    // Déterminer si c'est un coin
    if (max_distance - moy_distance < 0.04) { //coin
        edge_distance =(moy_distance*0.25 +min_distance*0.75);
    } else if (max_distance - moy_distance < 0.08) { //coin
        edge_distance =(moy_distance*0.1 +min_distance*0.9);
    } else{
        edge_distance = min_distance;
    }

    int forme_bruit=0;
    if(forme_bruit ==0){
            // Aucun SDF, juste une atténuation sur les bords
        ;
    }
    else if(forme_bruit ==1){
        // Sphère
        float sphere_distance = sdSphere(p, 0.5); // Rayon 0.5
        attenuation *= smoothstep(-0.1, 0.0, sphere_distance); // Transition douce
        }
    else if(forme_bruit ==2){
            // Tore
            float torus_distance = sdTorus(p, vec2(0.6, 0.2)); // Rayon majeur 0.6, mineur 0.2
            attenuation *= smoothstep(-0.1, 0.0, torus_distance);
    }
    else if(forme_bruit ==3){
            // Capsule
            float capsule_distance = sdCapsule(p, vec3(-0.3, 0.0, 0.0), vec3(0.3, 0.0, 0.0), 0.2);
            attenuation *= smoothstep(-0.1, 0.0, capsule_distance);
    }
    else if(forme_bruit ==4){
        // Croix
        float cross_distance = sdCross(p, 0.3); // Taille des barres : 0.3
        attenuation *= smoothstep(-0.1, 0.0, cross_distance);
    }

*/
