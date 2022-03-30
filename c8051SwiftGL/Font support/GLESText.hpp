//
//  GLESText.hpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-30.
//

#ifndef GLESText_hpp
#define GLESText_hpp

struct CPPClass;

class GLESText
{
    private:
        struct CPPClass *theObj;
        int fontPtSize;
        int dpi;
        float posx;
        float posy;
    public:
        void DrawText(char* theText, char* fontName);
        unsigned char* GetImage();
        int GetWidth();
        int GetHeight();
    
};

#endif /* GLESText_h */
