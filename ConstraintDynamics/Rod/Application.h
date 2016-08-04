#pragma once
#include <string>
#include <set>

#include <SDL.h>
#include <SDL_image.h>

#include "Guisan/guisan.hpp"
#include "Guisan/sdl.hpp"

#include "Point.h"
#include "Rod.h"

class Application
{
	// Constants 
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;

	static const char* const FONT_FILENAME;

	std::string title;
	bool terminated;
	float fixedDeltaTime;
	Point* dragPoint;
	float gravity;
	float conservation;

	typedef std::set<Rod*> rodlist;
	rodlist rods;

	int fps;

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
	gcn::Label* lblFPS;
	gcn::CheckBox* chkGravity;
	gcn::TextField* textGravity;
	gcn::CheckBox* chkConservation;
	gcn::TextField* textConservation;

	void SdlOpen();

	void GuiOpen();

	void GuiClose();

	void SdlClose();

	void createWidgets();

	void updateWidgets();

	void destroyWidgets();

	Point* findPointAt(const Vector2& position);

public:
	Application(const std::string& title, float fixedDeltaTime = 0.022);

	~Application();

	int Run();
};