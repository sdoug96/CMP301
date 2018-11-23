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

	void lightsInit();
	void guiEdits();
	bool render();
	void depthPass();
	void depthPass1();
	void depthPass2();
	void depthPass3();
	void finalPass();
	void gui();

	void teapotDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void cubeDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void sphereDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);

	void teapotFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void cubeFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void sphereFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:

	//GUI editbale mesh and model positions
	float cubeXPos = 5, cubeYPos = 1, cubeZPos = 1;

	float modelXPos = -20, modelYPos = 7, modelZPos = 5;

	float sphereXPos = 3, sphereYPos = 1, sphereZPos = 1;

	//GUI editable light positions and directions

	//Top left light
	float lightXPos = -40.0f, lightYPos = 10.0f, lightZPos = 75.0f;
	float lightXDir = 0.7f, lightYDir = -0.7f, lightZDir = -0.7f;

	//Bottom right light
	float light1XPos = 40.0f, light1YPos = 10.0f, light1ZPos = -5.0f;
	float light1XDir = -0.7f, light1YDir = -0.7f, light1ZDir = 0.7f;

	//Bottom left light
	float light2XPos = -40.0f, light2YPos = 10.0f, light2ZPos = -5.0f;
	float light2XDir = 0.7f, light2YDir = -0.7f, light2ZDir = 0.7f;

	//Top right light
	float light3XPos = 40.0f, light3YPos = 10.0f, light3ZPos = 75.0f;
	float light3XDir = -0.7f, light3YDir = -0.7f, light3ZDir = -0.7f;

	//Light Directions
	XMFLOAT4 lightDir, light1Dir, light2Dir, light3Dir;

	//Mesh objects
	PlaneMesh* mesh;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh1;
	OrthoMesh* orthoMesh2;
	OrthoMesh* orthoMesh3;

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

	bool drawShadowMap, drawShadowMap1, drawShadowMap2, drawShadowMap3;
};

#endif