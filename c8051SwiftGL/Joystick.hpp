#ifndef Joystick_hpp
#define Joystick_hpp

#include <stdio.h>
#include <stdlib.h>

#include <OpenGLES/ES3/gl.h>

class Joystick
{
public:
    Joystick(float uJoystickXPosition, float uJoystickYPosition, const char* uBackgroundJoystickImage, float uJoystickBackgroundWidth,float uJoystickBackgroundHeight, const char* uJoystickImage,float uJoystickWidth,float uJoystickHeight,float uScreenWidth,float uScreenHeight);
    bool isPressed;
private:
    float screenWidth, screenHeight, joystickDriverWidth, joystickDriverHeight, joystickBackgroundWidth, joystickBackgroundHeight, joystickXPosition, joystickYPosition, left, right, top, bottom;
    
    const char* joystickImage;
    const char* joystickBackgroundImage;
    
    void setJoystickBackgroundVertexAndUVCoords();
};

#endif /* GLESRenderer_hpp */
