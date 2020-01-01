#include "SdlWindow.h"

SdlWindow::SdlWindow() :
    win_surface(NULL)
{}

SdlWindow::~SdlWindow()
{
    if (win_surface != NULL)
    {
        SDL_FreeSurface(win_surface);
        win_surface = NULL;
        SDL_Quit();
    }
}

bool SdlWindow::init(const int width, const int height)
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    win_surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
    if (win_surface == NULL)
    {
        std::cerr << "Error setting video mode: " << SDL_GetError() 
                  << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

void SdlWindow::clear()
{
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format,25,0,25));
}

bool SdlWindow::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "Window quit event\n";
            return false;
        }
    }

    SDL_Flip(win_surface);
    
    // approx 30 fps
    SDL_Delay(1000.0F/30.0F);

    return true;
}

void SdlWindow::draw_line(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    lineColor(win_surface, x1,y1,x2,y2,SDL_MapRGB(win_surface->format, r,g,b));
}

