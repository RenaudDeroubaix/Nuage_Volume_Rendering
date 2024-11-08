#version 140


// --------------------------------------------------
// shader definition
// --------------------------------------------------

uniform sampler3D tex;
uniform  int segment;

varying vec3 position;
varying vec3 textCoord;

varying mat4 view_mat;


// --------------------------------------------------
// Fragment Shader:
// --------------------------------------------------

vec3 point_i_rayon(int i, int nb_segment, vec3 dir, vec3 campos, float dist_max)
{
    // Normaliser la direction pour obtenir un vecteur unitaire
    vec3 dir_normalized = normalize(dir);

    // Calculer la distance entre les points sur le rayon
    float distance = (dist_max * float(i)) / float(nb_segment);

    // Calculer le point en fonction de la distance
    vec3 point = campos + (dir_normalized * distance);

    return point;
}


vec3 rayTrace(vec3 inpos){
        vec3 camPos = (inverse(view_mat) * vec4(0, 0, 0, 1)).xyz;
        vec3 dir = normalize(inpos - camPos);
        float intensity = 0.0;
        for (int i = 0 ; i <10 ; i++ )
        {
            intensity += i * texture(tex , point_i_rayon(i ,10,dir,camPos,128. ) ).r;


        }
        float absorption =0.3;
        intensity = exp( -intensity * absorption);



        return vec3(1,  intensity, 1);
}

void main() {
    vec4 color = vec4 (rayTrace(position),1.0);
    //vec4 color = texture(tex , textCoord);

    gl_FragColor = color;
}
