#ifndef TextRenderer_cpp
#define TextRenderer_cpp

#include <SDL.h>
#include <SDL_image.h>
#include <string>
// #include <iostream>

enum Align {
	CENTER = 0,
	LEFT = 1,
	UP = 2,
	RIGHT = 4,
	BOTTOM = 8,
	UPPER_LEFT = 3,
	UPPER_RIGHT = 6,
	LOWER_LEFT = 9,
	LOWER_RIGHT = 12
};

static SDL_Texture *ASCII = NULL;
static int letterHeight = 16, letterWidth = 8;

class TextRenderer
{
  public:

	static void renderText(SDL_Renderer * renderer, float xTemp, float yTemp,
						   const std::string & textTemp, float scale = 1.0f, Align align = LEFT)
	{
		if (!(NULL == ASCII))
		{
			unsigned int length = textTemp.length();
			float charWidth = (float)letterWidth * scale;
			float charHeight = (float)letterHeight * scale;

			for (unsigned int i = 0; i < length; i++)
			{
				int _x, _y;
				char l = textTemp[i];
				  _x = (l % 16);
				  _y = (l / 16) + 1;
				  SDL_Rect window =
				  { xTemp + (letterWidth * scale) * i, yTemp, (letterWidth * scale),
				  (letterHeight * scale)
				};
				SDL_Rect letter =
					{ _x * letterWidth, _y * letterHeight, letterWidth, letterHeight };
				SDL_RenderCopy(renderer, ASCII, &letter, &window);
			}
		}
	}

	static void renderBanner(SDL_Renderer * renderer, float xTemp, float yTemp,
							 const std::string & textTemp, float xMar = 10.0f, float yMar = 10.0f, float scale = 1.0f, Align align = CENTER)
	{
		SDL_Rect r =
			{ xTemp - xMar, yTemp - yMar, scale * letterWidth * textTemp.length() + 2.0f * xMar,
			scale * letterHeight + 2 * yMar
		};
		SDL_RenderFillRect(renderer, &r);
		renderText(renderer,xTemp,yTemp,textTemp);
	}

	static int init(SDL_Renderer * renderer)
	{
		SDL_Surface *temp = IMG_Load("img/ascii.png");

		if (NULL == temp)
		{
			return 1;
		}
		else
			ASCII = SDL_CreateTextureFromSurface(renderer, temp);

		SDL_FreeSurface(temp);
		return 0;
	}

	static void close()
	{
		SDL_DestroyTexture(ASCII);
	}

};

#endif
