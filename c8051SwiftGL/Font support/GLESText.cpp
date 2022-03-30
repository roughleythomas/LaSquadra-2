//
//  GLESText.cpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-30.
//

#include "GLESText.hpp"
#include "GLESFTText.hpp"

struct CPPClass
{
    GLESFTText textObj;
};

GLESText::GLESText(){
    
    theObj = new CPPClass;
    
    fontPtSize = 24;
    dpi = 100;
    posx = 0.2f;
    posy = 0.2f;
}

void GLESText::DrawText(char * theText, char * fontName)
{
    theObj->textObj.DrawText(fontName, theText, fontPtSize, dpi, posx, posy);
}

unsigned char* GLESText::GetImage (){ return theObj->textObj.GetImage(); }

int GLESText::GetWidth() { return theObj->textObj.GetWidth(); }
int GLESText::GetHeight() { return theObj->textObj.GetHeight(); }
