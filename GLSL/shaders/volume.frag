#version 330 core

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
uniform int segment;

uniform vec3 couleurNuage;
uniform float absorptionNuage;

uniform Plan plans[6];

uniform vec3 BBmin;
uniform vec3 BBmax;


// --------------------------------------------------
// In/Out Variables
// --------------------------------------------------

in mat4 view_mat;  // View matrix passed from the vertex shader
in mat4 proj_mat;

in vec3 fragPosition;  // Output vertex position to fragment shader
in vec3 fragTexCoord;  // Output texture coordinates to fragment shader

out vec4 fragColor;  // The output color of the fragment





// --------------------------------------------------
// Functions
// --------------------------------------------------
float beersLaw(float distance, float absorption) {
    return exp(-distance * absorption);
}


vec3 intersectionRayPlan(vec3 fragPos, vec3 camPos) {
    float epsilon = 0.001;
    vec3 dir = normalize(fragPos - camPos);
    vec3 closestPoint = fragPos;
    float minT = 1e9;

    for (int i = 0; i < 6; i++) {
        // Plane normal and point
        vec4 nplan = view_mat * vec4(plans[i].normale, 0.0);
        vec4 pplan = view_mat * vec4(plans[i].point, 1.0);
        vec4 rplan = view_mat * vec4(plans[i].right_vect, 0.0);
        vec4 uplan = view_mat * vec4(plans[i].up_vect, 0.0);

        // Distance to the plane
        float D = dot(vec3(pplan), vec3(nplan));
        float t = (D - dot(fragPos, vec3(nplan)));
        if (t < epsilon) continue; // evite l'intersections sur le plan de fragPos

        // Intersection point
        vec3 r = fragPos + t * dir;

        float right_length_squared = dot(vec3(rplan), vec3(rplan));
        float up_length_squared = dot(vec3(uplan), vec3(uplan));
        vec3 relPoint = r - vec3(pplan);

        float u = dot(vec3(rplan), relPoint) / right_length_squared;
        float v = dot(vec3(uplan), relPoint) / up_length_squared;

        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0) {
            // Track closest intersection
            if (t < minT) {
                minT = t;
                closestPoint = r;
            }
        }
    }

    return closestPoint;
}

vec3 IntersectionPlan(vec3 fragPos, vec3 camPos) {
    vec3 dir = normalize(fragPos - camPos);
    float tExit = 1000.0; // Initialisation pour trouver le plus petit tMax
    float epsilon = 0.0001;
    for (int i = 0; i < 6; i++) {
        vec3 planNormal = plans[i].normale;
        vec3 planPoint = plans[i].point;

        // vec4 nplan = view_mat * vec4(plans[i].normale, 0.0);
        // vec4 pplan = view_mat * vec4(plans[i].point, 1.0);
        // vec3 planNormal = nplan.xyz;
        // vec3 planPoint = pplan.xyz;


        // Calcul de t pour ce plan
        float denom = dot(planNormal, dir);
        if (abs(denom) < epsilon) continue; // Rayon parallèle au plan

        float t = dot(planPoint - fragPos, planNormal) / denom;

        // Vérification si le point est dans les limites du plan
        vec3 intersection = fragPos + t * dir;
        vec3 localCoord = intersection - planPoint;

        float u = dot(localCoord, plans[i].right_vect);
        float v = dot(localCoord, plans[i].up_vect);

        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && t >= epsilon) {
            tExit = min(tExit, t); // Plus proche sortie

        }
    }

    if (tExit > 0.0 && tExit < 1000.0) {
        vec3 exitPoint = fragPos + tExit * dir;
        return exitPoint; // Point de sortie
    }

    return fragPos; // Aucun point trouvé
}

// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------
void main() {
    vec4 noise = texture(tex, fragTexCoord);

    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);

    // vec3 intersectionPoint = intersectionRayPlan(fragPosition, camPos);

    // float dist = length(fragPosition - intersectionPoint);


    // float a = 1 * beersLaw(dist,absorptionNuage);

    // // Apply the color to the output fragment color
    // //fragColor = vec4( fragPosition - intersectionRayPlan(fragPosition,camPos),a);
    // //fragColor = vec4(  intersectionRayPlan(fragPosition,camPos),a);
    // fragColor = vec4(couleurNuage, a);

    vec3 exitPoint = IntersectionPlan(fragPosition, camPos);
    float a = 0.0;
    float dist = length(exitPoint - fragPosition);
    if(dist > 0.0001){

        a = 1 - beersLaw(dist,absorptionNuage );
    }

    fragColor = vec4(couleurNuage, a); // Visualisation distance

}
