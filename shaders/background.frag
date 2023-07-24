#version 330 core
out vec4 color;

uniform vec4 locationAndSize;

void main()
{
	if (((gl_FragCoord.x >= locationAndSize.x) && (gl_FragCoord.x <= locationAndSize.x + 5)) || ((gl_FragCoord.x >= locationAndSize.x + locationAndSize.z - 5) && (gl_FragCoord.x <= locationAndSize.x + locationAndSize.z)))
	{
		color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}
	else
	{
		color = vec4(0.8f, 0.75f, 0.3f, 1.0f);
	}
}
