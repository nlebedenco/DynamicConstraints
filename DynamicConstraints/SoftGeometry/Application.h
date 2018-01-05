#pragma once
#include <string>
#include <set>

#include <SDL.h>
#include <SDL_image.h>

#include "Guisan/guisan.hpp"
#include "Guisan/sdl.hpp"

#include "Point.h"
#include "SoftGeometry.h"

class Application
{
	// Constants 
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;

	static const int MIN_X = 0;
	static const int MIN_Y = 150;

	static const int ROD_LENGTH = 100;

	static const char* const FONT_FILENAME;

	std::string title;
	bool terminated;
	float fixedDeltaTime;
	float timeScale;
	float gravity;
	float conservation;
	float elasticity;
	float elasticConservation;

	int fps;
	bool dragging;
	bool clicking;

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

	// Widgets
	gcn::Label* lblDescription;
	gcn::Label* lblFPS;
	gcn::CheckBox* chkTimeScale;
	gcn::TextField* textTimeScale;
	gcn::CheckBox* chkGravity;
	gcn::TextField* textGravity;
	gcn::CheckBox* chkConservation;
	gcn::TextField* textConservation;
	gcn::CheckBox* chkElasticity;
	gcn::TextField* textElasticity;
	gcn::CheckBox* chkElasticConservation;
	gcn::TextField* textElasticConservation;

	void SdlOpen();

	void GuiOpen();

	void GuiClose();

	void SdlClose();

	void createWidgets();

	void updateWidgets();

	void destroyWidgets();

public:
	Application(const std::string& title);

	~Application();

	int Run();
};