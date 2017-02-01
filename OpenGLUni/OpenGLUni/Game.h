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
#include "Common.h"
#include "GameWindow.h"
#include "Shaders.h"

// Classes used in game
class CShader;
class CShaderProgram;

class Game {
private:
    void draw();
    void drawTriangle();
    
    void Update();
    void Render();
    
    CShaderProgram *m_pShaderProgram;
    glm::mat4 *m_pModelMatrix;
    glm::mat4 *m_pViewMatrix;
    glm::mat4 *m_pProjectionMatrix;
    GLuint m_uiVAO;	// A vertex array object (to wrap VBOs)
    
    GLuint shaderProgram;
    
public:
    Game();
    ~Game();
    
    void Execute();
    void addShaders();
    void addShadersFromResources(std::string path);
    
    
private:
    void GameLoop();
    GameWindow gameWindow;
   
};

#endif /* Game_h */
