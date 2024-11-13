layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoordAtrib;
layout (location = 2) in vec4 colourAtrib;
layout (location = 3) in vec3 normalAtrib;

layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 biTangent;
layout (location = 6) in vec4 boneWeights;
layout (location = 7) in vec4 boneIds;

layout (location = 0) out VertexData {
	vec2 texCoord;
	vec4 vertexColour;
	vec3 vertexNormal;
	vec3 vertexPosition;
	vec3 fragPos;
	mat3 TBN;
	vec3 localNormal;
} vertexOut;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 cameraPosition;
uniform bool shadowPass;
uniform float animationTimeElapsed;
uniform int animationCount;

void main() 
{
    vec4 vPosition = vec4(position.rgb, 1.0);
    mat4 wvpMat = projMat * viewMat * modelMat;

    if (shadowPass) 
	{
		gl_Position = wvpMat * vPosition;
		return;
	}

	vec3 lightPos = vec3(0,0,0);

	vertexOut.vertexNormal = normalize(mat3(modelMat) * normalAtrib);
	vertexOut.texCoord = texCoordAtrib;
    vertexOut.vertexColour = colourAtrib
	vertexOut.fragPos = (modelMat * vec4(vPosition.rgb, 1.0)).xyz;
	vertexOut.vertexPosition = vPosition.xyz;
	gl_Position = wvpMat * vPosition;


}