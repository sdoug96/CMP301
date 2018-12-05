// geometry shader.cpp
#include "RainShader.h"


RainShader::RainShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"rain_vs.cso", L"rain_gs.cso", L"rain_ps.cso");
}


RainShader::~RainShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the fog constant buffer.
	if (fogBuffer)
	{
		fogBuffer->Release();
		fogBuffer = 0;
	}

	// Release the second fog constant buffer.
	if (fogBuffer1)
	{
		fogBuffer1->Release();
		fogBuffer1 = 0;
	}

	// Release the camera constant buffer.
	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void RainShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC tessellationBufferDesc;
	D3D11_BUFFER_DESC fogBufferDesc;
	D3D11_BUFFER_DESC fogBufferDesc1;
	D3D11_BUFFER_DESC cameraBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup the description of the dynamic fog constant buffer that is in the vertex shader.
	fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc.ByteWidth = sizeof(FogBufferType);
	fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc.MiscFlags = 0;
	fogBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&fogBufferDesc, NULL, &fogBuffer);

	// Setup the description of the second dynamic fog constant buffer that is in the pixel shader.
	fogBufferDesc1.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc1.ByteWidth = sizeof(FogBufferType1);
	fogBufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc1.MiscFlags = 0;
	fogBufferDesc1.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&fogBufferDesc1, NULL, &fogBuffer1);

	//Setup camera buffer
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

}

void RainShader::initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadGeometryShader(gsFilename);
}


void RainShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, float fogStart, float fogEnd, Camera* cam, bool fogDisable)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	FogBufferType* fogPtr;
	FogBufferType1* fogPtr1;
	CameraBufferType* camPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);

	// Send matrix data
	deviceContext->Map(fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	fogPtr = (FogBufferType*)mappedResource.pData;
	fogPtr->fogStart = fogStart;
	fogPtr->fogEnd = fogEnd;
	fogPtr->padding.x = 0;
	fogPtr->padding.y = 0;

	deviceContext->Unmap(fogBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &fogBuffer);

	// Send matrix data
	deviceContext->Map(fogBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	fogPtr1 = (FogBufferType1*)mappedResource.pData;
	fogPtr1->fogDisable = fogDisable;
	fogPtr1->padding.x = 0;
	fogPtr1->padding.y = 0;
	fogPtr1->padding.z = 0;

	deviceContext->Unmap(fogBuffer1, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &fogBuffer1);

	//Send camera data
	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	camPtr = (CameraBufferType*)mappedResource.pData;
	camPtr->cameraPosition = cam->getPosition();
	camPtr->padding = 0.0f;

	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &cameraBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
}

void RainShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}