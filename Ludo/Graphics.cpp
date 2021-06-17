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

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

struct JSONLoader::boardPositions* positions;


Graphics::Graphics()
{
    //game = GameLogic(4);
    //dobble d = dobble();

    JSONLoader* jsonLoader = new JSONLoader();
    positions = new struct JSONLoader::boardPositions();
    jsonLoader->loadPositions(positions);

    //for (int i = 0; i < 40; i++)
    //    positions.playPositions[i] = glm::vec3(-8 + (0.5f * i), 0, 0);

    //for (int i = 0; i < 4; i++)
    //    positions.blueStartPositions[i] = glm::vec3(-4, 0, -5 + (i * 0.5f));
    //for (int i = 0; i < 4; i++)
    //    positions.redStartPositions[i] = glm::vec3(-2, 0, -5 + (i * 0.5f));
    //for (int i = 0; i < 4; i++)
    //    positions.yellowStartPositions[i] = glm::vec3(0, 0, -5 + (i * 0.5f));
    //for (int i = 0; i < 4; i++)
    //    positions.greenStartPositions[i] = glm::vec3(2, 0, -5 + (i * 0.5f));
}

Graphics::~Graphics()
{
}

void Graphics::mainLoop() {
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

    return;
}

void Graphics::init()
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
    board->position = glm::vec3(7.28f, 0, 5.0f);
    board->scale = glm::vec3(5);
    drawables.push_back(board);

    ObjModel* pawnModel = new ObjModel("Resource/pawn/pawn.obj");

    for (int i = 0; i < 16; i++) {
        glm::vec4 color;
        glm::vec3 pos = glm::vec3(0, 0, 0);
        if (i < 4) {
            color = glm::vec4(0, 0, 1, 1);
            pos = positions->blueStartPositions[i];
        }
        else if (i < 8) {
            color = glm::vec4(1, 0, 0, 1);
            pos = positions->redStartPositions[i - 4];
        }
        else if (i < 12) {
            color = glm::vec4(1, 1, 0, 1);
            pos = positions->yellowStartPositions[i - 8];
        }
        else {
            color = glm::vec4(0, 1, 0, 1);
            pos = positions->greenStartPositions[i - 12];
        }

        Pawn* p = new Pawn(pawnModel, color, pos);
        drawables.push_back(p);
        pawns.push_back(p);
    }

    Pawn* p = new Pawn(pawnModel, glm::vec4(1, 1, 1, 1), glm::vec3(0, 0, 0));
    drawables.push_back(p);

    // Init all drawables
    for (auto& d : drawables) {
        d->init();
    }
}

void Graphics::update()
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

void Graphics::draw()
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

void Graphics::moveFromBase(state color, int pos, bool attacking, state enemyColor)
{
    int pawnIndex = 0;
    switch (color) {
    case state::blue:
        pawnIndex = 0;
        break;
    case state::red:
        pawnIndex = 4;
        break;
    case state::yellow:
        pawnIndex = 8;
        break;
    case state::green:
        pawnIndex = 12;
        break;
    }
    int limit = pawnIndex + 4;
    for (pawnIndex; pawnIndex < limit; pawnIndex++) {
        if (pawns.at(pawnIndex)->atBase) {
            pawns.at(pawnIndex)->atBase = false;

            if (attacking) {
                pawns.at(pawnIndex)->attack(positions->playPositions[pos]);
                returnToBase(pos, enemyColor);
            }
            else {
                pawns.at(pawnIndex)->moveTo(positions->playPositions[pos]);
            }

            break;
        }
    }
}

void Graphics::movePawn(int pawn, glm::vec3 target)
{
    Pawn* p = (Pawn*)pawns.at(pawn);
    p->moveTo(target);
}

void Graphics::movePawn(int pawnPos, int targetPos)
{
    for (auto& p : pawns) {
        if (p->position == positions->playPositions[pawnPos]) {
            p->moveTo(positions->playPositions[targetPos]);
        }
    }
}

void Graphics::attackPawn(int pawn, glm::vec3 target)
{
    Pawn* p = (Pawn*)pawns.at(pawn);
    p->attack(target);
}

void Graphics::attackPawn(int pawnPos, int targetPos, state enemyColor)
{
    for (auto& p : pawns) {
        if (p->position == positions->playPositions[pawnPos]) {
            p->attack(positions->playPositions[targetPos]);
            returnToBase(targetPos, enemyColor);
        }
    }
}

bool Graphics::isMoving(int pawn)
{
    for (auto& p : pawns) {
        if (p->position == positions->playPositions[pawn]) {
            return !p->reachedTarget();
        }
    }
}

bool Graphics::isAttacking(int pawn)
{
    Pawn* p = (Pawn*)pawns.at(pawn);
    return !p->hasAttacked();
}

void Graphics::returnToBase(int pawnPos, state color)
{
    bool foundPawn = false;
    for (auto& p : pawns) {
        if (p->position == positions->playPositions[pawnPos]) {
            switch (color) {
            case state::blue:
                for (int i = 0; i < 4; i++) {
                    for (int pw = 0; pw < 4; pw++) {
                        if (pawns.at(pw)->position == positions->blueStartPositions[i]) {
                            foundPawn = true;
                            break;
                        }
                    }
                    if (foundPawn) {
                        foundPawn = false;
                        continue;
                    }
                    else {
                        p->atBase = true;
                        p->returnToBase(positions->blueStartPositions[i]);
                        return;
                    }
                }
                break;
            case state::red:
                for (int i = 0; i < 4; i++) {
                    for (int pw = 4; pw < 8; pw++) {
                        if (pawns.at(pw)->position == positions->redStartPositions[i]) {
                            foundPawn = true;
                            break;
                        }
                    }
                    if (foundPawn) {
                        foundPawn = false;
                        continue;
                    }
                    else {
                        p->atBase = true;
                        p->returnToBase(positions->redStartPositions[i]);
                        return;
                    }
                }
                break;
            case state::yellow:
                for (int i = 0; i < 4; i++) {
                    for (int pw = 8; pw < 12; pw++) {
                        if (pawns.at(pw)->position == positions->yellowStartPositions[i]) {
                            foundPawn = true;
                            break;
                        }
                    }
                    if (foundPawn) {
                        foundPawn = false;
                        continue;
                    }
                    else {
                        p->atBase = true;
                        p->returnToBase(positions->yellowStartPositions[i]);
                        return;
                    }
                }
                break;
            case state::green:
                for (int i = 0; i < 4; i++) {
                    for (int pw = 12; pw < 16; pw++) {
                        if (pawns.at(pw)->position == positions->greenStartPositions[i]) {
                            foundPawn = true;
                            break;
                        }
                    }
                    if (foundPawn) {
                        foundPawn = false;
                        continue;
                    }
                    else {
                        p->atBase = true;
                        p->returnToBase(positions->greenStartPositions[i]);
                        return;
                    }
                }
                break;
            }
        }
    }
}

void Graphics::finishPawn(state color, int finishPos, int pawnPos)
{
    for (auto& p : pawns) {
        if (p->position == positions->playPositions[pawnPos]) {
            switch (color) {
            case state::blue:
                p->moveTo(positions->blueEndPositions[finishPos]);
                break;
            case state::red:
                p->moveTo(positions->redEndPositions[finishPos]);
                break;
            case state::yellow:
                p->moveTo(positions->yellowEndPositions[finishPos]);
                break;
            case state::green:
                p->moveTo(positions->greenEndPositions[finishPos]);
                break;
            }
            break;
        }
    }
}
