Texture2D heightMap : register(t0);
SamplerState sampler0 : register(s0);

//Matrix buffer
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
	matrix lightViewMatrix1;
	matrix lightProjectionMatrix1;
	matrix lightViewMatrix2;
	matrix lightProjectionMatrix2;
	matrix lightViewMatrix3;
	matrix lightProjectionMatrix3;
};

//Fog buffer
cbuffer FogBuffer : register(b1)
{
	float fogStart;
	float fogEnd;
	float2 padding;
}

//Camera buffer
cbuffer CameraBuffer : register(b2)
{
	float3 camPos;
	float padding1;
}

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
	float4 lightViewPos2 : TEXCOORD3;
	float4 lightViewPos3 : TEXCOORD4;
	float fogFactor : TEXCOORD5;
	float3 viewVector : TEXCOORD6;
};


OutputType main(InputType input)
{
    OutputType output;

	//Offset which determines height generated on height map
	float4 offset = heightMap.SampleLevel(sampler0, input.tex, 0);

	//offset position based on calculated height map offset value
	input.position.y += offset.y * 10;

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

	// Calculate the position of the vertice as viewed by the light 2 source.
	output.lightViewPos2 = mul(input.position, worldMatrix);
	output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrix2);
	output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrix2);

	// Calculate the position of the vertice as viewed by the light 3 source.
	output.lightViewPos3 = mul(input.position, worldMatrix);
	output.lightViewPos3 = mul(output.lightViewPos3, lightViewMatrix3);
	output.lightViewPos3 = mul(output.lightViewPos3, lightProjectionMatrix3);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	float4 worldPosition = mul(input.position, worldMatrix);
	output.viewVector = camPos.xyz - worldPosition.xyz;
	output.viewVector = normalize(output.viewVector);

    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	//Fog calculation based on how far camera is from the vertex
	output.fogFactor = saturate((length(output.viewVector) - fogStart) / (fogEnd - fogStart));

	return output;
}