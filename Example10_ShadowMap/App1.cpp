#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	//Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Initialise mesh objects
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	//Initialise ortho mesh objects
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);
	orthoMesh1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 2.7, screenHeight / 2.7);
	orthoMesh2 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, -screenHeight / 2.7);
	orthoMesh3 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 2.7, -screenHeight / 2.7);
	softShadowOrtho = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);

	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");

	//Load textures
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("heightMap", L"res/heightMap.png");
	textureMgr->loadTexture("rock", L"res/rock.png");

	//Initialise shaders
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	softShadowShader = new SoftShadowShader(renderer->getDevice(), hwnd);
	rippleShader = new RippleShader(renderer->getDevice(), hwnd);
	horizontalBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	verticalBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	//Shadow map values
	int shadowmapWidth = 4096;
	int shadowmapHeight = 4096;
	int sceneWidth = 200;
	int sceneHeight = 200;

	//Initialise shadow maps
	shadowMap = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMap1 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMap2 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMap3 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);

	lightsInit();
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}

void App1::lightsInit()
{
	int sceneWidth = 200;
	int sceneHeight = 200;

	//Initialise lights

	light = new Light;
	light->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	light1 = new Light;
	light1->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	light2 = new Light;
	light2->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	light3 = new Light;
	light3->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	lightDir.x = lightXDir;
	lightDir.y = lightYDir;
	lightDir.z = lightZDir;
	lightDir.w = 0.0f;

	light1Dir.x = light1XDir;
	light1Dir.y = light1YDir;
	light1Dir.z = light1ZDir;
	light1Dir.w = 0.0f;

	light2Dir.x = light2XDir;
	light2Dir.y = light2YDir;
	light2Dir.z = light2ZDir;
	light2Dir.w = 0.0f;

	light3Dir.x = light3XDir;
	light3Dir.y = light3YDir;
	light3Dir.z = light3ZDir;
	light3Dir.w = 0.0f;
}

void App1::guiEdits()
{
	//Update lights positions and directions
	light->setPosition(lightXPos, lightYPos, lightZPos);
	light->setDirection(lightXDir, lightYDir, lightZDir);

	light1->setPosition(light1XPos, light1YPos, light1ZPos);
	light1->setDirection(light1XDir, light1YDir, light1ZDir);

	light2->setPosition(light2XPos, light2YPos, light2ZPos);
	light2->setDirection(light2XDir, light2YDir, light2ZDir);

	light3->setPosition(light3XPos, light3YPos, light3ZPos);
	light3->setDirection(light3XDir, light3YDir, light3ZDir);

	if (lightswitch)
	{
		light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
		light->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
		light->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (light1switch)
	{
		light1->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
		light1->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		light1->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
		light1->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (light2switch)
	{
		light2->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
		light2->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		light2->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
		light2->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (light3switch)
	{
		light3->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
		light3->setDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		light3->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
		light3->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	//Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	time += timer->getTime();

	//Update GUI editable values
	guiEdits();
	// Perform depth pass
	depthPass();
	//Perform second depth pass
	depthPass1();
	//Perform third depth pass
	depthPass2();
	//Perform fourth depth pass
	depthPass3();
	// Render scene
	finalPass();

	return true;
}

void App1::depthPass()
{
	//Set the render target to be the render to texture.
	shadowMap->setRenderTarget(renderer->getDeviceContext());
	shadowMap->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	XMMATRIX lightViewMatrix = light->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	//Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//TEAPOT

	teapotDepthPass(lightViewMatrix, lightProjectionMatrix);

	//CUBE

	cubeDepthPass(lightViewMatrix, lightProjectionMatrix);

	//SPHERE

	sphereDepthPass(lightViewMatrix, lightProjectionMatrix);

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPass1()
{
	// Set the render target to be the render to texture.
	shadowMap1->setRenderTarget(renderer->getDeviceContext());
	shadowMap1->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// get the world, view, and projection matrices from the camera and d3d objects.
	light1->generateViewMatrix();
	XMMATRIX lightViewMatrix1 = light1->getViewMatrix();
	XMMATRIX lightProjectionMatrix1 = light1->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//TEAPOT

	teapotDepthPass(lightViewMatrix1, lightProjectionMatrix1);

	//CUBE

	cubeDepthPass(lightViewMatrix1, lightProjectionMatrix1);

	//SPHERE

	sphereDepthPass(lightViewMatrix1, lightProjectionMatrix1);

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPass2()
{
	//Set the render target to be the render to texture.
	shadowMap2->setRenderTarget(renderer->getDeviceContext());
	shadowMap2->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Get the world, view, and projection matrices from the camera and d3d objects.
	light2->generateViewMatrix();
	XMMATRIX lightViewMatrix2 = light2->getViewMatrix();
	XMMATRIX lightProjectionMatrix2 = light2->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	//Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix2, lightProjectionMatrix2);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//TEAPOT

	teapotDepthPass(lightViewMatrix2, lightProjectionMatrix2);

	//CUBE

	cubeDepthPass(lightViewMatrix2, lightProjectionMatrix2);

	//SPHERE

	sphereDepthPass(lightViewMatrix2, lightProjectionMatrix2);

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPass3()
{
	//Set the render target to be the render to texture.
	shadowMap3->setRenderTarget(renderer->getDeviceContext());
	shadowMap3->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Get the world, view, and projection matrices from the camera and d3d objects.
	light3->generateViewMatrix();
	XMMATRIX lightViewMatrix3 = light3->getViewMatrix();
	XMMATRIX lightProjectionMatrix3 = light3->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	//Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix3, lightProjectionMatrix3);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//TEAPOT

	teapotDepthPass(lightViewMatrix3, lightProjectionMatrix3);

	//CUBE

	cubeDepthPass(lightViewMatrix3, lightProjectionMatrix3);

	//SPHERE

	sphereDepthPass(lightViewMatrix3, lightProjectionMatrix3);

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.0f, 0.0f, 0.0f, 0.0f);
	camera->update();

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	rippleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), shadowMap2->getShaderResourceView(), shadowMap3->getShaderResourceView(), light, light1, light2, light3, lightDir, light1Dir, light2Dir, light3Dir, time, height, frequency, speed);
	rippleShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//TEAPOT

	teapotFinalPass(viewMatrix, projectionMatrix);

	//CUBE

	cubeFinalPass(viewMatrix, projectionMatrix);

	//SPHERE

	sphereFinalPass(viewMatrix, projectionMatrix);

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);

	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	if (drawShadowMap)
	{
		orthoMesh->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	}

	if (drawShadowMap1)
	{
		orthoMesh1->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap1->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthoMesh1->getIndexCount());
	}

	if (drawShadowMap2)
	{
		orthoMesh2->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap2->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthoMesh2->getIndexCount());
	}

	if (drawShadowMap3)
	{
		orthoMesh3->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap3->getShaderResourceView());
		textureShader->render(renderer->getDeviceContext(), orthoMesh3->getIndexCount());
	}

	renderer->setZBuffer(true);

	gui();

	renderer->endScene();
}

void App1::teapotDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//TEAPOT

	worldMatrix = renderer->getWorldMatrix();

	//Translate teapot
	worldMatrix = XMMatrixTranslation(modelXPos, modelYPos, modelZPos);

	//Rotate teapot
	//XMMATRIX rotateMatrix = XMMatrixRotationY(rotation);
	//worldMatrix = XMMatrixMultiply(rotateMatrix, worldMatrix);

	//Scale teapot
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());
}

void App1::cubeDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//TEAPOT

	worldMatrix = renderer->getWorldMatrix();

	//Translate teapot
	worldMatrix = XMMatrixTranslation(cubeXPos, cubeYPos, cubeZPos);

	//Rotate teapot
	//XMMATRIX rotateMatrix = XMMatrixRotationY(rotation);
	//worldMatrix = XMMatrixMultiply(rotateMatrix, worldMatrix);

	//Scale teapot
	XMMATRIX scaleMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	// Render model
	cubeMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
}

void App1::sphereDepthPass(XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//TEAPOT

	worldMatrix = renderer->getWorldMatrix();

	//Translate teapot
	worldMatrix = XMMatrixTranslation(sphereXPos, sphereYPos, sphereZPos);

	//Rotate teapot
	//XMMATRIX rotateMatrix = XMMatrixRotationY(rotation);
	//worldMatrix = XMMatrixMultiply(rotateMatrix, worldMatrix);

	//Scale teapot
	XMMATRIX scaleMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	// Render model
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
}

void App1::teapotFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//TEAPOT

	worldMatrix = renderer->getWorldMatrix();

	//Translate teapot
	worldMatrix = XMMatrixTranslation(modelXPos, modelYPos, modelZPos);

	//Rotate teapot
	//XMMATRIX rotateMatrix = XMMatrixRotationY(rotation);
	//worldMatrix = XMMatrixMultiply(rotateMatrix, worldMatrix);

	//Scale teapot
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	model->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), shadowMap2->getShaderResourceView(), shadowMap3->getShaderResourceView(), light, light1, light2, light3, lightDir, light1Dir, light2Dir, light3Dir);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());
}

void App1::cubeFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//CUBE

	worldMatrix = renderer->getWorldMatrix();

	//Translate cube
	worldMatrix = XMMatrixTranslation(cubeXPos, cubeYPos, cubeZPos);

	//Rotate cube
	//XMMATRIX rotateMatrix1 = XMMatrixRotationY(rotation1);
	//worldMatrix = XMMatrixMultiply(rotateMatrix1, worldMatrix);

	//Scale cube
	XMMATRIX scaleMatrix1 = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix1);

	//Render cube
	cubeMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), shadowMap2->getShaderResourceView(), shadowMap3->getShaderResourceView(), light, light1, light2, light3, lightDir, light1Dir, light2Dir, light3Dir);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
}

void App1::sphereFinalPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//SPHERE

	worldMatrix = renderer->getWorldMatrix();

	//Translate Sphere
	worldMatrix = XMMatrixTranslation(sphereXPos, sphereYPos, sphereZPos);

	////RotateSphere
	//XMMATRIX rotateMatrix2 = XMMatrixRotationY(rotation2);
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix2);

	//Scale Sphere
	XMMATRIX scaleMatrix2 = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix2);

	//Render sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("rock"), textureMgr->getTexture("heightMap"), shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), shadowMap2->getShaderResourceView(), shadowMap3->getShaderResourceView(), light, light1, light2, light3, lightDir, light1Dir, light2Dir, light3Dir);
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	//Cube Stuff

	if (ImGui::CollapsingHeader("Cube Values"))
	{
		ImGui::SliderFloat("Cube X Position: ", &cubeXPos, -9, 9, 0, 1);
		ImGui::SliderFloat("Cube Y Position: ", &cubeYPos, 1, 5, 0, 1);
		ImGui::SliderFloat("Cube Z Position: ", &cubeZPos, -1, 16, 0, 1);
	}

	//Model Stuff

	if (ImGui::CollapsingHeader("Model Values"))
	{
		ImGui::SliderFloat("Model X Position: ", &modelXPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Model Y Position: ", &modelYPos, 7, 20, 0, 1);
		ImGui::SliderFloat("Model Z Position: ", &modelZPos, -5, 80, 0, 1);
	}

	//Light 1 Stuff

	if (ImGui::CollapsingHeader("Light 1 Values"))
	{
		ImGui::SliderFloat("Light 1 X Position: ", &lightXPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Light 1 Y Position: ", &lightYPos, lightHeightDownLimit, lightHeightUpLimit, 0, 1);
		ImGui::SliderFloat("Light 1 Z Position: ", &lightZPos, -80, 80, 0, 1);

		ImGui::SliderFloat("Light 1 X Direction: ", &lightXDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 1 Y Direction: ", &lightYDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 1 Z Direction: ", &lightZDir, -1, 1, 0, 1);
	}

	//Light 2 Stuff

	if (ImGui::CollapsingHeader("Light 2 Values"))
	{

		ImGui::SliderFloat("Light 2 X Position: ", &light1XPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Light 2 Y Position: ", &light1YPos, lightHeightDownLimit, lightHeightUpLimit, 0, 1);
		ImGui::SliderFloat("Light 2 Z Position: ", &light1ZPos, -80, 80, 0, 1);

		ImGui::SliderFloat("Light 2 X Direction: ", &light1XDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 2 Y Direction: ", &light1YDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 2 Z Direction: ", &light1ZDir, -1, 1, 0, 1);
	}

	//Light 3 Stuff

	if (ImGui::CollapsingHeader("Light 3 Values"))
	{

		ImGui::SliderFloat("Light 3 X Position: ", &light2XPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Light 3 Y Position: ", &light2YPos, lightHeightDownLimit, lightHeightUpLimit, 0, 1);
		ImGui::SliderFloat("Light 3 Z Position: ", &light2ZPos, -80, 80, 0, 1);

		ImGui::SliderFloat("Light 3 X Direction: ", &light2XDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 3 Y Direction: ", &light2YDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 3 Z Direction: ", &light2ZDir, -1, 1, 0, 1);
	}

	//Light 4 Stuff

	if (ImGui::CollapsingHeader("Light 4 Values"))
	{

		ImGui::SliderFloat("Light 4 X Position: ", &light3XPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Light 4 Y Position: ", &light3YPos, lightHeightDownLimit, lightHeightUpLimit, 0, 1);
		ImGui::SliderFloat("Light 4 Z Position: ", &light3ZPos, -80, 80, 0, 1);

		ImGui::SliderFloat("Light 4 X Direction: ", &light3XDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 4 Y Direction: ", &light3YDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 4 Z Direction: ", &light3ZDir, -1, 1, 0, 1);
	}

	if (ImGui::CollapsingHeader("Light Switches"))
	{
		ImGui::Checkbox("Light 1 Switch: ", &lightswitch);
		ImGui::Checkbox("Light 2 Switch: ", &light1switch);
		ImGui::Checkbox("Light 3 Switch: ", &light2switch);
		ImGui::Checkbox("Light 4 Switch: ", &light3switch);
	}

	//Shadow Maps

	if (ImGui::CollapsingHeader("Shadow Maps"))
	{
		ImGui::Checkbox("Render First Shadow Map", &drawShadowMap);
		ImGui::Checkbox("Render Second Shadow Map", &drawShadowMap1);
		ImGui::Checkbox("Render Third Shadow Map", &drawShadowMap2);
		ImGui::Checkbox("Render Fourth Shadow Map", &drawShadowMap3);
	}

	if (ImGui::CollapsingHeader("Plane Ripple Values"))
	{
		ImGui::SliderFloat("Wave height: ", &height, 0, 1, 0, 1);
		ImGui::SliderFloat("Wave frquency: ", &frequency, 0, 20, 0, 1);
		ImGui::SliderFloat("Wave speed: ", &speed, 0, 5, 0, 1);
	}

	//ImGui::ShowDemoWindow();

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}