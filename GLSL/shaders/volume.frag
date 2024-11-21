#version 330 core
#define PI 3.14159265358979
struct Plan{
    vec3 point;
    vec3 normale;
    vec3 up_vect;
    vec3 right_vect;
};

// ------------------------------intensite--------------------
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

out vec4 fragColor;  // The output color of the fragment



float HenyeyGreenstein(float g , float costh) {
    return (1.0 / (4.0 * PI)) * ((1.0 - g * g ) / pow(1.0 + g - 2.0 * g * costh , 1.5));
}

// --------------------------------------------------
// Functions
// --------------------------------------------------
float beersLaw(float distance ) {
    return exp(-distance );
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

vec3 translate_in_tex_coord(vec3 point_i){

    vec3 point_i_tex_coord;

    point_i_tex_coord = (point_i - BBmin) / (BBmax - BBmin);
    point_i_tex_coord.z*=-1;

    return point_i_tex_coord;
}
vec3 point_sample_light_j(float dist , vec3 dir , int i , vec3 exitPointForLight , vec3 point_i)
{

    vec3 point_sample_light_j = point_i + (i * dist * dir);
    if (length(point_sample_light_j - point_i) < length(exitPointForLight - point_i)){
        return point_sample_light_j;
    }
}

vec3 point_i_in_tex3D(float dist , vec3 dir ,int i)
{
    vec3 point_i = fragPosition + (i * dist * dir);
    return point_i;
}

float anisotropic_scatering(float g , vec3 point_i, vec3 dir){
    float costh = dot(dir, normalize(LightPos -  point_i));
    return  mix(HenyeyGreenstein(g , costh) , HenyeyGreenstein(-g , costh) , 0.7);
}

// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------

void main() {
    float epsilon = 0.0001;
    float a = 0.0;
    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);
    vec3 dir = normalize(fragPosition - camPos );

    vec3 exitPoint = IntersectionPlan(camPos , epsilon , dir);

    vec4 distance = vec4(0.0);
    vec4 obscuration_lumiere_au_point_j = vec4(0.0);

    float g = 0.0;
    float lumierecumuleFragpos=0.0;

    float dist = length(exitPoint - fragPosition) / NuageSample ;

    for (int i = 1 ; i < NuageSample; i++){

        vec3 point_i = point_i_in_tex3D(dist , dir , i );
        vec3 point_i_tex_coord = translate_in_tex_coord(point_i);

        distance += dist * texture(tex,  point_i_tex_coord);



        vec3 dir_light = normalize(LightPos - point_i);
        vec3 exitPointForLight = IntersectionPlan(point_i , epsilon , dir_light);
        float dist_light = length(LightPos - point_i) / LightSample ;

        for (int j = 0 ; j < LightSample; j++){

            vec3 point_light_j = point_sample_light_j(dist_light, dir_light , j , exitPointForLight , point_i );
            vec3 point_light_j_tex_coord = translate_in_tex_coord(point_light_j);
            obscuration_lumiere_au_point_j += dist_light * texture(tex,  point_light_j_tex_coord);
            lumierecumuleFragpos += anisotropic_scatering ( obscuration_lumiere_au_point_j.r , point_light_j, dir)  ;

        }


    }


    a = 1 - beersLaw(distance.r * absorptionNuage);

    fragColor = vec4(couleurNuage * LightColor * lumierecumuleFragpos    ,  a    ); // Visualisation distance
}
