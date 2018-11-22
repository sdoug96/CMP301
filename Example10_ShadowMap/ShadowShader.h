// Light shader.h
// Basic single light shader setup
#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "DXF.h"

using namespace std;
using namespace DirectX;


class ShadowShader : public BaseShader
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

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void setShaderParameters
	(
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
		XMFLOAT4 light3Dir
	);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
};

#endif