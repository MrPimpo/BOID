// Simple Sledge game about evading trees on your way down the hill

#ifndef Engine_cpp
#define Engine_cpp

class Engine;

// below comment has to be here for Android tests
// #include "SDL2/SDL.h"
#include <iostream>
#include <memory>
#include <list>
#include "SDL.h"
#include "SDL_image.h"
#include "GUI.cpp"
#include "Rect.cpp"
#include "BOID.cpp"
#include "Vec2D.cpp"
#include "Scarecrow.cpp"
#include "TextRenderer.cpp"
#include "SpriteRenderer.cpp"

// 1920x1080 || 640x360
static Rect SCREEN = { 0,0,1920,1280 };
static float GAME_SCALE = 1.0f;

class Engine
{
private:
	bool running = false,
		quitFlag = false,
		debug = false,
		fullscreen = false;
	int steps = 0,
		ticks = 0,
		fps = 0;
	int mouse_x = 0,
		mouse_y = 0;
	int gamestate = 1;
	int lastTick = 0;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	GUI gui;
	Vec2D camera;
	FLOCK flock;
	Scarecrow scarecrow;

public:
	bool init()
	{
		if ((SDL_Init(SDL_INIT_EVERYTHING) < 0))
		{
			SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
			std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return true;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			SDL_Log("SDL_image could not initialize! SDL_Error: %s", SDL_GetError());
			std::cout << "SDL_image could not initialize! SDL_Error: " << SDL_GetError() <<
				std::endl;
			return true;
		}

		window =
			SDL_CreateWindow("BOID", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				SCREEN.w, SCREEN.h, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
			return true;
		}

		renderer =
			SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
				SDL_RENDERER_TARGETTEXTURE);
		if (renderer == NULL)
		{
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return true;
		}

		SDL_RenderSetLogicalSize(renderer, (int)(SCREEN.w), (int)(SCREEN.h));
		SDL_ShowCursor(SDL_DISABLE);

		if (TextRenderer::init(renderer) > 0)
			std::cout << "Text renderer: Failed not load \"font.png\". " << SDL_GetError() <<
			std::endl;
		else
			std::cout << "Text renderer: Initialized. " << std::endl;

		if (SpriteRenderer::init(renderer) > 0)
			std::cout << "Could not initialize sprite handler. " << SDL_GetError() << std::endl;

		gui.init(&SCREEN);
		flock.init(&SCREEN, 300, false);
		scarecrow.init(&flock);

		srand((int)time(NULL));
		running = true;
		ticks = SDL_GetTicks();

		SDL_Log("SDL initialized properly");

		std::cout << "Width: " << SCREEN.w << ", height: " << SCREEN.h << std::endl;

		if (fullscreen)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

		return false;
	}

	bool close()
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();

		return false;
	}

	void quit()
	{
		running = false;
	}

	void update()
	{
		int t = SDL_GetTicks();
		float deltaTime = ((float)t - (float)lastTick) / 60;
		lastTick = t;

		gui.update(deltaTime);
		scarecrow.update({ (float)mouse_x, (float)mouse_y }, deltaTime);
		flock.update(scarecrow, deltaTime);

		steps++;
		t = SDL_GetTicks() / 1000;
		if (t > ticks)
		{
			fps = steps;
			ticks = t;
			steps = 0;
		}

		if (quitFlag)
			quit();
	}

	void render()
	{
		SDL_SetRenderDrawColor(renderer, 128, 128, 196, 255);
		SDL_RenderClear(renderer);

		flock.render(renderer);
		scarecrow.render(renderer);

		if (debug)
		{
			SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
			std::string s = "Mouse ( x:";
			//s.append(std::to_string(camera.getDistance(player->getX(),player->getY())));
			s.append(std::to_string(mouse_x));
			s.append("; y:");
			s.append(std::to_string(mouse_y));
			s.append(")");
			TextRenderer::renderBanner(renderer, ((float)SCREEN.w * .8), 280, s);

			s = "fps: ";
			s.append(std::to_string(fps));
			TextRenderer::renderBanner(renderer, (float)SCREEN.w - 200, 30, s);
		}

		gui.render(renderer);
		//SpriteRenderer::renderEx(renderer, SPR_RED_BUTTON, SCREEN.w - (16.0f * GAME_SCALE), 16.0f * GAME_SCALE, 0, .0f, GAME_SCALE, GAME_SCALE);

		SDL_RenderPresent(renderer);
	}

	void event()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit();
				break;
			case SDL_FINGERDOWN: {
				float x = e.tfinger.x, y = e.tfinger.y;
				mouse_x = (int)(x * (float)SCREEN.w);
				mouse_y = (int)(y * (float)SCREEN.h);
				interact(mouse_x, mouse_y);
				flock.mouseRepelSwitch(true);
			}
				break;
			case SDL_FINGERUP:
				flock.mouseRepelSwitch(false);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quitFlag = true;
					break;
				case SDLK_r:

					break;
				default:
					if (gamestate == 1)
						interact(SCREEN.w / 2, SCREEN.h / 2);
					break;
				}
				break;
			case SDL_KEYUP:
				break;
			case SDL_MOUSEBUTTONDOWN:
				interact(mouse_x, mouse_y);
				flock.mouseRepelSwitch();
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEMOTION:
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;
				break;
			}
		}
	}

	void interact(int x, int y) {

	}

	inline bool isRunning()
	{
		return running;
	}

	inline SDL_Renderer* getRenderer()
	{
		return renderer;
	}
};

static Engine* ENGINE;

int main(int argc, char* argv[])
{

	ENGINE = new Engine();

	if (ENGINE->init() == false)
	{
		while (ENGINE->isRunning())
		{
			ENGINE->event();
			ENGINE->update();
			ENGINE->render();
		}
	}

	ENGINE->close();
	delete ENGINE;

	return 0;
}

#endif
