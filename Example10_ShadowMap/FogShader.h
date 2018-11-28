#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class FogShader : public BaseShader
{
public:
	FogShader(ID3D11Device* device, HWND hwnd);
	~FogShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd);

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

	};

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		XMFLOAT2 padding;
	};

	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11Buffer * fogBuffer;
	ID3D11SamplerState* sampleState;
};

