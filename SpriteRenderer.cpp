#ifndef SpriteRenderer_cpp
#define SpriteRenderer_cpp

#include <SDL.h>
#include "Vec2D.cpp"
#include <SDL_image.h>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

static float* SCALE = NULL;

class Sprite
{
private:
	SDL_Texture* texture;
	std::string name;
	int imgCount = 0;
	int width = 1, height = 1;
	Vec2D origin;
public:
	Sprite(SDL_Texture *tex = NULL, std::string name = "", int imageCount = 1, float oX = 0.0f, float oY = 0.0f)
	{
		this->name = name;
		this->texture = tex;
		this->imgCount = imageCount;
		origin = { oX, oY };

		SDL_QueryTexture(tex, NULL, NULL, &width, &height);
		//width /= imgCount;
		std::cout << "Sprite created. Width: " << width << "; height: " << height;// << std::endl;
	}

	SDL_Texture *getTexture()
	{
		return texture;
	}
	int getTexWidth()
	{
		return width/imgCount;
	}
	int getTexHeight()
	{
		return height;
	}
	int getImgCount()
	{
		return imgCount;
	}

	SDL_Point getOrigin_SDL_Point(Vec2D scale = { 1.0f, 1.0f })
	{
		return { (int)(origin.x*scale.x), (int)(origin.y*scale.y) };
	}
	float getTop()
	{
		return origin.y;
	}
	float getLeft()
	{
		return origin.x;
	}
	float getBottom()
	{
		return (height - origin.y);
	}
	float getRight()
	{
		return (width - origin.x);
	}
};

static std::list<Sprite> sprites; // just to destroy all the textures at the end
static std::unordered_map<std::string, Sprite*> spriteMap;

class SpriteRenderer
{
public: 
	static int init(SDL_Renderer* renderer)
	{
		std::fstream file;
		std::string dir = "img/";
		file.open(dir + "sprite-list.txt", std::ios::in);

		if (file.good())
		{
			while (!file.eof())
			{
				std::string subdir, name;
				int imageCount, oX, oY;
				// 
				file >> imageCount >> oX >> oY >> name >> subdir;
				SDL_Surface* temp = IMG_Load((dir + subdir).c_str());
				
				if (!(NULL == temp))
				{
					SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, temp);

					Sprite* sprite = new Sprite(tex, name, imageCount, oX, oY);
					sprites.push_back(*sprite);
					spriteMap.emplace(name, sprite);

					if (NULL == tex)
						std::cout << SDL_GetError();
					else
						std::cout << std::endl << "\"" << subdir << "\" loaded. ";
				}
				else
					std::cout << "could not load \"" << subdir << "\"" << std::endl;

				SDL_FreeSurface(temp);
			}

			file.close();
		}
		else
		{
			std::cout << "could not open \"sprite-list.txt\"" << std::endl;
			return 1;
		}
		return 0;
	}

	static Sprite* getSprite(std::string name)
	{
		return (spriteMap.at(name));
	}

	static void render(SDL_Renderer* renderer, Sprite *spr, float x, float y, int imgNum = 0, float xScale = 1.0f, float yScale = 1.0f)
	{
		if (NULL != spr)
		{
			SDL_Rect w = { x - spr->getLeft() * xScale, y - spr->getTop() * yScale, spr->getTexWidth() * xScale, spr->getTexHeight() * yScale };
			imgNum = imgNum % spr->getImgCount();
			SDL_Rect s = { imgNum * spr->getTexWidth(), 0, spr->getTexWidth(), spr->getTexHeight() };
			SDL_Texture* t = spr->getTexture();
			if (NULL!=t)
				SDL_RenderCopy(renderer, t, &s, &w);
		}
	}

	static void renderEx(SDL_Renderer* renderer, Sprite* spr, float x, float y, int imgNum, float angle = 0.0f, float xScale = 1.0f, float yScale = 1.0f)
	{
		if (NULL != spr)
		{
			bool invertH = false, invertV = false;
			if (xScale < 0.0f) {
				invertH = true;
				xScale *= -1;
			}

			if (yScale < 0.0f) {
				invertV = true;
				yScale *= -1;
			}

			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (invertH && invertV)
				flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
			else if (invertH)
				flip = SDL_FLIP_HORIZONTAL;
			else if (invertV)
				flip = SDL_FLIP_VERTICAL;
		
			SDL_Rect w = { x - spr->getLeft() * xScale, y - spr->getTop() * yScale, spr->getTexWidth() * xScale, spr->getTexHeight() * yScale };
			imgNum = imgNum % spr->getImgCount();
			SDL_Rect s = { imgNum * spr->getTexWidth(), 0, spr->getTexWidth(), spr->getTexHeight() };
			SDL_Texture* t = spr->getTexture();
			Vec2D scale = (xScale, yScale);
			SDL_Point p = spr->getOrigin_SDL_Point(scale);
			if (NULL != t)
				SDL_RenderCopyEx(renderer, t, &s, &w, (angle - 90.0), &p, flip);
		}
	}

	static void close()
	{
		for (auto &s : sprites)
		{
			SDL_DestroyTexture(s.getTexture());
			//delete &s; sprites are not pointers now, so no deleting
			s = NULL;
		}
		sprites.clear();
	}
};

#endif