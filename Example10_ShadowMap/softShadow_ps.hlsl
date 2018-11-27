
Texture2D shaderTexture : register(t0);
Texture2D shadowTexture : register(t1);
Texture2D shadowTexture1 : register(t2);
Texture2D shadowTexture2 : register(t3);
Texture2D shadowTexture3 : register(t4);

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

float4 shadowCalc(float4 lightViewPos, float4 diffuse, float4 direction, Texture2D depthMapTexture, InputType input)
{
	float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float shadowValue;
	float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);
	
	calculateLighting(-direction, input.normal, diffuse);

	colour = colour * textureColour;

	// Calculate the projected texture coordinates.
	float2 pTexCoord = lightViewPos.xy / lightViewPos.w;
	pTexCoord *= float2(0.5, -0.5);
	pTexCoord += float2(0.5f, 0.5f);

	// Sample the shadow map (get depth of geometry)
	shadowValue = shadowTexture.Sample(shadowSampler, pTexCoord).r;

	colour = colour * shadowValue;

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	//LIGHT 1

	float4 colour = shadowCalc(input.lightViewPos, diffuse[0], direction[0], shadowTexture, input);

	//LIGHT 2

	float4 colour1 = shadowCalc(input.lightViewPos1, diffuse[1], direction[1], shadowTexture1, input);

	//LIGHT 3

	float4 colour2 = shadowCalc(input.lightViewPos2, diffuse[2], direction[2], shadowTexture2, input);

	//LIGHT 4

	float4 colour3 = shadowCalc(input.lightViewPos3, diffuse[3], direction[3], shadowTexture3, input);

	//Add light colours to give proper effect

	colour = colour + colour1 + colour2 + colour3;

	return colour;
}