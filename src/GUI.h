#pragma once
#include <sstream>

const std::uint32_t WINDOW_WIDTH         = 1016;
const std::uint32_t WINDOW_HEIGHT        = 960;

const std::uint32_t CHRTABLE_RENDERSCALE = 2;
const std::uint32_t CHRTABLE_WIDTH       = 128;
const std::uint32_t CHRTABLE_HEIGHT      = 256;
const std::uint32_t CHRTABLE_POSX        = 20;
const std::uint32_t CHRTABLE_POSY        = 20;

const std::uint32_t CPUSTATUS_WIDTH      = 128;
const std::uint32_t CPUSTATUS_HEIGHT     = 400;
const std::uint32_t CPUSTATUS_POSX       = 900;
const std::uint32_t CPUSTATUS_POSY       = 900;

const std::uint32_t SCREEN_RENDERSCALE   = 4;
const std::uint32_t SCREEN_WIDTH         = 256;
const std::uint32_t SCREEN_HEIGHT        = 240;
const std::uint32_t SCREEN_POSX          = 0*(WINDOW_WIDTH/SCREEN_RENDERSCALE - SCREEN_WIDTH);
const std::uint32_t SCREEN_POSY          = 0*(WINDOW_HEIGHT/SCREEN_RENDERSCALE - SCREEN_HEIGHT);
// const std::uint32_t SCREEN_POSY          = 10;

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void RenderCPUStatus(SDL_Renderer* Renderer, TTF_Font* Font, std::vector<std::string> CPUStatusStr)
{
    for(std::uint32_t i = 0;i<CPUStatusStr.size();i++)
    {
        SDL_Rect Dest;
        SDL_Surface* TextSurface = TTF_RenderText_Blended_Wrapped(Font, CPUStatusStr[i].c_str(), {255,255,255},1000);
        SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
        Dest.x = CPUSTATUS_POSX;
        Dest.y = CPUSTATUS_POSY + 30*i;
        Dest.w = TextSurface->w;
        Dest.h = TextSurface->h;
        SDL_RenderCopy(Renderer, TextTexture, NULL, &Dest);
        SDL_DestroyTexture(TextTexture);
        SDL_FreeSurface(TextSurface);
    }
}

void RenderScreen(SDL_Renderer* Renderer, SDL_Texture* Texture)
{
    SDL_SetRenderDrawColor( Renderer, 0XAA, 0XAA, 0XAA, 0XAA ); //setting a background colour
    SDL_RenderSetScale(Renderer, SCREEN_RENDERSCALE, SCREEN_RENDERSCALE);
    SDL_Rect Rect;
    Rect.x = SCREEN_POSX;
    Rect.y = SCREEN_POSY;
    Rect.w = SCREEN_WIDTH;
    Rect.h = SCREEN_HEIGHT;
    SDL_RenderCopy(Renderer, Texture, NULL, NULL);
    SDL_RenderFillRect( Renderer, &Rect ); // Draw a filled rectangle
    SDL_RenderSetScale(Renderer, 1, 1);
}

void RenderPatternTables(SDL_Renderer* Renderer, SDL_Texture* Texture, std::vector<uint8_t> Framebuffer)
{
    unsigned char* pixels;
    int pitch;
    SDL_RenderSetScale(Renderer, CHRTABLE_RENDERSCALE, CHRTABLE_RENDERSCALE);
    SDL_Rect Rect;
    Rect.x = CHRTABLE_POSX;   //the x coordinate
    Rect.y = CHRTABLE_POSY;   //the y coordinate
    Rect.w = CHRTABLE_WIDTH;  //the width of the texture
    Rect.h = CHRTABLE_HEIGHT; //the height of the texture
    SDL_LockTexture( Texture, NULL, (void**)&pixels, &pitch );
    for(int i = 0; i < 256*pitch; i++) {
        pixels[i] = Framebuffer[i];
    }
    SDL_UnlockTexture( Texture );
    SDL_RenderCopy(Renderer, Texture, NULL, &Rect); //actually drawing the texture to the screen.
    SDL_RenderSetScale(Renderer, 1, 1);
}

void RenderScreen(SDL_Renderer* Renderer, SDL_Texture* Texture, std::vector<uint8_t> Framebuffer)
{
    unsigned char* pixels;
    int pitch;
    SDL_RenderSetScale(Renderer, SCREEN_RENDERSCALE, SCREEN_RENDERSCALE);
    SDL_Rect Rect;
    Rect.x = SCREEN_POSX;   //the x coordinate
    Rect.y = SCREEN_POSY;   //the y coordinate
    Rect.w = SCREEN_WIDTH;  //the width of the texture
    Rect.h = SCREEN_HEIGHT; //the height of the texture
    SDL_LockTexture( Texture, NULL, (void**)&pixels, &pitch );
    for(int i = 0; i < 240*pitch; i++) {
        pixels[i] = Framebuffer[i];
    }
    SDL_UnlockTexture( Texture );
    SDL_RenderCopy(Renderer, Texture, NULL, &Rect); //actually drawing the texture to the screen.
    SDL_RenderSetScale(Renderer, 1, 1);
}

void RenderSprites(SDL_Renderer* Renderer, SDL_Texture* Texture, std::vector<uint8_t> Framebuffer)
{
    unsigned char* pixels;
    int pitch;
    SDL_RenderSetScale(Renderer, SCREEN_RENDERSCALE, SCREEN_RENDERSCALE);
    SDL_Rect Rect;
    Rect.x = SCREEN_POSX;   //the x coordinate
    Rect.y = SCREEN_POSY;   //the y coordinate
    Rect.w = SCREEN_WIDTH;  //the width of the texture
    Rect.h = SCREEN_HEIGHT; //the height of the texture
    SDL_LockTexture( Texture, NULL, (void**)&pixels, &pitch );
    for(int i = 0; i < 240*pitch; i++) {
        pixels[i] = Framebuffer[i];
    }
    SDL_UnlockTexture( Texture );
    SDL_RenderCopy(Renderer, Texture, NULL, &Rect); //actually drawing the texture to the screen.
    SDL_RenderSetScale(Renderer, 1, 1);
}

std::string to_hex_string( uint8_t Value ) {
    std::stringstream s;
    s << "0x" << std::hex << Value;
    return s.str();
}
