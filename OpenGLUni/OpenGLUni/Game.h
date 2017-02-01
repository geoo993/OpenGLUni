//
//  Game.hpp
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#ifndef Game_h
#define Game_h

#pragma once

// Setup includes
#include "Common.h"
#include "GameWindow.h"

// Game includes
#include "Shaders.h"
#include "MatrixStack.h"
#include "Sphere.h"

// Classes used in game
class CShader;
class CShaderProgram;
class GameWindow;
class CSphere;


class Game {
private:
    void GameLoop();
    GameWindow gameWindow;
    
    void drawTriangle(std::string resourcePath);
    void drawSphere(std::string resourcePath);
    
    void Update();
    void Render();
    
    CShaderProgram *m_pShaderProgram;
    CSphere *m_pSphere = nullptr;
    
    glm::mat4 *m_pModelMatrix = nullptr;
    
    GLuint m_uiVAO;	// A vertex array object (to wrap VBOs)
    float m_rotY;
    float m_dt;
 
public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    
    ~Game();
    
    void Execute();
    
    void addShaders();
    void addShadersFromResources(std::string path, glm::mat4 &m_viewMatrix,glm::mat4 &m_projectionMatrix);
};

#endif /* Game_h */
