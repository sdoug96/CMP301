// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
    float4 fogColour;
	float4 finalColour;

	textureColour = texture0.Sample(Sampler0, input.tex);

	fogColour = float4(0.5f, 0.5f, 0.5f, 1.0f);

	finalColour = input.fogFactor * textureColour + (1.0 - input.fogFactor) * fogColour;

	return finalColour;

}