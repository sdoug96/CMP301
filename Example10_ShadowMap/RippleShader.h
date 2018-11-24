#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class RippleShader : public BaseShader
{
private:

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX lightView1;
		XMMATRIX lightProjection1;
		XMMATRIX lightView2;
		XMMATRIX lightProjection2;
		XMMATRIX lightView3;
		XMMATRIX lightProjection3;

	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[4];
		XMFLOAT4 diffuse[4];
		XMFLOAT4 direction[4];
	};

	struct TimeBufferType
	{
		float time;
		float height;
		float frequency;
		float speed;
	};

public:
	RippleShader(ID3D11Device* device, HWND hwnd);
	~RippleShader();

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext,
		const XMMATRIX &world,
		const XMMATRIX &view,
		const XMMATRIX &projection,
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* heightMap,
		ID3D11ShaderResourceView*depthMap,
		ID3D11ShaderResourceView*depthMap1,
		ID3D11ShaderResourceView*depthMap2,
		ID3D11ShaderResourceView*depthMap3,
		Light* light,
		Light* light1,
		Light* light2,
		Light* light3,
		XMFLOAT4 lightDir,
		XMFLOAT4 light1Dir,
		XMFLOAT4 light2Dir,
		XMFLOAT4 light3Dir,
		float time,
		float height,
		float frequency,
		float speed
	);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
};