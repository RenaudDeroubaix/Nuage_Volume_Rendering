#version 430 core

uniform vec3 LightPos;    // Light position
uniform vec3 LightColor;  // Light color
uniform sampler2D textureSampler; // Texture

in vec3 fragNormal;       // Interpolated normal
in vec2 fragTexCoord;     // Interpolated texture coordinates

out vec4 fragColor;       // Output color

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(LightPos);

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;

    // Sample texture
    vec4 texColor = texture(textureSampler, fragTexCoord);

    fragColor = vec4(diffuse, 1.0) * texColor;
}
