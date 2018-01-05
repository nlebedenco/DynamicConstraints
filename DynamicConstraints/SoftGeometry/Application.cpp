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



Application::
Application(const std::string& title)
	: title(title),
	  terminated(false),
	  fixedDeltaTime(0.022),
	  timeScale(0.8),
	  gravity(8),
	  conservation(0.97),
	  elasticity(0.8),
	  elasticConservation(0.7),
	  fps(0),
	  dragging(0),
	  clicking(0),
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
      textConservation(0),
	  chkElasticity(0),
      textElasticity(0),
	  chkElasticConservation(0),
	  textElasticConservation(0)
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

void
Application::
createWidgets()
{
	lblDescription = new gcn::Label("Click and drag the large dot or click in the grey area using the left mouse button");

	lblFPS = new gcn::Label("FPS:");

	chkTimeScale = new gcn::CheckBox("TimeScale:", true);
	textTimeScale = new gcn::TextField(std::format("%g", timeScale));
	textTimeScale->setSize(80, 18);

	chkGravity = new gcn::CheckBox("Gravity:", false);
	textGravity = new gcn::TextField(std::format("%g", gravity));
	textGravity->setSize(80, 18);

	chkConservation = new gcn::CheckBox("Momentum: ", false);
	textConservation = new gcn::TextField(std::format("%g", conservation));
	textConservation->setSize(80, 18);

	chkElasticity = new gcn::CheckBox("Hardness: ", true);
	textElasticity = new gcn::TextField(std::format("%g", elasticity));
	textElasticity->setSize(80, 18);

	chkElasticConservation = new gcn::CheckBox("Bounciness: ", true);
	textElasticConservation = new gcn::TextField(std::format("%g", elasticConservation));
	textElasticConservation->setSize(80, 18);

	guiTop->add(lblDescription, 8, 8);

	guiTop->add(lblFPS, 8, 28);

	guiTop->add(chkTimeScale, 8, 48);
	guiTop->add(textTimeScale, 104, 48);

	guiTop->add(chkGravity, 8, 48 + 28);
	guiTop->add(textGravity, 76, 48 + 28);

	guiTop->add(chkConservation, 8, 48 + 28 + 28);
	guiTop->add(textConservation, 104, 48 + 28 + 28);

	guiTop->add(chkElasticity, 200 + 8, 48);
	guiTop->add(textElasticity, 200 + 84, 48);

	guiTop->add(chkElasticConservation, 200 + 8, 48 + 28);
	guiTop->add(textElasticConservation, 200 + 94, 48 + 28);
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
		const std::string& text = textTimeScale->getText();
		double value = std::clamp<double>(text.empty() ? 0 : std::stod(text), 0.1, 1);
		if (value >= 0)
			timeScale = chkTimeScale->isSelected() ? value : 1;
		else
			textTimeScale->setText(std::format("%g", timeScale));
	}
	catch (std::invalid_argument&)
	{
		textTimeScale->setText(std::format("%g", timeScale));
	}

	try
	{
		const std::string& text = textGravity->getText();
		double value = text.empty() ? 0 : std::stod(text);
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
		const std::string& text = textConservation->getText();
		double value = text.empty() ? 0 : std::stod(text);
		if (value >= 0)
			conservation = chkConservation->isSelected() ? value : 0;
		else
			textConservation->setText(std::format("%g", conservation));
	}
	catch (std::invalid_argument&)
	{
		textConservation->setText(std::format("%g", conservation));
	}

	try
	{
		const std::string& text = textElasticity->getText();
		double value = text.empty() ? 0 : std::stod(text);
		if (value >= 0)
			elasticity = chkElasticity->isSelected() ? value : 0;
		else
			textElasticity->setText(std::format("%g", elasticity));
	}
	catch (std::invalid_argument&)
	{
		textElasticity->setText(std::format("%g", elasticity));
	}

	try
	{
		const std::string& text = textElasticConservation->getText();
		double value = std::clamp<double>(text.empty() ? 0 : std::stod(text), 0, 1);
		if (value >= 0)
			elasticConservation = chkElasticConservation->isSelected() ? value : 0;
		else
			textElasticConservation->setText(std::format("%g", elasticConservation));
	}
	catch (std::invalid_argument&)
	{
		textElasticConservation->setText(std::format("%g", elasticConservation));
	}
}

void
Application::
destroyWidgets()
{
	delete lblDescription;
	delete lblFPS;
	delete chkTimeScale;
	delete textTimeScale;
	delete chkGravity;
	delete textGravity;
	delete chkConservation;
	delete textConservation;
	delete chkElasticity;
	delete textElasticity;
	delete chkElasticConservation;
	delete textElasticConservation;
}

int 
Application::
Run()
{
	// Create Textures
	Texture dot(sdlRenderer);
	dot.Load("normal.bmp");

	Texture smallDot(sdlRenderer);
	smallDot.Load("small.bmp");

	// Create Points
	int halfScreenWidth = SCREEN_WIDTH / 2;
	int halfScreenHeight = SCREEN_HEIGHT / 2;

	Point P1(Vector2(halfScreenWidth - (ROD_LENGTH/2), halfScreenHeight), &dot);
	Point P2(Vector2(halfScreenWidth + (ROD_LENGTH/2), halfScreenHeight), &smallDot);
	Point P3(Vector2(halfScreenWidth, halfScreenHeight + ROD_LENGTH * 0.5 * 1.73205080756887729), &smallDot);

	// Create SoftGeometry
	SoftGeometry rod(sdlRenderer, &P1, &P2, &P3);

	// Define click rect (display purposes only)
	SDL_Rect clickArea = {MIN_X, MIN_Y, SCREEN_WIDTH, SCREEN_HEIGHT	};

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

		elapsedTime += deltaTime * timeScale;
		
		for (; elapsedTime >= fixedDeltaTime; elapsedTime -= fixedDeltaTime)
		{
			// Update Rod
			rod.update(fixedDeltaTime * timeScale, Vector2::Down * gravity * 1000, conservation, elasticity, elasticConservation);
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
					auto x = e.button.x;
					auto y = e.button.y;
					if (x > MIN_X && y > MIN_Y && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
					{
						Vector2 mousePosition(x, y);
						if (Vector2::SqrDistance(mousePosition, P1.getPosition()) < 100)
							dragging = true;
						else
							clicking = true;
					}
				}
				break;
			case SDL_MOUSEMOTION:
			{
				clicking = false;

				if (dragging)
				{
					auto x = e.motion.x;
					auto y = e.motion.y;
					if (x > MIN_X && y > MIN_Y && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
					{
						rod.setTarget(Vector2(x, y));
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (clicking)
					{
						auto x = e.button.x;
						auto y = e.button.y;
						if (x > MIN_X && y > MIN_Y && x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
						{
							rod.setTarget(Vector2(x, y));
						}
					}
					clicking = false;
					dragging = false;
				}
				break;
			}

			// Now that we are done polling and using SDL events we pass
			// the leftovers to the SDLInput object to later be handled by
			// the Gui. 
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

		SDL_SetRenderDrawColor(sdlRenderer, 0xCE, 0xCE, 0xCE, 0xFF);
		SDL_RenderFillRect(sdlRenderer, &clickArea);

		// Render Rods
		rod.render();

		//Update renderer
		SDL_RenderPresent(sdlRenderer);
	}

	return 0;
}