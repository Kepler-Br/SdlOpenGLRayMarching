#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec3 position;

void main(void) {
    // Set the position to the one defined in our vertex array
    gl_Position = vec4(position, 1.0f);
}
