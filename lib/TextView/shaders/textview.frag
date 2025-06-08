#version 440
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D glyphSampler;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 color;

void main()
{
    float d = texture(glyphSampler, uv).r;
    float a = smoothstep(0.5 - 0.1, 0.5 + 0.1, d);
    fragColor = vec4(color.rgb, color.a * a);
}
