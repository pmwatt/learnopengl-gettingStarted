// vertex shader source

#version 330 core
layout (location = 0) in vec3 aPos; // attrib position 1

out vec3 color;

uniform float offset;

void main()
{
	gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0); // output
	color = aPos; // set ourColor to input color from the vertex data
}

// bottom left is black because vertex value for the position is (-0.5, -0.5, 0.0)
// color value ranges from [0.0, 1.0], so it get cut to 0.0 from -0.5