// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);
	orthoMesh1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 2.7, screenHeight / 2.7);

	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");

	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("heightMap", L"res/heightMap.png");
	textureMgr->loadTexture("rock", L"res/rock.png");

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	int shadowmapWidth = 2048;
	int shadowmapHeight = 2048;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMap1 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);

	light = new Light;
	light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	light1 = new Light;
	light1->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light1->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light1->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	lightDir.x = lightXDir;
	lightDir.y = lightYDir;
	lightDir.z = lightZDir;
	lightDir.w = 0.0f;

	light1Dir.x = light1XDir;
	light1Dir.y = light1YDir;
	light1Dir.z = light1ZDir;
	light1Dir.w = 0.0f;
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}

void App1::guiEdits()
{
	//rotation += 0.1;
    //rotation1 += 0.02;
    //rotation2 += 0.02;

	light->setPosition(lightXPos, lightYPos, lightZPos);
	light->setDirection(lightXDir, lightYDir, lightZDir);

	light1->setPosition(light1XPos, light1YPos, light1ZPos);
	light1->setDirection(light1XDir, light1YDir, light1ZDir);
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	//Update GUI editable values
	guiEdits();
	// Perform depth pass
	depthPass();
	//Perform second depth pass
	depthPass1();
	// Render scene
	finalPass();

	return true;
}

void App1::depthPass()
{
	// Set the render target to be the render to texture.
	shadowMap->setRenderTarget(renderer->getDeviceContext());
	shadowMap->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	XMMATRIX lightViewMatrix = light->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//FLOOR

	//Translate floor
	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

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
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	//SPHERE

	worldMatrix = renderer->getWorldMatrix();

	//Translate Sphere
	worldMatrix = XMMatrixTranslation(3.0f, 1.0f, 10.f);

	////RotateSphere
	//XMMATRIX rotateMatrix2 = XMMatrixRotationY(rotation2);
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix2);

	//Scale Sphere
	XMMATRIX scaleMatrix2 = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix2);

	//Render Sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

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
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

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
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	//SPHERE

	worldMatrix = renderer->getWorldMatrix();

	//Translate Sphere
	worldMatrix = XMMatrixTranslation(3.0f, 1.f, 10.f);

	////RotateSphere
	//XMMATRIX rotateMatrix2 = XMMatrixRotationY(rotation2);
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix2);

	//Scale Sphere
	XMMATRIX scaleMatrix2 = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix2);

	//Render Sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
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
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), light, light1, lightDir, light1Dir);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

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
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), light, light1, lightDir, light1Dir);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

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
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), NULL, shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), light, light1, lightDir, light1Dir);
	shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	//SPHERE

	worldMatrix = renderer->getWorldMatrix();

	//Translate Sphere
	worldMatrix = XMMatrixTranslation(3.0f, 1.0f, 10.f);

	////RotateSphere
	//XMMATRIX rotateMatrix2 = XMMatrixRotationY(rotation2);
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix2);

	//Scale Sphere
	XMMATRIX scaleMatrix2 = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix2);

	//Render sphere
	sphereMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("rock"), textureMgr->getTexture("heightMap"), shadowMap->getShaderResourceView(), shadowMap1->getShaderResourceView(), light, light1, lightDir, light1Dir);
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);

	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	orthoMesh1->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap1->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh1->getIndexCount());

	renderer->setZBuffer(true);

	gui();

	renderer->endScene();
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
		ImGui::SliderFloat("Cube X Position: ", &cubeXPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Cube Y Position: ", &cubeYPos, 1, 20, 0, 1);
		ImGui::SliderFloat("Cube Z Position: ", &cubeZPos, -5, 80, 0, 1);
	}

	//Model Stuff

	if (ImGui::CollapsingHeader("Model Values"))
	{
		ImGui::SliderFloat("Model X Position: ", &modelXPos, -20, 20, 0, 1);
		ImGui::SliderFloat("Model Y Position: ", &modelYPos, 7, 20, 0, 1);
		ImGui::SliderFloat("Model Z Position: ", &modelZPos, -5, 80, 0, 1);
	}

	//Light Stuff

	if (ImGui::CollapsingHeader("Light Values"))
	{
		ImGui::SliderFloat("Light X Position: ", &lightXPos, -40, 40, 0, 1);
		ImGui::SliderFloat("Light Y Position: ", &lightYPos, 0, 20, 0, 1);
		ImGui::SliderFloat("Light Z Position: ", &lightZPos, 0, 80, 0, 1);

		ImGui::SliderFloat("Light X Direction: ", &lightXDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light Y Direction: ", &lightYDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light Z Direction: ", &lightZDir, -1, 1, 0, 1);
	}

	//Light 1 Stuff

	if (ImGui::CollapsingHeader("Light 1 Values"))
	{

		ImGui::SliderFloat("Light 1 X Position: ", &light1XPos, -40, 40, 0, 1);
		ImGui::SliderFloat("Light 1 Y Position: ", &light1YPos, 0, 20, 0, 1);
		ImGui::SliderFloat("Light 1 Z Position: ", &light1ZPos, 0, 80, 0, 1);

		ImGui::SliderFloat("Light 1 X Direction: ", &light1XDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 1 Y Direction: ", &light1YDir, -1, 1, 0, 1);
		ImGui::SliderFloat("Light 1 Z Direction: ", &light1ZDir, -1, 1, 0, 1);
	}

	//ImGui::ShowDemoWindow();

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

