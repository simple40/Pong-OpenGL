#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 modelMatrix;

void main()
{
	gl_Position =  modelMatrix * position;
};

#shader fragment
#version 330 core

uniform vec4 uniformColor;  // Color uniform
uniform float alpha;
out vec4 fragColor;  // Output color passed to the fragment shader

void main()
{
	//fragColor = uniformColor;
	fragColor = vec4(uniformColor.rgb, uniformColor.a * alpha);
};