//
//  Game.cpp
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//


#include "Game.h"

// Constructor
Game::Game(): m_pShaderProgram(nullptr), m_uiVAO(0), m_rotY(0.0f), m_dt(0.0f)
{
    
}

// Destructor
Game::~Game() 
{ 
    
    //game objects
    delete m_pShaderProgram;
    delete m_pSphere;
     
}

void Game::drawSphere(std::string resourcePath){
    
    m_pShaderProgram = new CShaderProgram;
    m_pModelMatrix = new glm::mat4(1);
    m_pSphere = new CSphere;
    
    // This sets the position, viewpoint, and up vector of the synthetic camera
    glm::vec3 vEye(0, 0, 20);
    glm::vec3 vView(0, 0, 0);
    glm::vec3 vUp(0, 1, 0);
    glm::mat4 mViewMatrix = glm::lookAt(vEye, vView, vUp);
    
    // This creates a view frustum
    glm::mat4 mProjectionMatrix = glm::perspective(45.0f, 1.333f, 1.0f, 150.0f);
  
    // This sets the background colour
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0);
    
    addShadersFromResources(resourcePath, mViewMatrix, mProjectionMatrix);
    
}

void Game::drawTriangle(std::string resourcePath) 
{
    
    m_pShaderProgram = new CShaderProgram;
    m_pModelMatrix = new glm::mat4(1);
    
    GLuint uiVBO[2];	// Two vertex buffer objects
    
    float fTrianglePositions[9];	// An array to store triangle vertex positions
    float fTriangleColor[9];		// An array to store triangle vertex colours
    
    // This sets the position, viewpoint, and up vector of the camera
    glm::vec3 vEye(10, 10, 10);
    glm::vec3 vView(0, 0, 0);
    glm::vec3 vUp(0, 1, 0);
    glm::mat4 mViewMatrix = glm::lookAt(vEye, vView, vUp);
    
    // This creates a view frustum
    glm::mat4 mProjectionMatrix = glm::perspective(45.0f, 1.333f, 1.0f, 150.0f);
  
    
    // This sets the background colour
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.0,0.0,0.0,1);
    
    // Setup triangle vertex positions
    fTrianglePositions[0] = -1.0f; fTrianglePositions[1] = 0.0f; fTrianglePositions[2] = 0.0f;
    fTrianglePositions[3] = 1.0f; fTrianglePositions[4] = 0.0f; fTrianglePositions[5] = 0.0f;
    fTrianglePositions[6] = 0.0f; fTrianglePositions[7] = 1.0f; fTrianglePositions[8] = 0.0f;
    
    // Setup triangle vertex colours
    fTriangleColor[0] = 0.0f; fTriangleColor[1] = 1.0f; fTriangleColor[2] = 0.0f;
    fTriangleColor[3] = 0.0f; fTriangleColor[4] = 0.0f; fTriangleColor[5] = 1.0f;
    fTriangleColor[6] = 1.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 0.0f;
    
    // Generate a VAO and two VBOs
    glGenVertexArrays(1, &m_uiVAO); 
    glGenBuffers(2, &uiVBO[0]); 
    
    // Create the VAO for the triangle
    glBindVertexArray(m_uiVAO);
    
    // Create a VBO for the triangle vertices
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTrianglePositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Create a VBO for the triangle colours
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    addShadersFromResources(resourcePath, mViewMatrix, mProjectionMatrix);
   
}

void Game::addShadersFromResources(std::string path, glm::mat4 &m_viewMatrix,
                                   glm::mat4 &m_projectionMatrix)
{
   
    // Load and compile shaders 
    CShader shVertex, shFragment;	
    
    //full driectory of the shaders
    shVertex.LoadShader(path+"/shaders/shader.vert", GL_VERTEX_SHADER);
    shFragment.LoadShader(path+"/shaders/shader.frag", GL_FRAGMENT_SHADER);
    
    // Create shader program and add shaders
    m_pShaderProgram->CreateProgram();
    m_pShaderProgram->AddShaderToProgram(&shVertex);
    m_pShaderProgram->AddShaderToProgram(&shFragment);
    
    // Link and use the program
    m_pShaderProgram->LinkProgram();
    m_pShaderProgram->UseProgram();
    
    // Set the modeling, viewing, and projection matrices in the shader
    m_pShaderProgram->SetUniform("viewMatrix", m_viewMatrix);
    m_pShaderProgram->SetUniform("projectionMatrix", m_projectionMatrix);
    m_pShaderProgram->SetUniform("vlightDirection", glm::normalize(glm::vec3(0.5f, 0.5f, 0.5f)));
    m_pShaderProgram->SetUniform("sampler0", 0);
    
    
    //remove this for using the triangle shader
    m_pSphere->Create(path+"/textures/", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
}



void Game::Update(){
    
    m_rotY += 100.0f * m_dt;
    
}


// Render method runs repeatedly in a loop
void Game::Render() 
{
    
    // Clear the buffer for rendering a new frame
    gameWindow.clearWindow();
    
    // Bind the VAO
    glBindVertexArray(m_uiVAO);
    
    // Set the modeling matrix
    //*m_pModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    *m_pModelMatrix = glm::mat4(1);
    *m_pModelMatrix = glm::rotate(*m_pModelMatrix, m_rotY, glm::vec3(0, 0.1, 0));
    m_pShaderProgram->SetUniform("modelMatrix", m_pModelMatrix);
    
    //render sphere
    m_pSphere->Render();
    
    // Render the triangle consisting of 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Swap buffers to show the rendered image
    gameWindow.swapBuffers(); 	
}


// The game loop runs repeatedly until game over.  Later we'll add an update method here and use a timer as well to avoid framerate dependent motion.
void Game::GameLoop()
{
    Update();
    Render();
}

int keyCode; 
int button;

static void OnKeyDown_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    std::cout << "Key pressed with key: " << key << " and with action: " << action << std::endl;
    
    switch (action) {
        case GLFW_PRESS:
            keyCode = key; 
            std::cout << "key pressed" << std::endl;
            break;
        case GLFW_RELEASE:
            keyCode = -1; 
            std::cout << "key released" << std::endl; 
            break;
        case GLFW_REPEAT:
            keyCode = key;
            std::cout << "key down" << std::endl; 
            break;
        default:
            break;
    }
    
}

static void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods){
    std::cout << "Mouse Down with button: " << button << " and with action: " << action << std::endl;
    //onMouseDown(button,action);
    
}


void Game::Execute() 
{
    gameWindow.setWidth(1000);
    gameWindow.setHeight(700);
    
    gameWindow.createWindow();
    printf("Starting App\n");
    
    std::string filepath = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLUni/OpenGLUni/OpenGLUni/resources";
    
    //drawTriangle(filepath);//draw triangle from lab 1
    
    drawSphere(filepath);//sphere from lab 2
    
    
    keyCode = -1;
    gameWindow.setInputs(OnKeyDown_callback, OnMouseDown_callback);
    
    while ( !gameWindow.shouldClose() ){
        gameWindow.setViewport();
        
        GameLoop();
        
        glfwPollEvents();           
    }
    
    gameWindow.destroyWindow();
    
}

