// vertex shader source

#version 330 core
layout (location = 0) in vec3 aPos; // attrib position 1
layout (location = 1) in vec3 aColor; // attrib position 2

out vec3 ourColor;

void main()
{
	gl_Position = vec4(-aPos.x, -aPos.y, aPos.z, 1.0); // output
	ourColor = aColor; // set ourColor to input color from the vertex data
}
