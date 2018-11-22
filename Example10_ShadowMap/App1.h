// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:

	void guiEdits();
	bool render();
	void depthPass();
	void depthPass1();
	void finalPass();
	void gui();

	void teapotDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void cubeDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void sphereDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);

private:

	//GUI editbale mesh and model positions
	float cubeXPos = 5, cubeYPos = 1, cubeZPos = 1;

	float modelXPos = -20, modelYPos = 7, modelZPos = 5;

	float sphereXPos = 3, sphereYPos = 1, sphereZPos = 1;

	//GUI editable light positions and directions
	float lightXPos = -40.0f, lightYPos = 20.0f, lightZPos = 40.0f;
	float lightXDir = 0.7f, lightYDir = -0.7f, lightZDir = 0.0f;

	float light1XPos = 40.0f, light1YPos = 20.0f, light1ZPos = 40.0f;
	float light1XDir = -0.7f, light1YDir = -0.7, light1ZDir = 0.0f;

	//Light Directions
	XMFLOAT4 lightDir, light1Dir;

	//Mesh objects
	PlaneMesh* mesh;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh1;

	//Models
	Model* model;

	//Lights
	Light* light;
	Light* light1;
	Light* light2;
	Light* light3;

	//Shaders
	ShadowShader* shadowShader;
	DepthShader* depthShader;
	TextureShader* textureShader;

	//Shadow Maps
	RenderTexture* shadowMap;
	RenderTexture* shadowMap1;
	RenderTexture* shadowMap2;
	RenderTexture* shadowMap3;

	bool drawShadowMap, drawShadowMap1;
};

#endif