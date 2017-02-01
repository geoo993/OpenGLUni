//
//  GameWindow.hpp
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#ifndef GameWindow_h
#define GameWindow_h

#include "Common.h"


class GameWindow {
    
public:
    
    GameWindow();
    GameWindow(std::string name, int w, int h);
    
    void createWindow(); 
    
    void CreateGameWindow(std::string title);
    bool InitOpenGL();
    bool InitGLFW();
    
    bool Fullscreen() const { return m_bFullscreen; }
    
    int width()  { return m_width; }
    int height() { return m_height; }
    float ratio() { return (float)m_width / m_height;}
    void setWidth(const int &w)  { m_width = w; }
    void setHeight(const int &h) { m_height = h; }
    
    void setInputs(const GLFWkeyfun &cbfunKey, const GLFWmousebuttonfun &cbfunMouse);
    
    void clearWindow();
    void setViewport();
    bool shouldClose();
    void swapBuffers();
    void destroyWindow();
   
    void onKeyDown(int key, int action);
    
    ~GameWindow();
    
    
private:
    
    GameWindow(const GameWindow &other);
    GameWindow &operator=(const GameWindow &other);
    
    std::string m_sAppName;
    
    int m_width, m_height;

    GLFWwindow * m_window = nullptr;
    
    bool  m_bFullscreen;
    
    
};

#endif /* GameWindow_h */
