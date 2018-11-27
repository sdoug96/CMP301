Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
	float screenHeight;
	float3 padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	float weight0, weight1, weight2, weight3, weight4, weight5, weight6, weight7, weight8, weight9, weight10;
float4 colour;

// Create the weights that each neighbor pixel will contribute to the blur.
weight0 = 0.3;
weight1 = 0.2;
weight2 = 0.1;
weight3 = 0.09;
weight4 = 0.08;
weight5 = 0.06;
weight6 = 0.05;
weight7 = 0.05;
weight8 = 0.04;
weight9 = 0.02;
weight10 = 0.01;

// Initialize the colour to black.
colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

float texelSize = 1.0f / screenHeight;
// Add the vertical pixels to the colour by the specific weight of each.
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -10.0f, 0.0f)) * weight10;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -9.0f, 0.0f)) * weight9;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -8.0f, 0.0f)) * weight8;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -7.0f, 0.0f)) * weight7;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -6.0f, 0.0f)) * weight6;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -5.0f, 0.0f)) * weight5;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -4.0f, 0.0f)) * weight4;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -3.0f, 0.0f)) * weight3;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -2.0f, 0.0f)) * weight2;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -1.0f, 0.0f)) * weight1;
colour += shaderTexture.Sample(SampleType, input.tex) * weight0;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 1.0f, 0.0f)) * weight1;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 2.0f, 0.0f)) * weight2;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 3.0f, 0.0f)) * weight3;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 4.0f, 0.0f)) * weight4;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 5.0f, 0.0f)) * weight5;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 6.0f, 0.0f)) * weight6;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 7.0f, 0.0f)) * weight7;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 8.0f, 0.0f)) * weight8;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 9.0f, 0.0f)) * weight9;
colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 10.0f, 0.0f)) * weight10;

// Set the alpha channel to one.
colour.a = 1.0f;

return colour;
}
