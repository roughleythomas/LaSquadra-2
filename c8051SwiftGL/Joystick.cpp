#include "Joystick.hpp"
//https://www.haroldserrano.com/blog/how-to-add-a-joystick-to-your-game-using-opengl-es

Joystick::Joystick()
{
    
    
}


Joystick::Joystick(float uJoystickXPosition, float uJoystickYPosition, /*const char* uBackgroundJoystickImage,*/ float uJoystickBackgroundWidth,float uJoystickBackgroundHeight, /*const char* uJoystickImage,*/ float uJoystickWidth,float uJoystickHeight,float uScreenWidth,float uScreenHeight)
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
    //joystickImage=uJoystickImage;
    //joystickBackgroundImage=uBackgroundJoystickImage;

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

void Joystick::setJoystickBackgroundVertexAndUVCoords()
{
    
    //1. set the width, height and depth for the joystick background image rectangle
    float width=joystickBackgroundWidth/screenWidth;
    float height=joystickBackgroundHeight/screenHeight;
    float depth=0.0;

    //2. Set the value for each vertex into an array

    //Upper-Right Corner vertex of rectangle
    joystickBackgroundVertices[0]=width;
    joystickBackgroundVertices[1]=height;
    joystickBackgroundVertices[2]=depth;

    //Lower-Right corner vertex of rectangle
    joystickBackgroundVertices[3]=width;
    joystickBackgroundVertices[4]=-height;
    joystickBackgroundVertices[5]=depth;

    //Lower-Left corner vertex of rectangle
    joystickBackgroundVertices[6]=-width;
    joystickBackgroundVertices[7]=-height;
    joystickBackgroundVertices[8]=depth;

    //Upper-Left corner vertex of rectangle
    joystickBackgroundVertices[9]=-width;
    joystickBackgroundVertices[10]=height;
    joystickBackgroundVertices[11]=depth;


    //3. Set the value for each uv coordinate into an array

    joystickBackgroundUVCoords[0]=1.0;
    joystickBackgroundUVCoords[1]=0.0;

    joystickBackgroundUVCoords[2]=1.0;
    joystickBackgroundUVCoords[3]=1.0;

    joystickBackgroundUVCoords[4]=0.0;
    joystickBackgroundUVCoords[5]=1.0;

    joystickBackgroundUVCoords[6]=0.0;
    joystickBackgroundUVCoords[7]=0.0;

    //4. set the value for each index into an array

    joystickBackgroundIndex[0]=0;
    joystickBackgroundIndex[1]=1;
    joystickBackgroundIndex[2]=2;

    joystickBackgroundIndex[3]=2;
    joystickBackgroundIndex[4]=3;
    joystickBackgroundIndex[5]=0;
    
}

void Joystick::setJoystickDriverVertexAndUVCoords()
{
    
    //1. set the width, height and depth for the joystick background image rectangle
    float width=joystickBackgroundWidth/screenWidth;
    float height=joystickBackgroundHeight/screenHeight;
    float depth=0.0;

    //2. Set the value for each vertex into an array

    //Upper-Right Corner vertex of rectangle
    joystickDriverVertices[0]=width;
    joystickDriverVertices[1]=height;
    joystickDriverVertices[2]=depth;

    //Lower-Right corner vertex of rectangle
    joystickDriverVertices[3]=width;
    joystickDriverVertices[4]=-height;
    joystickDriverVertices[5]=depth;

    //Lower-Left corner vertex of rectangle
    joystickDriverVertices[6]=-width;
    joystickDriverVertices[7]=-height;
    joystickDriverVertices[8]=depth;

    //Upper-Left corner vertex of rectangle
    joystickDriverVertices[9]=-width;
    joystickDriverVertices[10]=height;
    joystickDriverVertices[11]=depth;


    //3. Set the value for each uv coordinate into an array

    joystickDriverUVCoords[0]=1.0;
    joystickDriverUVCoords[1]=0.0;

    joystickDriverUVCoords[2]=1.0;
    joystickDriverUVCoords[3]=1.0;

    joystickDriverUVCoords[4]=0.0;
    joystickDriverUVCoords[5]=1.0;

    joystickDriverUVCoords[6]=0.0;
    joystickDriverUVCoords[7]=0.0;

    //4. set the value for each index into an array

    joystickDriverIndex[0]=0;
    joystickDriverIndex[1]=1;
    joystickDriverIndex[2]=2;

    joystickDriverIndex[3]=2;
    joystickDriverIndex[4]=3;
    joystickDriverIndex[5]=0;
    
}

void Joystick::setupJoyStickBackgroundOpenGL(){

//...

//5a. Dump the data into the Buffer

glBufferData(GL_ARRAY_BUFFER, sizeof(joystickBackgroundVertices)+sizeof(joystickBackgroundUVCoords), NULL, GL_STATIC_DRAW);

//5b. Load vertex data with glBufferSubData
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(joystickBackgroundVertices), joystickBackgroundVertices);

//5c. Load uv data with glBufferSubData
glBufferSubData(GL_ARRAY_BUFFER, sizeof(joystickBackgroundVertices), sizeof(joystickBackgroundUVCoords), joystickBackgroundUVCoords);

//...

}

void Joystick::setupJoyStickDriverOpenGL(){

//...

//5a. Dump the data into the Buffer

glBufferData(GL_ARRAY_BUFFER, sizeof(joystickDriverVertices)+sizeof(joystickDriverUVCoords), NULL, GL_STATIC_DRAW);

//5b. Load vertex data with glBufferSubData
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(joystickDriverVertices), joystickDriverVertices);

//5c. Load uv data with glBufferSubData
glBufferSubData(GL_ARRAY_BUFFER, sizeof(joystickDriverVertices), sizeof(joystickDriverUVCoords), joystickDriverUVCoords);

//...

}

void Joystick::draw()
{
    
    //glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);
    
}
