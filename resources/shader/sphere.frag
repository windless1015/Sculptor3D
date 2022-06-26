#version 450 core
in vec3 thePos;
out vec4 fragColor;
void main() {
float red = abs(sqrt(thePos.x * thePos.x + thePos.z * thePos.z));
fragColor = vec4(red, 0.0f, 0.0f, 1.0f);
}