//
//  Game.cpp
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//


#include "Game.h"

// Constructor
Game::Game()
{
    m_pShaderProgram = nullptr;
}

// Destructor
Game::~Game() 
{ 
    delete m_pShaderProgram;
}

// Initialisation:  This method only runs once at startup
void Game::draw() 
{
    
    m_pShaderProgram = new CShaderProgram;
    m_pModelMatrix = new glm::mat4(1);
    m_pViewMatrix = new glm::mat4(1);
    m_pProjectionMatrix = new glm::mat4(1);
    
    
    GLuint uiVBO[2];	// Two vertex buffer objects
    
    float fTrianglePositions[9];	// An array to store triangle vertex positions
    float fTriangleColor[9];		// An array to store triangle vertex colours
    
    // This sets the position, viewpoint, and up vector of the camera
    glm::vec3 vEye(10, 10, 10);
    glm::vec3 vView(0, 0, 0);
    glm::vec3 vUp(0, 1, 0);
    *m_pViewMatrix = glm::lookAt(vEye, vView, vUp);
    
    // This creates a view frustum
    *m_pProjectionMatrix = glm::perspective(45.0f, 1.333f, 1.0f, 150.0f);
    
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
    
    
    
    
}

void Game::addShadersFromResources(std::string path){
    
    // Load and compile shaders 
    CShader shVertex, shFragment;	
    
    //full driectory of the shaders
    shVertex.LoadShader(path+"shader.vert", GL_VERTEX_SHADER);
    shFragment.LoadShader(path+"shader.frag", GL_FRAGMENT_SHADER);
    
    // Create shader program and add shaders
    m_pShaderProgram->CreateProgram();
    m_pShaderProgram->AddShaderToProgram(&shVertex);
    m_pShaderProgram->AddShaderToProgram(&shFragment);
    
    // Link and use the program
    m_pShaderProgram->LinkProgram();
    m_pShaderProgram->UseProgram();
    
    // Set the modeling, viewing, and projection matrices in the shader
    m_pShaderProgram->SetUniform("viewMatrix", m_pViewMatrix);
    m_pShaderProgram->SetUniform("projectionMatrix", m_pProjectionMatrix);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
}


void Game::drawTriangle(){
    
    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    GLuint triangleVBO = 0;
    
    glGenBuffers(1, &triangleVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
    
    //GLuint triangleVAO = 0;
    m_uiVAO = 0;
    
    glGenVertexArrays(1, &m_uiVAO);
    
    glBindVertexArray(m_uiVAO);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    
    
}

void Game::addShaders(){
    
    const char* vertexShaderSource =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4 (vp, 1.0);"
    "}";
    
    const char* fragmentShaderSource =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    
    glCompileShader(fragmentShader);
    
    
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, fragmentShader);
    
    glAttachShader(shaderProgram, vertexShader);
    
    glLinkProgram(shaderProgram);
}

void Game::Update(){
    
    
}


// Render method runs repeatedly in a loop
void Game::Render() 
{
    
    // Clear the buffer for rendering a new frame
    gameWindow.clearWindow();
    
    // Bind the VAO
    glBindVertexArray(m_uiVAO);
    
    //use shader program
    glUseProgram(shaderProgram);
    
    // Set the modeling matrix
    //*m_pModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
    //m_pShaderProgram->SetUniform("modelMatrix", m_pModelMatrix);
    
    // Render the triangle consisting of 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Swap buffers to show the rendered image
    gameWindow.swapBuffers(); 	
}


// The game loop runs repeatedly until game over.  Later we'll add an update method here and use a timer as well to avoid framerate dependent motion.
void Game::GameLoop()
{
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
    
    draw();
    drawTriangle();
    
    //shaders
    //addShadersFromResources("/Users/GeorgeQuentin/Desktop/OpenGLTest/OpenGLTest/resources/shaders/");
    addShaders();
    
    keyCode = -1;
    gameWindow.setInputs(OnKeyDown_callback, OnMouseDown_callback);
    
    while ( !gameWindow.shouldClose() ){
        gameWindow.setViewport();
        
        GameLoop();
        
        glfwPollEvents();           
    }
    
    gameWindow.destroyWindow();
    
}

