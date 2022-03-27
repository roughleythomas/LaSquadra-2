#ifndef Joystick_hpp
#define Joystick_hpp

#include <stdio.h>
#include <stdlib.h>

#include <OpenGLES/ES3/gl.h>

class Joystick
{
public:
    Joystick();
    Joystick(float uJoystickXPosition, float uJoystickYPosition/*, const char* uBackgroundJoystickImage*/, float uJoystickBackgroundWidth,float uJoystickBackgroundHeight/*, const char* uJoystickImage*/,float uJoystickWidth,float uJoystickHeight,float uScreenWidth,float uScreenHeight);
    bool isPressed;
    GLuint joystickCont, joystickBack;
    float screenWidth, screenHeight;
    float joystickDriverIndex[6];
    
    void draw();
    
private:
    float joystickDriverWidth, joystickDriverHeight, joystickBackgroundWidth, joystickBackgroundHeight, joystickXPosition, joystickYPosition, left, right, top, bottom;
    float joystickBackgroundVertices[12];
    float joystickDriverVertices[12];
    float joystickBackgroundUVCoords[8];
    float joystickDriverUVCoords[8];
    float joystickBackgroundIndex[6];
    
    const char* joystickImage;
    const char* joystickBackgroundImage;
    
    void setJoystickBackgroundVertexAndUVCoords();
    void setJoystickDriverVertexAndUVCoords();
    
    void setupJoyStickBackgroundOpenGL();
    void setupJoyStickDriverOpenGL();
};

#endif /* GLESRenderer_hpp */
