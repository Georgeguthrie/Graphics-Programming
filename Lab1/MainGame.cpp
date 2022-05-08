#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Shader reflectShader();
	Shader adsShader();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	
	mesh1.loadModel("..\\res\\Zuccarello.obj");
	mesh2.loadModel("..\\res\\bone.obj");
	mesh3.loadModel("..\\res\\monkey3.obj");
	reflectShader.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	adsShader.init("..\\res\\shaderADS.vert", "..\\res\\shaderADS.frag");
    geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -7), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::linkGeo()
{
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
}

void MainGame::linkReflect()
{
	reflectShader.setMat4("model", transform.GetModel());
	reflectShader.setMat4("view", myCamera.getView());
	reflectShader.setMat4("projection", myCamera.getProjection());
	reflectShader.setInt("skybox", 0);
	reflectShader.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::linkADS()
{
	adsShader.setMat4("projection", myCamera.getProjection());
	adsShader.setMat4("view", myCamera.getView());
	adsShader.setMat4("model", transform.GetModel());
	adsShader.setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
	adsShader.setVec3("objectColour", glm::vec3(0.81f, 0.71f, 0.23f));
	adsShader.setVec3("lightPosition", glm::vec3(-3.25f, 0.0f, 1.0f));
	adsShader.setVec3("cameraPosition", myCamera.getPos());
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture

	//reflection shader
	transform.SetPos(glm::vec3(0.0, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	reflectShader.Bind();
	linkReflect();
	reflectShader.Update(transform, myCamera);
    mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);
	
	//geo shader
	transform.SetPos(glm::vec3(2.0, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.005, 0.005, 0.005));

	geoShader.Bind();
	linkGeo();
	geoShader.Update(transform, myCamera);
	texture.Bind(0);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);

	//ads shader
	transform.SetPos(glm::vec3(-2.0, -sinf(counter), 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	adsShader.Bind();
	linkADS();
	adsShader.Update(transform, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform.GetPos(), 0.62f);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);	
	
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();	
} 