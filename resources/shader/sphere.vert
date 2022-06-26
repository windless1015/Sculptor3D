#version 450 core
layout (location = 0) in vec3 vPos;
uniform mat4 mvp;
out vec3 thePos;
void main() {
gl_Position = mvp * vec4(vPos, 1.0f);
thePos = vPos;
}