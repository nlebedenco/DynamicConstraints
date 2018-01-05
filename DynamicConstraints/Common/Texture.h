#pragma once

#include <string>

#include <SDL.h>
#include <SDL_image.h>

class Texture
{
private:
    SDL_Renderer* renderer;
    void free();

public:
    Texture(SDL_Renderer* renderer);

    ~Texture();

    void Load(const std::string& filename);

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

private:

    SDL_Texture* texture;
    int width;
    int height;
};
