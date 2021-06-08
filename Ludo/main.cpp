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
#include "iostream"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();

int main(void)
{
    GameLogic game = GameLogic(4);
    //dobble d = dobble();

    JSONLoader* jsonLoader = new JSONLoader();
    struct JSONLoader::boardPositions p;
    struct JSONLoader::boardPositions* positions = &p;
    jsonLoader->loadPositions(positions);

    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


    return 0;
}

std::list<Drawable*> drawables;
double timeLastFrame = 0;
Camera* camera;
Pawn* p;
Pawn* p1;

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

    p = new Pawn(pawnModel, glm::vec4(0, 0, 1, 1), glm::vec3(-1, 0, 0));
    drawables.push_back(p);

    // Init all drawables
    for (auto& d : drawables) {
        d->init();
    }

    p1 = new Pawn(pawnModel, glm::vec4(1, 0, 0, 1), glm::vec3(2, 0, 0));
    drawables.push_back(p1);

    p->attack(p1->position);
}

bool done = false;
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
    
    if (p->hasAttacked() && !done) {
        done = true;
        p1->returnToBase(glm::vec3(-4, 0, -4));
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