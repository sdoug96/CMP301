#include "PointList.h"

// ColourTriangle.cpp
// Simple triangle mesh for example purposes. With colour, no texture coordinates or normals.

// Initialise buffers
PointList::PointList(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
PointList::~PointList()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Build shape and fill buffers.
void PointList::initBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	float particleDevX = 150.0f;
	float particleDevY = 50.0f;
	float particleDevZ = 100.0f;

	float maxParticles = 10000.0f;

	vertexCount = maxParticles;
	indexCount = maxParticles;

	VertexType_Colour* vertices = new VertexType_Colour[maxParticles];
	unsigned long* indices = new unsigned long[maxParticles];

	for (int i = 0; i < maxParticles; i++)
	{
		vertices[i].position.x = (((float)rand() - (float)rand()) / RAND_MAX) * particleDevX;
		vertices[i].position.y = (((float)rand() - (float)rand()) / RAND_MAX) * particleDevY;
		vertices[i].position.z = (((float)rand() - (float)rand()) / RAND_MAX) * particleDevZ;
		vertices[i].colour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	for (int i = 0; i < maxParticles; i++)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType_Colour) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

// Send Geometry data to the GPU
void PointList::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType_Colour);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}
