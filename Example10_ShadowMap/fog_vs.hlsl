cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer FogBuffer : register(b1)
{
	float fogStart;
	float fogEnd;
	float2 padding;
}

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};

OutputType main(InputType input)
{
	OutputType output;
	float4 cameraPosition;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	cameraPosition = mul(input.position, worldMatrix);
	cameraPosition = mul(cameraPosition, viewMatrix);

	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}