#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Drawable.h"
#include <list>
#include "Camera.h"
#include "Board.h"
#include "DebugCamera.h"
#include "Texture.h"
#include "Pawn.h"
#include "dobble.h"
#include "GameLogic.h"
#include "JSONLoader.h"
#include <iostream>
#include <thread>
#include "Graphics.h"
#include "Number.h"

class Graphics
{
private:
	GLFWwindow* window;
	std::list<Drawable*> drawables;
	std::vector<Pawn*> pawns;
	std::vector<Number*> numbers;
	double timeLastFrame = 0;
	Camera* camera;

	void drawNumbers_internal(glm::vec3 = glm::vec3(100), glm::vec3 = glm::vec3(100), glm::vec3 = glm::vec3(100), glm::vec3 = glm::vec3(100));

public:
	Graphics();
	~Graphics();

	void mainLoop();
	void init();
	void update();
	void draw();

	void moveFromBase(state, int, bool, state);
	void movePawn(int, int);
	void attackPawn(int, int, state);
	void returnToBase(int, state);
	bool isMoving(int);
	bool isAttacking(int);
	void finishPawn(state, int, int);
	void drawText(std::string, glm::vec3);
	void drawNumbers(state, int = -1, int = -1, int = -1, int = -1);
	void stopDrawingNumbers();
};

