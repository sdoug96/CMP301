Texture2D heightMap : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix1;
	matrix lightProjectionMatrix1;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
	float4 lightViewPos1 : TEXCOORD2;
};


OutputType main(InputType input)
{
    OutputType output;

	float4 offset = heightMap.SampleLevel(sampler0, input.tex, 0);

	//offset position based on sine wave
	input.position.y += offset.y / 3;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPos = mul(input.position, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix);

	// Calculate the position of the vertice as viewed by the light 1 source.
	output.lightViewPos1 = mul(input.position, worldMatrix);
	output.lightViewPos1 = mul(output.lightViewPos1, lightViewMatrix1);
	output.lightViewPos1 = mul(output.lightViewPos1, lightProjectionMatrix1);

    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}