Texture2D shaderTexture : register(t0);
SamplerState diffuseSampler  : register(s0);

//Fog buffer
cbuffer FogBuffer : register(b0)
{
	bool fogDisable;
	float3 padding1;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float fogFactor : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
	float fogCol = 0.9f;
    
    //Fog colour
    float4 fogColour = float4(fogCol, fogCol, fogCol, 1.0f);

	//Snow colour
	float4 colour = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //Final fog amount calculation
    float4 finalFog = input.fogFactor + (1.0 - input.fogFactor) * fogColour;
    
    //If fog is disabled return regular lit colour, if not, apply fog
    if (fogDisable)
    {
    	return colour;
    }
    else
    {
    	//Combine lit colour with fog
    	return lerp(colour, fogColour, finalFog);
    }
}