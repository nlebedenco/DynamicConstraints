#include "Texture.h"
#include "StringUtil.h"

Texture::
Texture(SDL_Renderer* renderer)
    : renderer(renderer),
      texture(NULL),
      width(0),
      height(0)
{
}

Texture::
~Texture()
{
    free();
    renderer = NULL;
}

void
Texture::
free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

void 
Texture::
Load(const std::string& path)
{
    free();

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        throw new std::runtime_error(std::format("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError()));
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL)
        {
            throw new std::runtime_error(std::format("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError()));
        }
        else
        {
            //Get image dimensions
            width = loadedSurface->w;
            height = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
}

void 
Texture::
setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(texture, red, green, blue);
}

void 
Texture::
setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(texture, blending);
}

void 
Texture::
setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(texture, alpha);
}

void 
Texture::
render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width, height };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int 
Texture::
getWidth()
{
    return width;
}

int 
Texture::
getHeight()
{
    return height;
}