cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float fogFactor : TEXCOORD1;
	float3 viewVector : TEXCOORD2;
};

InputType main(InputType input)
{
	return input;
}