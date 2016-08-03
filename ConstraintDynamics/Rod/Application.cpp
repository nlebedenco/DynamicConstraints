#include "Application.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <iostream>
#include <string>

#include "Common/Vector2.h"
#include "Common/Texture.h"
#include "Common/StringUtil.h"
#include "Common/Extensions.h"


const char* const 
Application::
FONT_FILENAME = "fixedfont.bmp";

void 
Application::
SdlOpen()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::pexit(EXIT_FAILURE, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		fprintf(stderr, "WARNING: Linear texture filtering not enabled!");

	//Create window
	sdlWindow = SDL_CreateWindow("Particle Sim 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (sdlWindow == NULL)
		std::pexit(EXIT_FAILURE, "Window could not be created! SDL Error: %s\n", SDL_GetError());

	//Create renderer for window
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRenderer == NULL)
		std::pexit(EXIT_FAILURE, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());

	sdlCanvas = SDL_GetWindowSurface(sdlWindow);
	if (sdlCanvas == NULL)
		std::pexit(EXIT_FAILURE, "Canvas could not be created! SDL Error: %s\n", SDL_GetError());

	//Initialize renderer color
	SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		std::pexit(EXIT_FAILURE, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
}

void 
Application::
GuiOpen()
{
	guiImageLoader = new gcn::SDLImageLoader();
	// The ImageLoader in use is static and must be set to be able to load images
	gcn::Image::setImageLoader(guiImageLoader);
	guiGraphics = new gcn::SDLGraphics();
	// Set the target for the graphics object to be the screen.
	// In other words, we will draw to the screen.
	// Note, any surface will do, it doesn't have to be the screen.
	guiGraphics->setTarget(sdlCanvas);
	guiInput = new gcn::SDLInput();

	// Initialize and create the gui with Guichan stuff.
	guiTop = new gcn::Container();
	// Set the dimension of the top container to match the screen.
	guiTop->setDimension(gcn::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	// Set base color
	guiTop->setBaseColor(gcn::Color(255, 255, 255));

	gui = new gcn::Gui();
	// Set gui to use the SDLGraphics object.
	gui->setGraphics(guiGraphics);
	// Set gui to use the SDLInput object
	gui->setInput(guiInput);
	// Set the top container
	gui->setTop(guiTop);
	// Load the image font.
	guiFont = new gcn::ImageFont(FONT_FILENAME, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:-");
	// The global font is static and must be set.
	gcn::Widget::setGlobalFont(guiFont);
}

void 
Application::
GuiClose()
{
	delete guiFont;
	delete gui;

	delete guiTop;

	delete guiInput;
	delete guiGraphics;
	delete guiImageLoader;
}

void 
Application::
SdlClose()
{
	//Destroy window	
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Application::
Application()
	: sdlWindow(0),
	  sdlCanvas(0),
	  sdlRenderer(0),
	  guiInput(0),
	  guiGraphics(0),
	  guiImageLoader(0),
	  gui(0),
	  guiFont(0),
	  guiTop(0)
{
	std::randomize();

	SdlOpen();
	GuiOpen();
}

Application::
~Application()
{
	GuiClose();
	SdlClose();
}

int 
Application::
Run()
{
	return 0;
}