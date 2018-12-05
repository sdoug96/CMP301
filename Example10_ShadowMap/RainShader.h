// Light shader.h
// Geometry shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class RainShader : public BaseShader
{

public:

	RainShader(ID3D11Device* device, HWND hwnd);
	~RainShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd, Camera* cam, bool fogDisable);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		XMFLOAT2 padding;
	};

	struct FogBufferType1
	{
		float fogDisable;
		XMFLOAT3 padding;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* fogBuffer;
	ID3D11Buffer* fogBuffer1;
	ID3D11Buffer* cameraBuffer;
	ID3D11SamplerState* sampleState;
};
