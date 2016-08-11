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
	sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	createWidgets();
}

void 
Application::
GuiClose()
{
	delete guiFont;
	delete gui;
	delete guiTop;

	destroyWidgets();

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

void
Application::
createWidgets()
{
	lblDescription = new gcn::Label("Click and drag one of the black dots using the left mouse button");
	lblFPS = new gcn::Label("FPS:");
	chkGravity = new gcn::CheckBox("Gravity:", true);
	textGravity = new gcn::TextField(std::format("%g", gravity));
	textGravity->setSize(80, 18);

	chkConservation = new gcn::CheckBox("Conservation: ", true);
	textConservation = new gcn::TextField(std::format("%g", conservation));
	textConservation->setSize(80, 18);

	guiTop->add(lblDescription, 8, 8);
	guiTop->add(lblFPS, 8, 28);
	guiTop->add(chkGravity, 8, 48);
	guiTop->add(textGravity, 76, 48);
	guiTop->add(chkConservation, 8, 76);
	guiTop->add(textConservation, 104, 76);
}

void
Application::
updateWidgets()
{
	// Update widgets
	lblFPS->setCaption(std::format("FPS: %d", fps));
	lblFPS->adjustSize();

	try
	{
		const std::string& textGravityValue = textGravity->getText();
		double value = textGravityValue.empty() ? 0 : std::stod(textGravityValue);
		if (value >= 0)
			gravity = chkGravity->isSelected() ? value : 0;
		else
			textGravity->setText(std::format("%g", gravity));
	}
	catch (std::invalid_argument&)
	{
		textGravity->setText(std::format("%g", gravity));
	}

	try
	{
		const std::string& textConservationValue = textConservation->getText();
		double value = textConservationValue.empty() ? 0 : std::stod(textConservationValue);
		if (value >= 0)
			conservation = chkConservation->isSelected() ? value : 0;
		else
			textConservation->setText(std::format("%g", conservation));
	}
	catch (std::invalid_argument&)
	{
		textConservation->setText(std::format("%g", conservation));
	}
}

void 
Application::
destroyWidgets()
{
	delete lblDescription;
	delete lblFPS;
	delete chkGravity;
	delete textGravity;
	delete chkConservation;
	delete textConservation;

}

Point*
Application::
findPointAt(const Vector2& position)
{
	for (rodlist::iterator it = rods.begin(); it != rods.end(); ++it)
	{
		Point* point = (*it)->getPointAt(position);
		if (point != NULL)
			return point;
	}

	return NULL;
}

Application::
Application(const std::string& title, float fixedDeltaTime)
	: title(title),
	  fixedDeltaTime(fixedDeltaTime),
	  terminated(false),
	  dragPoint(0),
	  gravity(8),
	  conservation(0.97),
	  rods(),
	  fps(0),
      sdlWindow(0),
	  sdlCanvas(0),
	  sdlRenderer(0),
	  guiInput(0),
	  guiGraphics(0),
	  guiImageLoader(0),
	  gui(0),
	  guiFont(0),
	  guiTop(0),
      lblDescription(0),
  	  lblFPS(0),
	  chkGravity(0),
	  textGravity(0),
	  chkConservation(0),
      textConservation(0)
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
	// Create Textures
	Texture normalTexture(sdlRenderer);
	normalTexture.Load("normal.bmp");
	Texture selectedTexture(sdlRenderer);
	selectedTexture.Load("selected.bmp");

	// Create Points
	int halfScreenWidth = SCREEN_WIDTH / 2;
	int halfScreenHeight = SCREEN_HEIGHT / 2;

	Point A(Vector2(halfScreenWidth - 80, halfScreenHeight), &normalTexture, &selectedTexture);
	Point B(Vector2(halfScreenWidth + 80, halfScreenHeight), &normalTexture, &selectedTexture);

	// Create Rod
	Rod R(sdlRenderer, &A, &B);
	rods.insert(&R);
	
	// Loop control
	uint32_t last = SDL_GetTicks();
	float elapsedTime = 0;
	while (!terminated)
	{
		// Calculate Delta Time and invoke fixed updates
		uint32_t now = SDL_GetTicks();
		float deltaTime = float(now - last) / 1000;
		last = now;
		fps = (deltaTime > 0) ? (int)((0.95 * fps) + (0.05 / deltaTime)) : fps;
		elapsedTime += deltaTime;
		while (elapsedTime >= fixedDeltaTime)
		{
			for (rodlist::iterator it = rods.begin(); it != rods.end(); ++it)
			{
				(*it)->update(fixedDeltaTime, gravity * 1000, conservation);
			}

			elapsedTime -= fixedDeltaTime;
		}

		// Handle Input
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				terminated = true;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE: terminated = true; break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					//Get the mouse offsets
					dragPoint = findPointAt(Vector2(e.button.x, e.button.y));
					if (dragPoint)
					{
						dragPoint->selected = true;
						dragPoint->grabbed = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
					if (dragPoint)
					{
						dragPoint->selected = false;
						dragPoint->grabbed = false;
						dragPoint = NULL;
					}
				break;
			}

			// Now that we are done polling and using SDL events we pass
			// the leftovers to the SDLInput object to later be handled by
			// the Gui. (This example doesn't require us to do this 'cause a
			// label doesn't use input. But will do it anyway to show how to
			// set up an SDL application with Guichan.)            
			guiInput->pushInput(e);
		}

		//Clear screen
		SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdlRenderer);

		// Let the gui perform it's logic (like handle widget input)
		gui->logic();
		// update widgets (includes parsing and validation)
		updateWidgets();
		// Draw the gui
		gui->draw();

		// Render Canvas
		SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, sdlCanvas);
		SDL_RenderCopy(sdlRenderer, texture, 0, 0);
		SDL_DestroyTexture(texture);

		// Render Rods
		for (rodlist::iterator it = rods.begin(); it != rods.end(); ++it)
		{
			(*it)->render();
		}

		//Update renderer
		SDL_RenderPresent(sdlRenderer);
	}

	return 0;
}