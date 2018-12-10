// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "RainShader.h"
#include "PointList.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:

	void geometryInit();
	void lightsInit();

	void guiEdits();

	bool render();

	//Depth passes
	void depthPass();
	void depthPass1();
	void depthPass2();
	void depthPass3();

	void finalPass();

	void gui();

	//Model depth passes
	void treeDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void cabinDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);
	void stumpDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix);

	//Model final passes
	void treeFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void cabinFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void stumpFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	//Fog values
	float fogColour = 0.9f;
	float fogStart = 15.0f;
	float fogEnd = 100.0f;

	bool drawShadowMap, drawShadowMap1, drawShadowMap2, drawShadowMap3, lightoff, light1off, light2off, light3off;

private:

	//Positions for models
	float cubeXPos = -71, cubeYPos = -99, cubeZPos = 99;

	float modelXPos = -20, modelYPos = -380, modelZPos = 5;

	float fireXPos = -4, fireYPos = -18, fireZPos = 39;

	//GUI editable light positions and directions

	//Top left light
	float lightXPos = -20.0f, lightYPos = 20.0f, lightZPos = 75.0f;
	float lightXDir = 0.7f, lightYDir = -0.7f, lightZDir = -0.7f;

	//Bottom right light
	float light1XPos = 20.0f, light1YPos = 20.0f, light1ZPos = -5.0f;
	float light1XDir = -0.7f, light1YDir = -0.7f, light1ZDir = 0.7f;

	//Bottom left light
	float light2XPos = -20.0f, light2YPos = 20.0f, light2ZPos = -5.0f;
	float light2XDir = 0.7f, light2YDir = -0.7f, light2ZDir = 0.7f;

	//Top right light
	float light3XPos = 20.0f, light3YPos = 20.0f, light3ZPos = 75.0f;
	float light3XDir = -0.7f, light3YDir = -0.7f, light3ZDir = -0.7f;

	float lightHeightDownLimit = 0;
	float lightHeightUpLimit = 20;

	bool fogDisable, snowDisable;

	//Initial particle positions
	float initParticlePosX = 0.0f;
	float initParticlePosY = 0.0f;

	//Particle speeds
	float particleVelocityX = 0.05;
	float particleVelocityY = 0.05;

	//Light Directions
	XMFLOAT4 lightDir, light1Dir, light2Dir, light3Dir;

	//Mesh objects
	PlaneMesh* mesh;
	PointList* pointList;

	//Ortho mesh objects
	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh1;
	OrthoMesh* orthoMesh2;
	OrthoMesh* orthoMesh3;

	//Models
	Model* tree;
	Model* cabin;
	Model* stump;

	//Lights
	Light* light;
	Light* light1;
	Light* light2;
	Light* light3;

	//Shaders
	ShadowShader* shadowShader;
	DepthShader* depthShader;
	TextureShader* textureShader;
	RainShader* rainShader;

	//Shadow Maps
	RenderTexture* shadowMap;
	RenderTexture* shadowMap1;
	RenderTexture* shadowMap2;
	RenderTexture* shadowMap3;
};

#endif