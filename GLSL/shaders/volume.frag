#version 460 core

struct Plan{
    vec3 point;
    vec3 normale;
    vec3 up_vect;
    vec3 right_vect;
};

// --------------------------------------------------
// Uniforms
// --------------------------------------------------
uniform sampler3D tex;



uniform int NuageSample;
uniform int LightSample;


uniform vec3 couleurNuage;
uniform float absorptionNuage;

uniform Plan plans[6];

uniform vec3 LightPos;
uniform vec3 LightColor;


uniform vec3 BBmin;
uniform vec3 BBmax;


// --------------------------------------------------
// In/Out Variables
// --------------------------------------------------

in mat4 view_mat;  // View matrix passed from the vertex shader
in mat4 proj_mat;

in vec3 fragPosition;  // Output vertex position to fragment shader
in vec3 fragTexCoord;

out vec4 fragColor;  // The output color of the fragment





// --------------------------------------------------
// Functions
// --------------------------------------------------
float beersLaw(float distance, float absorption) {
    return exp(-distance * absorption );
}

vec3 IntersectionPlan(vec3 camPos , float epsilon , vec3 dir) {
    float tExit = 1000.0; // Initialisation pour trouver le plus petit tMax

    for (int i = 0; i < 6; i++) {
        vec3 planNormal = plans[i].normale;
        vec3 planPoint = plans[i].point;

        // Calcul de t pour ce plan
        float denom = dot(planNormal, dir);
        if (abs(denom) < epsilon) continue; // Rayon parallèle au plan

        float t = dot(planPoint - fragPosition, planNormal) / denom;

        // Vérification si le point est dans les limites du plan
        vec3 intersection = fragPosition + t * dir;
        vec3 localCoord = intersection - planPoint;

        float u = dot(localCoord, plans[i].right_vect);
        float v = dot(localCoord, plans[i].up_vect);

        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && t >= epsilon) {
            tExit = min(tExit, t); // Plus proche sortie

        }
    }

    if (tExit > 0.0 && tExit < 1000.0) {
        vec3 exitPoint = fragPosition + tExit * dir;
        return exitPoint; // Point de sortie
    }

    return fragPosition; // Aucun point trouvé
}

vec3 point_i_in_tex3D(vec3 exitPoint , vec3 dir ,int i)
{
    float dist = length(exitPoint - fragPosition) / float(NuageSample);
    vec3 point_i_texcoord;
    vec3 point_i = fragPosition + i * dist * dir;
    point_i_texcoord.x = (point_i.x -BBmin.x) / (BBmax.x - BBmin.x) ;
    point_i_texcoord.y = (point_i.y -BBmin.y) / (BBmax.y - BBmin.y) ;
    point_i_texcoord.z = (point_i.z -BBmin.z)  / (BBmax.z - BBmin.z);
    return point_i_texcoord;


}

// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------
void main() {
    float epsilon = 0.001;
    float a = 0.0;

    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);
    vec3 dir = normalize(fragPosition - camPos);

    vec3 exitPoint = IntersectionPlan(camPos , epsilon , dir);

    vec4 distance = vec4(0.0);
    float dist = length(exitPoint - fragPosition) / float(NuageSample) ;
    for (int i = 1 ; i < NuageSample ; i++){
       vec3 point_i = point_i_in_tex3D(exitPoint , dir , i);
       distance +=  dist * texture(tex,  point_i);

    }

    vec4 amplitude= vec4(0.6,0.25,0.125,0.05);
    a =1.0-  beersLaw(distance.r *amplitude.r + distance.g * amplitude.g + distance.b*amplitude.b + distance.a*amplitude.a , absorptionNuage);

    fragColor = vec4(couleurNuage, a); // Visualisation distance

    vec4 colorb= texture(tex,  fragTexCoord);
    fragColor = vec4(colorb);

}
