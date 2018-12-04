#include "BaseMesh.h"

class TessPlaneMesh : public BaseMesh
{

public:
	TessPlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~TessPlaneMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};