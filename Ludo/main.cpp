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

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();
void gameThread();

GameLogic game;

int main(void)
{
    //JSONLoader* jsonLoader = new JSONLoader();
    //struct JSONLoader::boardPositions p;
    //struct JSONLoader::boardPositions* positions = &p;
    //jsonLoader->loadPositions(positions);

    game = GameLogic(4);
    //dobble d = dobble();

 //   if (!glfwInit())
 //       throw "Could not initialize glwf";
 //   window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
 //   if (!window)
 //   {
 //       glfwTerminate();
 //       throw "Could not initialize glwf";
 //   }
 //   glfwMakeContextCurrent(window);

 //   tigl::init();

 //   init();

 //   std::thread t1 = std::thread(&gameThread);

	//while (!glfwWindowShouldClose(window))
	//{
	//	update();
	//	draw();
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

 //   game.running = false;
 //   t1.join();
	//glfwTerminate();


    return 0;
}

std::list<Drawable*> drawables;
double timeLastFrame = 0;
Camera* camera;

void init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    // Create camera
    camera = new DebugCamera(window);
    camera->moveTo(glm::vec3(0, -8, -7), glm::vec3(45, 0, 0));

    // Create board
    Board* board = new Board(new Texture("Resource/ludo_game_board.jpg"));
    board->scale = glm::vec3(5);
    drawables.push_back(board);

    ObjModel* pawnModel = new ObjModel("Resource/pawn/pawn.obj");
    std::vector<Pawn*> pawns (16);

    for (int i = 0; i < 16; i++) {
        glm::vec4 color;
        glm::vec3 pos = glm::vec3(0, 0, 0);
        if (i < 4) {
            color = glm::vec4(0, 0, 1, 1);
        }
        else if (i < 8) {
            color = glm::vec4(1, 0, 0, 1);
        }
        else if (i < 12) {
            color = glm::vec4(1, 1, 0, 1);
        }
        else {
            color = glm::vec4(0, 1, 0, 1);
        }

        Pawn* p = new Pawn(pawnModel, color, pos);
        drawables.push_back(p);
        pawns.push_back(p);
    }

    

    // Init all drawables
    for (auto& d : drawables) {
        d->init();
    }
}

void gameThread() {
    while (game.running) {
        game.update();
    }
}

void update()
{
    // Calculate time between this frame and last frame
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - timeLastFrame;
    timeLastFrame = currentTime;

    camera->update(window);

    // Update all drawables
    for (auto& d : drawables) {
        d->update(deltaTime);
    }
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getViewMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);
    tigl::shader->enableColorMult(true);

    glEnable(GL_DEPTH_TEST);

    // Draw all drawables
    for (auto& d : drawables) {
        d->draw();
    }
}