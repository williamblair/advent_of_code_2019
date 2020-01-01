#ifndef _SDL_WINDOW_H_INCLUDED_
#define _SDL_WINDOW_H_INCLUDED_

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

class SdlWindow
{
public:
    SdlWindow();
    ~SdlWindow();

    // returns false if failed to init
    bool init(const int width, const int height);

    // fills the screen with black
    void clear();

    // returns false when window quit event received,
    // otherwise returns true
    bool update();

    // draws a line; does NOT update the screen
    void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b);
    
private:

    SDL_Surface *win_surface;
};

#endif //_SDL_WINDOW_H_INCLUDED_ 


