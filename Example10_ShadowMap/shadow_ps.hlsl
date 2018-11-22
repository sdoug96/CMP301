
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture1 : register(t2);
Texture2D depthMapTexture2 : register(t3);
Texture2D depthMapTexture3 : register(t4);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[4];
	float4 diffuse[4];
	float4 direction[4];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
	float4 lightViewPos1 : TEXCOORD2;
	float4 lightViewPos2 : TEXCOORD3;
	float4 lightViewPos3 : TEXCOORD4;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 shadowCalc(float4 lightViewPos, float4 diffuse, float4 direction, Texture2D depthMapTexture, InputType input, float4 textureColour)
{
	float depthValue;
	float lightDepthValue;
	float shadowMapBias = 0.005f;
	float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Calculate the projected texture coordinates.
	float2 pTexCoord = lightViewPos.xy / lightViewPos.w;
	pTexCoord *= float2(0.5, -0.5);
	pTexCoord += float2(0.5f, 0.5f);

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
	{
		return textureColour;
	}

	// Sample the shadow map (get depth of geometry)
	depthValue = depthMapTexture.Sample(shadowSampler, pTexCoord).r;

	// Calculate the depth from the light.
	lightDepthValue = lightViewPos.z / lightViewPos.w;
	lightDepthValue -= shadowMapBias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	if (lightDepthValue < depthValue)
	{
		colour = calculateLighting(-direction, input.normal, diffuse);
	}

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);

	//LIGHT 1

	float4 colour = shadowCalc(input.lightViewPos, diffuse[0], direction[0], depthMapTexture, input, textureColour);

	//LIGHT 2

	float4 colour1 = shadowCalc(input.lightViewPos1, diffuse[1], direction[1], depthMapTexture1, input, textureColour);

	//LIGHT 3

	float4 colour2 = shadowCalc(input.lightViewPos2, diffuse[2], direction[2], depthMapTexture2, input, textureColour);

	//LIGHT 4

	float4 colour3 = shadowCalc(input.lightViewPos3, diffuse[3], direction[3], depthMapTexture3, input, textureColour);
    
	//Add light colours to give proper effect

	colour += colour1;

	colour += ambient[0];

    return saturate(colour * textureColour);
}