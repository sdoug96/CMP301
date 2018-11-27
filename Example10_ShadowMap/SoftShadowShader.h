#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class SoftShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[4];
		XMFLOAT4 diffuse[4];
		XMFLOAT4 direction[4];
	};

public:

	SoftShadowShader(ID3D11Device* device, HWND hwnd);
	~SoftShadowShader();

	void setShaderParameters
	(
		ID3D11DeviceContext* deviceContext,
		const XMMATRIX &world,
		const XMMATRIX &view,
		const XMMATRIX &projection,
		ID3D11ShaderResourceView* texture,
		Light* light,
		Light* light1,
		Light* light2,
		Light* light3,
		XMFLOAT4 lightDir,
		XMFLOAT4 light1Dir,
		XMFLOAT4 light2Dir,
		XMFLOAT4 light3Dir
	);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
};