#ifndef GUI_cpp
#define GUI_cpp

#include "SpriteRenderer.cpp"
#include "Rect.cpp"
#include <iostream>

enum animations : int {
	ANIM_NONE = -1,
	ANIM_DARKEN = 0,
	ANIM_DARKEN_REVERSED = 1,
	ANIM_PLAIN_DARK = 2
};

class AnimationGUI
{
	int animType = ANIM_PLAIN_DARK;
	float animFrame = .0f;
	float animSpeed = .05f;
	Rect* SCREEN = NULL;

public:
	void init(Rect* screen)
	{
		this->SCREEN = screen;
	}

	void switchMode(int type, double startFrame, double speed)
	{
		animType = type;
		animFrame = startFrame;
		animSpeed = speed;

		std::cout << "GUI - AnimState change to " << type << std::endl;
	}

	void update(float deltaTime)
	{
		animFrame += animSpeed * deltaTime;
		//std::cout << "GUI - update. F: " << animFrame << "; S: " << animSpeed << std::endl;
		if (animFrame > 1.0f)
		{
			switch (animType)
			{
			case ANIM_DARKEN: {
				switchMode(ANIM_PLAIN_DARK, .0f, .1f);
				break;
			}
			case ANIM_PLAIN_DARK: {
				switchMode(ANIM_DARKEN_REVERSED, .0f, .1f);
				break;
			}
								/*case ANIM_DARKEN_REVERSED: {
									switchMode(ANIM_DARKEN, .0f, .1f);
									break;
									}*/
			default: {
				switchMode(ANIM_NONE, .0f, .0f);
				break;
			}
			}
		}
	}

	void render(SDL_Renderer* renderer)
	{
		switch (animType)
		{
		case ANIM_DARKEN: {
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, (animFrame * 255.0f));
			SDL_Rect r = SCREEN->getSDL_Rect();
			SDL_RenderFillRect(renderer, &r);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
			break;
		}
		case ANIM_DARKEN_REVERSED: {
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, (255.0f - (animFrame * 255.0f)));
			SDL_Rect r = SCREEN->getSDL_Rect();
			SDL_RenderFillRect(renderer, &r);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
			break;
		}
		case ANIM_PLAIN_DARK: {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Rect r = SCREEN->getSDL_Rect();
			SDL_RenderFillRect(renderer, &r);
			break;
		}
		}
	}

};

class GUI
{
private:
	Rect* SCREEN = NULL;
	AnimationGUI top, btm;
	
public:
	void init(Rect *screen)
	{
		this->SCREEN = screen;

		top.init(screen);
		btm.init(screen);

		top.switchMode(ANIM_PLAIN_DARK, .0f, .2);
		btm.switchMode(ANIM_PLAIN_DARK, .0f, .1);
		std::cout <<std::endl << "GUI - initialized. " << std::endl;
	}

	void update(float deltaTime)
	{
		top.update(deltaTime);
		btm.update(deltaTime);
	}

	void render(SDL_Renderer* renderer)
	{
		//btm.render(renderer);
	//	top.render(renderer);
	}

	void renderDebug(SDL_Renderer* renderer) 
	{

	}
};

#endif
