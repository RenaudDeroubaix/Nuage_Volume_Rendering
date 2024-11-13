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
//TODO calculer l'intersection rayon quad
//        Vec3 o,n,d,X,Y,r,projX,projY;
//        float t,D,u,v;
//        o=ray.origin();
//        d=ray.direction();
//        n=m_normal;
//        D=Vec3::dot(m_bottom_left ,n );
//        //D=m_bottom_left.norm();
//        t=(D-Vec3::dot(o,n))/Vec3::dot(d,n);
//        r=o+t*d;
//        //projection
//        X=m_right_vector;
//        Y=m_up_vector;
//        projX=Vec3::dot(X,r-m_bottom_left)/X.squareLength()*X;
//        u=projX[0]/X[0];
//        projY=Vec3::dot(Y,r-m_bottom_left)/Y.squareLength()*Y;
//        v=projY[1]/Y[1];
//        float dot=Vec3::dot(d,n);
//       /*std::cout<<" x; "<<X <<std::endl;
//        std::cout<<" y; "<<Y <<std::endl;
//        std::cout<<" b; "<<m_bottom_left <<std::endl;*/
//        if(t>intersection.znear&&u<1&&u>0&&v<1&&v>0&&dot<0){//
//            /*std::cout<<" projX/=X[0]; "<<u <<std::endl;
//            std::cout<<" projY/=Y[1]; "<<v <<std::endl;*/
//            intersection.intersectionExists=true;
//            intersection.t=t;
//            intersection.normal=n;
//            intersection.intersection=r;

//        }
//        else{intersection.intersectionExists=false;}
//        return intersection;



float beersLaw(float distance, float absorption) {
    return exp(-distance * absorption);
}

vec3 intersectionRayPlan(vec3 fragPos , vec3 camPos) {
    float epsilon = 0.05;
    vec3 dir = normalize(fragPos - camPos);
    mat4 mvp =proj_mat * view_mat;
    for(int i = 0 ; i < 6 ; i++)
    {
        vec4 nplan = view_mat * vec4(plans[i].normale ,0.0);
        float parralele = dot(dir, vec3(nplan));
        if (abs(parralele) < epsilon) continue; // Skip parallel planes

        vec4 pplan = mvp * vec4(plans[i].point ,1.0);
        vec4 rplan = view_mat * vec4(plans[i].right_vect ,0.0);
        vec4 uplan = view_mat * vec4(plans[i].up_vect ,0.0);

        float D = dot(vec3(pplan), vec3(nplan));
        float t =( D - dot(fragPos ,  vec3(nplan)) )/dot(dir , vec3(nplan));

        vec3 r = fragPos + t * dir;
        float sqearelenr = length(rplan) * length(rplan);
        float sqearelenu = length(uplan) * length(uplan);

//        vec3 ProjX = dot(vec3(rplan) , r - vec3(pplan))/ sqearelenr * vec3(rplan);
//        float u = ProjX.x/rplan.x;
//        vec3 ProjY = dot(vec3(uplan) , r - vec3(pplan))/ sqearelenu * vec3(uplan);
//        float v = ProjY.y/uplan.y;

        vec3 relPoint = r - vec3(pplan);
        float u = dot(vec3(rplan), relPoint) / sqearelenr;
        float v = dot(vec3(uplan), relPoint) / sqearelenu;

        float orientation = dot(dir,vec3(nplan));

        if(t > epsilon && u <1.0 && v < 1.0 && u>0.0 && v >0.0 && orientation < 0  ){
            return r;
        }
    }
    return fragPos;
}



//float distanceEntreeSortieCube(vec3 ray){

//}



//vec3 point_i_rayon(int i, int nb_segment, vec3 dir, vec3 campos, float dist_max) {
//    // Normaliser la direction pour obtenir un vecteur unitaire
//    vec3 dir_normalized = normalize(dir);

//    // Calculer la distance entre les points sur le rayon
//    float distance = (dist_max * float(i)) / float(nb_segment);

//    // Calculer le point en fonction de la distance
//    vec3 point = campos + (dir_normalized * distance);

//    return point;
//}



// vec3 rayTrace(vec3 inpos){
//         vec3 camPos = (inverse(view_mat) * vec4(0, 0, 0, 1)).xyz;//apparament inverse() existe pas en glsl
//         vec3 dir = normalize(inpos - camPos);
//         float intensity = 0.0;
//         for (int i = 0 ; i <10 ; i++ )
//         {
//             intensity += i * texture(tex , point_i_rayon(i ,10,dir,camPos,128. ) ).r;


//         }
//         float absorption =0.9;//a rendre parametrable plus tard
//         intensity = beersLaw(intensity , absorption);



//         return vec3(1,  intensity, 1);
// }

// --------------------------------------------------
// Main Shader Logic
// --------------------------------------------------
void main() {
    //vec4 color = vec4 (rayTrace(position),1.0);
    vec4 color = texture(tex, fragTexCoord);
    vec3 couleur = couleurNuage;
    vec3 camPos = -vec3(view_mat[3][0], view_mat[3][1], view_mat[3][2]) * mat3(view_mat);
    float dist= (fragPosition - intersectionRayPlan(fragPosition,camPos)).length();
    float a = 1 ;//* beersLaw(dist,absorptionNuage);
    // Apply the color to the output fragment color
    fragColor = vec4( fragPosition - intersectionRayPlan(fragPosition,camPos),a);
}
