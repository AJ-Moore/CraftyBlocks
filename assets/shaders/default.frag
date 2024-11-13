#version 460 core

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;
uniform sampler2D roughnessMap;
uniform sampler2D metalnessMap;

uniform vec4 baseColour = vec4(1.0, 1.0, 1.0, 1.0);

uniform bool hasDiffuseMap = false;
uniform bool hasEmissionMap = false;
uniform bool hasNormalMap = false;
uniform bool hasAOMap = false;
uniform bool hasRoughnessMap = false;
uniform bool hasMetalnessMap = false;

uniform bool unlit = false;
uniform bool useTriplanarProjection = false;
uniform bool shadowPass = false;
uniform float emission = 1.0;

uniform bool wireframe = false; 
uniform vec4 wireframeColour = vec4(1.0,0,0,1.0);

uniform float alphaClipThreshold = 0.0001;
uniform bool useAlphaClip = true;

layout (location = 0) out vec4 gColour;
layout (location = 1) out vec4 gPosition; 
layout (location = 2) out vec4 gNormal;
layout (location = 3) out vec4 gEmission;

// Ambient Occlusion, Roughness, Metalness
layout (location = 4) out vec4 gARM;

layout (location = 0) in VertexData {
	vec2 texCoord;
	vec4 vertexColour;
	vec3 vertexNormal;
	vec3 vertexPosition;
	vec3 fragPos;
	mat3 TBN;
	vec3 localNormal;
} vertexIn[];

vec4 SampleTexture(sampler2D sampler, vec2 coords)
{
    if (!useTriplanarProjection)
	{
		return texture(sampler, coords);
	}

	vec3 blending = abs(vertexIn.localNormal);
	blending = normalize(max(blending, 0.00001));
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);

	vec4 xaxis = texture( sampler, vertexIn.vertexPosition.zy);
	vec4 yaxis = texture( sampler, vertexIn.vertexPosition.xz);
	vec4 zaxis = texture( sampler, vertexIn.vertexPosition.xy);

	return xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
}

void main()
{
    if (shadowPass)
    {
        return;
    }

    vec4 colour = baseColour;

    if (hasDiffuseMap)
    {
        vec4 diffuse = SampleTexture(diffuseMap, vertexIn.texCoord);

        // Alpha clipping
		if (useAlphaClip)
		{
			if (tex.a < alphaClipThreshold) 
			{
				discard;
			}
		}

        colour *= diffuse;
    }

    if (!unlit) 
	{
		if (hasNormalMap) 
		{
			vec3 normal = SampleTexture(normalMap, coordsToMap).rgb;
			gNormal = vec4(normal * 2.0 - 1.0, 1);
            gNormal = vec4(normalize(vertexIn.TBN * gNormal.rgb), 1);
		}
		else
		{
			gNormal = vec4(normalize(vertexIn.vertexNormal), 1.0);
		}
	}

    if (hasEmissionMap) 
	{
        gEmission = colour * SampleTexture(emissionMap, coordsToMap) * emission;
	}
	else if (unlit) 
	{
		gEmission = vec4(objectColour.rgb * emission, objectColour.a);
	}
	else 
	{
		gEmission = vec4(0,0,0,0);
	}

    if (hasRoughnessMap)
    {
        float roughness = SampleTexture(roughnessMap, coordsToMap).r;
        gARM.g = roughness;
    }
    else
    {
        gARM.g = 0.5;
    }

    if (hasMetalnessMap)
    {
        float metalness = SampleTexture(metalnessMap, coordsToMap).r;
        gRoughness.g = r;
    }
    else
    {
        gRoughness.g = 0;
    }

    if (!unlit)
	{	
		gAlbedoSpec = objectColour;

		if (gAlbedoSpec.a == 0)
		{
			discard;
		}
	}

    if (gAlbedoSpec.a == 0 && gEmission.a == 0)
	{
		discard;
	}

    if (wireframe)
	{
		// Calculate the dot product between the fragment normal and the view direction
		float ndotv = abs(dot(normalize(vertexIn.fragPos), normalize(vertexIn.vertexNormal)));

		// Adjust this threshold to control the width of the wireframe lines
		float edgeThreshold = 0.02;

		// Check if the fragment is on an edge
		if (ndotv < edgeThreshold)
		{
			gAlbedoSpec = wireframeColour;
		}
		else
		{
			// If not on an edge, discard the fragment
			discard;
		}
	}
}