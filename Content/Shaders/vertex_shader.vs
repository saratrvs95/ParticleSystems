#version 450 core

layout (location = 0) in vec4 aDiffuse;
layout (location = 1) in vec4 aRotate;
layout (location = 2) in vec4 aCameraRotation;
layout (location = 3) in vec3 aPosition;
layout (location = 4) in float aSize;

out vec4 outDiffuse;
out vec4 outRotate;
out vec4 outCameraRotation;
out float outSize;

uniform mat4 viewProjection;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main() 
{
	gl_Position = viewProjection * vec4(aPosition, 1.0);
    outDiffuse = aDiffuse;
    outRotate = aRotate;
	outCameraRotation = aCameraRotation;
	outSize = aSize;
}	