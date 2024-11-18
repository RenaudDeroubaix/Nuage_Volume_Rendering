#ifndef FNC_WORLEY
#define FNC_WORLEY

#ifndef WORLEY_JITTER
#define WORLEY_JITTER 1.0
#endif

#ifndef WORLEY_DIST_FNC
#define WORLEY_DIST_FNC distEuclidean
#endif

#define RANDOM_SCALE vec4(443.897, 441.423, .0973, .1099)

float distEuclidean(vec2 a, vec2 b) { return distance(a, b); }
float distEuclidean(vec3 a, vec3 b) { return distance(a, b); }
float distEuclidean(vec4 a, vec4 b) { return distance(a, b); }

vec2 random2(float p) {
    vec3 p3 = fract(vec3(p) * RANDOM_SCALE.xyz);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.xx + p3.yz) * p3.zy);
}

vec2 random2(vec2 p) {
    vec3 p3 = fract(p.xyx * RANDOM_SCALE.xyz);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.xx + p3.yz) * p3.zy);
}

vec2 random2(vec3 p3) {
    p3 = fract(p3 * RANDOM_SCALE.xyz);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.xx + p3.yz) * p3.zy);
}

vec3 random3(float p) {
    vec3 p3 = fract(vec3(p) * RANDOM_SCALE.xyz);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.xxy + p3.yzz) * p3.zyx);
}

vec3 random3(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * RANDOM_SCALE.xyz);
    p3 += dot(p3, p3.yxz + 19.19);
    return fract((p3.xxy + p3.yzz) * p3.zyx);
}

vec3 random3(vec3 p) {
    p = fract(p * RANDOM_SCALE.xyz);
    p += dot(p, p.yxz + 19.19);
    return fract((p.xxy + p.yzz) * p.zyx);
}

vec2 worley2(vec2 p){
    vec2 n = floor( p );
    vec2 f = fract( p );

    float distF1 = 1.0;
    float distF2 = 1.0;
    vec2 off1 = vec2(0.0);
    vec2 pos1 = vec2(0.0);
    vec2 off2 = vec2(0.0);
    vec2 pos2 = vec2(0.0);
    for( int j= -1; j <= 1; j++ ) {
        for( int i=-1; i <= 1; i++ ) {
            vec2  g = vec2(i,j);
            vec2  o = random2( n + g ) * WORLEY_JITTER;
            vec2  p = g + o;
            float d = WORLEY_DIST_FNC(p, f);
            if (d < distF1) {
                distF2 = distF1;
                distF1 = d;
                off2 = off1;
                off1 = g;
                pos2 = pos1;
                pos1 = p;
            }
            else if (d < distF2) {
                distF2 = d;
                off2 = g;
                pos2 = p;
            }
        }
    }

    return vec2(distF1, distF2);
}

float worley(vec2 p){ return 1.0-worley2(p).x; }

vec2 worley2(vec3 p) {
    vec3 n = floor( p );
    vec3 f = fract( p );

    float distF1 = 1.0;
    float distF2 = 1.0;
    vec3 off1 = vec3(0.0);
    vec3 pos1 = vec3(0.0);
    vec3 off2 = vec3(0.0);
    vec3 pos2 = vec3(0.0);
    for( int k = -1; k <= 1; k++ ) {
        for( int j= -1; j <= 1; j++ ) {
            for( int i=-1; i <= 1; i++ ) {
                vec3  g = vec3(i,j,k);
                vec3  o = random3( n + g ) * WORLEY_JITTER;
                vec3  p = g + o;
                float d = WORLEY_DIST_FNC(p, f);
                if (d < distF1) {
                    distF2 = distF1;
                    distF1 = d;
                    off2 = off1;
                    off1 = g;
                    pos2 = pos1;
                    pos1 = p;
                }
                else if (d < distF2) {
                    distF2 = d;
                    off2 = g;
                    pos2 = p;
                }
            }
        }
    }

    return vec2(distF1, distF2);
}

float worley(vec3 p){ return 1.0-worley2(p).x; }

#endif
