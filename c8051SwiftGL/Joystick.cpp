#include "Joystick.hpp"
//https://www.haroldserrano.com/blog/how-to-add-a-joystick-to-your-game-using-opengl-es

Joystick::Joystick(float uJoystickXPosition, float uJoystickYPosition, const char* uBackgroundJoystickImage, float uJoystickBackgroundWidth,float uJoystickBackgroundHeight, const char* uJoystickImage,float uJoystickWidth,float uJoystickHeight,float uScreenWidth,float uScreenHeight)
{

    //1. screen width and height
    screenWidth=uScreenWidth;
    screenHeight=uScreenHeight;

    //2. Joystick driver & background width and height
    joystickDriverWidth=uJoystickWidth;
    joystickDriverHeight=uJoystickHeight;

    joystickBackgroundWidth=uJoystickBackgroundWidth;
    joystickBackgroundHeight=uJoystickBackgroundHeight;

    //3. set the reference of both Joystick element images
    joystickImage=uJoystickImage;
    joystickBackgroundImage=uBackgroundJoystickImage;

    //4. Joystick x and y position. Because our ortho matrix is in the range of [-1,1]. We need to convert from screen coordinates to ortho coordinates.
    joystickXPosition=uJoystickXPosition*2/screenWidth-1;
    joystickYPosition=uJoystickYPosition*(-2/screenHeight)+1;

    //5. calculate the boundaries of the Joystick
    left=joystickXPosition-joystickDriverWidth/screenWidth;
    right=joystickXPosition+joystickDriverWidth/screenWidth;

    top=joystickYPosition+joystickDriverHeight/screenHeight;
    bottom=joystickYPosition-joystickDriverHeight/screenHeight;

    //6. set the bool value to false
    isPressed=false;

    //7. set the vertex and UV coordinates for both joystick elements
    setJoystickBackgroundVertexAndUVCoords();
    setJoystickDriverVertexAndUVCoords();

}
