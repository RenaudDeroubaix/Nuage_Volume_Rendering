#version 430 core

float flt_max = 3.402823466e+38;
float flt_min = 1.175494351e-38;
float PI = 3.1415926;

struct Plan {
    vec3 point;
    vec3 normale;
    vec3 up_vect;
    vec3 right_vect;
};

// ------------------------------ Uniforms ------------------------------
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

// ------------------------------ In/Out Variables ------------------------------
in mat4 view_mat;
in mat4 proj_mat;
in vec3 fragPosition;
out vec4 fragColor;

// ------------------------------ Functions ------------------------------
float beersLaw(float distance, float absorption) {
    return exp(-distance * absorption);
}

float HenyeyGreenstein(float g, float costh) {
    return (1.0 / (4.0 * PI)) * ((1.0 - g * g) / pow(1.0 + g * g - 2.0 * g * costh, 1.5));
}

void IntersectionPlan(vec3 camPos, float epsilon, vec3 dir, out vec3 tEntryOut, out vec3 tExitOut) {
    float tExit = flt_max;
    float tEntry = 0.0;
    bool isInside = true;

    for (int i = 0; i < 6; i++) {
        vec3 planNormal = plans[i].normale;
        vec3 planPoint = plans[i].point;
        float denom = dot(planNormal, dir);

        if (abs(denom) < epsilon) continue; // Skip plan parralele

        float t = dot(planPoint - fragPosition, planNormal) / denom;
        vec3 intersection = fragPosition + t * dir;
        vec3 localCoord = intersection - planPoint;

        float u = dot(localCoord, plans[i].right_vect) / length(plans[i].right_vect);
        float v = dot(localCoord, plans[i].up_vect) / length(plans[i].up_vect);
        float boxWidth = length(plans[i].right_vect);
        float boxHeight = length(plans[i].up_vect);

        if (u >= 0.0 && u <= boxWidth && v >= 0.0 && v <= boxHeight) {
            if (dot(planNormal, dir) < 0.0) {
                tEntry = max(tEntry, t);
                isInside = false;
            } else {
                tExit = min(tExit, t);
            }
        }
    }

    tEntryOut = isInside ? fragPosition : fragPosition + tEntry * dir;
    tExitOut = (tEntry <= tExit && tExit < flt_max) ? fragPosition + tExit * dir : fragPosition;
}

vec3 translate_in_tex_coord(vec3 point_i) {
    return (point_i - BBmin) / (BBmax - BBmin);
}

vec3 point_i_in_tex3D(vec3 pos, float dist, vec3 dir, int i) {
    return pos + (i * dist * dir);
}

float anisotropic_scattering(float g, vec3 point_j, vec3 dir) {
    float costh = max(dot(normalize(point_j - LightPos), dir), 0.0);
    return HenyeyGreenstein(g, costh);
}

float powder_effect(vec3 lightDir, vec3 viewDir) {
    float dotLV = dot(normalize(lightDir), normalize(viewDir));
    return pow(max(dotLV, 0.0), 4.0);
}

// ------------------------------ Main Shader Logic ------------------------------
void main() {
    float epsilon = 0.0001;
    float transparence = 1.0;
    float LightDensity = absorptionLight * (1.0 / float(LightSample));
    float NuageDensity = absorptionNuage * (1.0 / float(NuageSample));

    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);
    vec3 dir = normalize(fragPosition - camPos);

    vec3 entryPoint, exitPoint;
    IntersectionPlan(camPos, epsilon, dir, entryPoint, exitPoint);
    float dist = length(exitPoint - entryPoint);

    if (dist < 0.1) discard;//skip frag qui qui n'affiche pas de nuage

    vec3 LightEnergy = vec3(0.0);
    vec4 textureValue;

    for (int i = 0; i < NuageSample; i++) {
        vec3 point_i = point_i_in_tex3D(entryPoint, dist * (1.0 / float(NuageSample)), dir, i);
        vec3 point_i_tex_coord = translate_in_tex_coord(point_i);

        textureValue = texture(tex, point_i_tex_coord);
        float density = dot(textureValue.rgb, facteurWorley.rgb) + textureValue.a * facteurWorley.a;

        if (density > epsilon) {
            vec3 dir_light = normalize(LightPos - point_i);
            float dist_light = 0.0;

            for (int j = 0; j < LightSample; j++) {
                vec3 point_j = point_i_in_tex3D(point_i, dist * (1.0 / float(LightSample)), dir_light, j);
                vec3 point_light_tex_coord = translate_in_tex_coord(point_j);

                vec4 lightTexture = texture(tex, point_light_tex_coord);
                float luminance = dot(lightTexture.rgb, facteurWorley.rgb) + lightTexture.a * facteurWorley.a;
                dist_light += luminance * anisotropic_scattering(0.3, point_j, dir_light);
            }

            float shadowTerm = beersLaw(dist_light, LightDensity);
            float powder = powder_effect(dir_light, -dir);

            vec3 absorbed_light = density * NuageDensity * shadowTerm * LightColor * powder;
            LightEnergy += absorbed_light * transparence;
            transparence *= (1.0 - density * NuageDensity);
        }
    }

    float alpha = 1.0 - transparence;


    vec3 finalColor = couleurNuage * LightEnergy + 0.2 * (alpha);

    fragColor = vec4(finalColor, alpha);
}
