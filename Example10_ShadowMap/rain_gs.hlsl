cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//Positions of vertices around point will determine size of each particle
cbuffer PositionBuffer
{
	static float3 g_positions[4] =
	{
	//Quad 1
	//Top left
	float3(-0.12, 0.12, -0.12),
	//Bottom left
	float3(-0.12, -0.12, -0.12),
	//Top right
	float3(0.12, 0.12, -0.12),
	//Bottom right
	float3(0.12, -0.12, -0.12),
	};
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
};

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	for (int i = 0; i < 4; i++)
	{
		float4 vposition = float4(g_positions[i], 1.0f) + input[0].position;
		output.position = mul(vposition, worldMatrix);
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);
		output.tex = input[0].tex;
		output.normal = input[0].normal;
		triStream.Append(output);
	}
	triStream.RestartStrip();
}