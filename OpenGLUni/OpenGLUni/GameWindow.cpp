//
//  GameWindow.cpp
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#include "GameWindow.h"

#define SIMPLE_OPENGL_CLASS_NAME "simple_openGL_class_name"

GameWindow::GameWindow(const GameWindow &other) {
    this -> m_sAppName = other.m_sAppName;
    this -> m_width = other.m_width;
    this -> m_height = other.m_height;
    this -> m_bFullscreen = other.m_bFullscreen;
}

GameWindow &GameWindow::operator=(const GameWindow &other){
    this -> m_sAppName = other.m_sAppName;
    this -> m_width = other.m_width;
    this -> m_height = other.m_height;
    this -> m_bFullscreen = other.m_bFullscreen;
    return *this;
}

GameWindow& GameWindow::GetInstance() 
{
    static GameWindow instance;
    
    return instance;
}

GameWindow::GameWindow() : m_sAppName("OpenGL Window"), m_width(600), m_height(400), m_bFullscreen(false) 
{
}

GameWindow::GameWindow(std::string name, int w, int h) : m_sAppName(name), m_width(w), m_height(h), m_bFullscreen(false) 
{
}

// Initialise GLEW and create the real game window
void GameWindow::createWindow() 
{
    if(!InitGLFW()){
        return;
    }
    
    CreateGameWindow(m_sAppName);
    
    return;
}


// Create a dummy window, intialise GLEW, and then delete the dummy window
bool GameWindow::InitGLFW()
{
    static bool bGlewInitialized = false;
    if(bGlewInitialized){ 
        return true;
    }
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return false;
    } 
    
    GLFWwindow* fakeWindow = glfwCreateWindow(m_width, m_height, "OpenGL Window", nullptr, nullptr); // Windowed
    //GLFWwindow* window = glfwCreateWindow(height, height, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
    
    
    if (fakeWindow == nullptr) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(fakeWindow);
    
    bool bResult = true;
    
    if(!bGlewInitialized)
    {
        if(glewInit() != GLEW_OK){
            throw std::runtime_error("glewInit failed");
            bResult = false;
        }
        
        bGlewInitialized = true;
    }
    
    glfwDestroyWindow(fakeWindow);
    glfwTerminate();
    
    return bResult;
}

// Create the game window
void GameWindow::CreateGameWindow(std::string sTitle) 
{
    //const char *title
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return;
    } 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    if (m_bFullscreen){
        m_window = glfwCreateWindow(m_width, m_height, sTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);//// Windowed
    }else{
        m_window = glfwCreateWindow(m_width, m_height, sTitle.c_str(), nullptr, nullptr); // Fullscreen
    }
    
    if (m_window == nullptr) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return ;
    }
    
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); ////<-- force interval (not guaranteed to work with all graphics drivers)
    
    //start GLEW extension handler
    glewExperimental = GL_TRUE;
    if(InitOpenGL()){
        return;
    }
    
    return;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Initialise OpenGL, including the pixel format descriptor and the OpenGL version
bool GameWindow::InitOpenGL()
{
    bool bError = false;
    
    // start GLEW extension handler
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("glewInit failed");
        bError = true;
    }
    
    if(!GLEW_VERSION_3_2){
        throw std::runtime_error("OpenGL 3.2 API is not available.");
        bError = true;
    }
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    if(bError)
    {
        glfwSetErrorCallback(error_callback);
        return bError;
    }
    
    return bError;
}


void GameWindow::setInputs(const GLFWkeyfun &cbfunKey, const GLFWmousebuttonfun &cbfunMouse){
    
    glfwSetKeyCallback(m_window, cbfunKey);
    //glfwSetCursorPosCallback(m_window, OnMouseMove_callback);
    glfwSetMouseButtonCallback(m_window, cbfunMouse );
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
    glfwMakeContextCurrent(m_window);
    
}

//Check whether window should close
bool GameWindow::shouldClose(){
    return glfwWindowShouldClose(m_window);
}

//Get the Current framebuffer Size in pixels and Set the Viewport to it    
void GameWindow::setViewport(){ 
    glfwGetFramebufferSize(m_window, &m_width, &m_height); 
    glViewport(0,0,m_width,m_height); 
    
    glMatrixMode(GL_PROJECTION);
    glOrtho(-ratio(), ratio(), -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);			// Select The Projection Matrix
    glLoadIdentity();				// Reset The Projection Matrix
    
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)m_width/(GLfloat)m_height,0.1f,200.0f);
    
    glMatrixMode(GL_MODELVIEW);			// Select The Modelview Matrix
    glLoadIdentity();				// Reset The Modelview Matrix
}

void GameWindow::clearWindow(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                  ////<-- CLEAR WINDOW CONTENTS
    glLoadIdentity();	
}

//Swap front and back buffers
void GameWindow::swapBuffers(){
    glfwSwapBuffers(m_window);
}

//Destroy the window, // Deinitialise the window and rendering context
void GameWindow::destroyWindow(){
    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// Deinitialise the window and rendering context
GameWindow::~GameWindow(){
    destroyWindow();
}

