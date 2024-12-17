#version 430 core

float flt_max = 3.402823466e+38;
float flt_min = 1.175494351e-38;
float PI = 3.1415926;

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
uniform vec4 facteurWorley;

uniform Plan plans[6];

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform float absorptionLight;

uniform vec3 BBmin;
uniform vec3 BBmax;


// --------------------------------------------------
// In/Out Variables
// --------------------------------------------------

in mat4 view_mat;  // View matrix passed from the vertex shader
in mat4 proj_mat;

in vec3 fragPosition;  // Output vertex position to fragment shader

out vec4 fragColor;  // The output color of the fragment


vec3 centresphere = vec3(0.0);
float radius = 0.490;



// --------------------------------------------------
// Functions
// --------------------------------------------------

float beersLaw(float distance, float absorbtion ) {
    return exp(-distance * absorbtion );
}

float HenyeyGreenstein(float g , float costh) {
    return (1.0 / (4.0 * PI)) * ((1.0 - g * g ) / pow(1.0 + g - 2.0 * g * costh , 1.5));
}

void IntersectionPlan(vec3 camPos, float epsilon, vec3 dir,out vec3 tEntryOut,out vec3 tExitOut) {
    float tExit = flt_max;  // Initialisation pour trouver le plus petit tMax
    float tEntry = 0.0;    // Initialisation pour trouver le plus grand tMin

    bool isInside = true;  // Pour vérifier si fragPosition est déjà dans la boîte

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

        float u = dot(localCoord, plans[i].right_vect) / length(plans[i].right_vect);
        float v = dot(localCoord, plans[i].up_vect) / length(plans[i].up_vect);

        // Dimensions de la boîte
        float boxWidth = length(plans[i].right_vect);// BBmax.x - BBmin.x;
        float boxHeight = length(plans[i].up_vect) ;//BBmax.y - BBmin.y;

        if (u >= 0.0 && u <= boxWidth && v >= 0.0 && v <= boxHeight) {
            // Intersection valide, classifier en entrée ou sortie
            if (dot(planNormal, dir) < 0.0) { // Plan orienté vers l'entrée
                tEntry = max(tEntry, t);
                isInside = false; // Le point n'est pas dans la boîte
            } else { // Plan orienté vers la sortie
                tExit = min(tExit, t);
            }
        }
    }

    if (isInside) {
        // Si le fragment est déjà dans la boîte, on considère le point d'entrée à `fragPosition`
        tEntryOut = fragPosition;
    } else if (tEntry <= tExit && tExit < flt_max) {
        // Si on a un intervalle valide pour l'entrée et la sortie
        tEntryOut = fragPosition + tEntry * dir;
    } else {
        // Pas d'intersection détectée
        tEntryOut = fragPosition;
    }

    // Calcul du point de sortie
    if (tExit < flt_max) {
        tExitOut = fragPosition + tExit * dir;
    } else {
        tExitOut = fragPosition; // Pas de point de sortie trouvé
    }
}


vec3 translate_in_tex_coord(vec3 point_i){

    vec3 point_i_tex_coord;

    point_i_tex_coord = (point_i - BBmin) / (BBmax - BBmin);

    return point_i_tex_coord;
}

vec3 point_i_in_tex3D(vec3 pos, float dist , vec3 dir ,int i)
{
    vec3 point_i = pos + (i * dist * dir);
    return point_i;
}


float anisotropic_scatering(float g , vec3 point_j,  vec3 dir){
    float costh = max(dot(normalize(point_j - LightPos) , dir) , 0 );
    return mix(HenyeyGreenstein(g , costh) , HenyeyGreenstein(-g , costh) , 0.3);
}






// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------

void main() {

    float epsilon = 0.0001;
    float transparence = 1.0;
    float LightDensity = absorptionLight;
    float LightStepSize = 1.0 / float (LightSample);
    float NuageStepSize = 1.0 / float (NuageSample);
    LightDensity *= float(LightStepSize);

    vec3 LightEnergy = vec3(0.0);
    float NuageDensity = absorptionNuage;
    NuageDensity *= NuageStepSize;

    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);

    vec3 dir = normalize(fragPosition - camPos )  ;

    vec3 exitPoint;
    vec3 entryPoint;
    IntersectionPlan(camPos , epsilon , dir, entryPoint, exitPoint);
    float dist = length(exitPoint - entryPoint)  ;

    if(dist < 0.1){
        discard;
    }

    vec4 textureValue = vec4(0.0);
    float densite = 0.0;


    for (int i = 0 ; i < NuageSample; i++){

        vec3 point_i = point_i_in_tex3D(entryPoint, dist * NuageStepSize , dir  , i );
        vec3 point_i_tex_coord = translate_in_tex_coord(point_i);


//        if (length(point_i - centresphere) > radius )
//            continue;

//       float distance_point_i_centre = smoothstep(1.0 , 0.0 ,length(vec3(0) - point_i) ) ;

//       if (length(vec3(0) - point_i) < distance_point_i_centre )
//            continue;


        textureValue =  dist * NuageStepSize * texture(tex,  point_i_tex_coord);
        //densite = ((textureValue.g + textureValue.b + textureValue.a)/3. - textureValue.r*3.0);
        densite = (textureValue.r * facteurWorley[0] + textureValue.g * facteurWorley[1] + textureValue.b * facteurWorley[2] + textureValue.a * facteurWorley[3]);
        if (densite > epsilon)
        {
            vec3 Ipos = point_i;
            float dist_light = 0;
            vec4 textureValueLight = vec4(0.0);
            vec3 dir_light = normalize(LightPos - point_i);
            vec3 exitPointForLight;
            vec3 entryPointForLight;
            IntersectionPlan(point_i , epsilon , dir_light, entryPointForLight, exitPointForLight);
            float dist_light_point = length (LightPos - point_i);
            float dist_J = length (exitPointForLight - point_i);
            float c = 1.0;
            float phase_attenuation = 0.1;
            for (int j = 0 ; j < LightSample; j++)
            {

                Ipos = point_i_in_tex3D(Ipos,LightStepSize * dist_light_point , dir_light , j);
                if (length(exitPointForLight-Ipos) > length(exitPointForLight-point_i) ){
                    break;
                }
                vec3 point_light_j_tex_coord = translate_in_tex_coord(Ipos);

                textureValueLight = dist_J * LightStepSize * texture(tex,  point_light_j_tex_coord);

                //float luminance = ((textureValueLight.g + textureValueLight.b + textureValueLight.a)/3.0 - textureValueLight.r*3);
                float luminance = (textureValueLight.r * facteurWorley[0] + textureValueLight.g * facteurWorley[1] + textureValueLight.b * facteurWorley[2] + textureValueLight.a * facteurWorley[3]);

                dist_light += luminance * anisotropic_scatering(0.3 * c , Ipos , dir_light);

                c *= (1.0 - phase_attenuation);

            }
            float cur_density= densite * NuageDensity;
            float shadowterm = beersLaw(dist_light, LightDensity);
            vec3 absorbed_light = vec3(cur_density * shadowterm );
            LightEnergy += absorbed_light * transparence;
            transparence *= 1 - cur_density;




        }

    }
    float a =  1.0 - transparence;



    fragColor = vec4(couleurNuage * LightColor * LightEnergy  ,  a /* * LightEnergy */); // Visualisation distance
    //fragColor = vec4(couleurNuage,1.0);
}

