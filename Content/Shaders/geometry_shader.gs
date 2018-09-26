#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

layout(location = 0) in vec4 outDiffuse[];
layout(location = 1) in vec4 outRotate[];
layout(location = 2) in vec4 outCameraRotation[];
layout(location = 3) in float outSize[];

out vec2 fragTexCoord;
out vec4 fragDiffuse;

vec4 pos = gl_in[0].gl_Position;

void createVertex(vec3 offset){
	vec4 actualOffset = vec4(offset, 1.0);
	vec4 worldPosition = pos + actualOffset;
	gl_Position = worldPosition;
}


void main(){
	createVertex(vec3(-outSize[0], -outSize[0], 1.0));
	fragTexCoord = vec2(0.0f, 0.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();
	
	createVertex(vec3(outSize[0], -outSize[0], 1.0));
	fragTexCoord = vec2(1.0f, 0.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();
	
	createVertex(vec3(outSize[0], outSize[0], 1.0));
	fragTexCoord = vec2(1.0f, 1.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();
	EndPrimitive();
	
	createVertex(vec3(outSize[0], outSize[0], 1.0));
	fragTexCoord = vec2(1.0f, 1.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();

	createVertex(vec3(-outSize[0], -outSize[0], 1.0));
	fragTexCoord = vec2(0.0f, 0.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();
	
	createVertex(vec3(-outSize[0], outSize[0], 1.0));
	fragTexCoord = vec2(0.0f, 1.0f);
	fragDiffuse = outDiffuse[0];
	EmitVertex();
	EndPrimitive();

}
