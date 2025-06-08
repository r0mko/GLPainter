#version 440
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec2 instPos;
layout(location = 3) in vec2 instSize;
layout(location = 4) in vec2 atlasPos;
layout(location = 5) in vec2 atlasSize;
layout(location = 6) in vec4 instColor;

layout(std140, binding = 0) uniform buf {
    mat4 matrix;
};

layout(location = 0) out vec2 uv;
layout(location = 1) out vec4 color;

void main()
{
    vec2 pos = instPos + inPos * instSize;
    gl_Position = matrix * vec4(pos, 0.0, 1.0);
    uv = atlasPos + inUV * atlasSize;
    color = instColor;
}
