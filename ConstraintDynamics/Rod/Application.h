#pragma once
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Guisan/guisan.hpp"
#include "Guisan/sdl.hpp"

#include "Node.h"

class Application
{
	// Constants 
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;

	static const char* const FONT_FILENAME;

	// SDL
	SDL_Window* sdlWindow;
	SDL_Surface* sdlCanvas;
	SDL_Renderer* sdlRenderer;

	// GUISAN SDL
	gcn::SDLInput* guiInput;				// Input driver
	gcn::SDLGraphics* guiGraphics;			// Graphics driver
	gcn::SDLImageLoader* guiImageLoader;	// For loading images

	// GUISAN
	gcn::Gui* gui;							// A Gui object that binds it all together
	gcn::ImageFont* guiFont;				// Font
	gcn::Container* guiTop;					// Top container

	void SdlOpen();

	void GuiOpen();

	void GuiClose();

	void SdlClose();

public:
	Application();

	~Application();

	int Run();
};