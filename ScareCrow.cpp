#ifndef SCARECROW_cpp
#define SCARECROW_cpp

#include "SpriteRenderer.cpp"
#include "BOID.cpp"

enum SCARE_ANIMATION
{
	APPEAR = 0,
	BE = 1,
	DISAPPEAR = 2,
	INVISIBRU = 3
};

class Scarecrow : public Vec2D {
	int animType = INVISIBRU;
	float animFrame = 1.1f,
		animSpeed = .0f;
	float xspd = .0f;
	FLOCK* flock;
	Sprite* SPR_SCARECROW = NULL;

public:
	void init(FLOCK* flock_ptr)
	{
		SPR_SCARECROW = SpriteRenderer::getSprite("SCARE");
		flock = flock_ptr;
	}

	void switchMode(int type, double startFrame, double speed)
	{
		animType = type;
		animFrame = startFrame;
		animSpeed = speed;

		//std::cout << "GUI - AnimState change to " << type << std::endl;
	}

	void update(Vec2D mouse, float deltaTime)
	{
		animFrame += animSpeed * deltaTime;
		if (animFrame > 1.0f)
		{
			
			switch (animType)
			{
				case APPEAR: {
					switchMode(BE, 1.1f, .0f);
					break;
				}
				case DISAPPEAR: {
					switchMode(INVISIBRU, 1.1f, .0f);
					break;
				}
				case BE: {
					if (!(flock->getMouseRepel()))
					{
						switchMode(DISAPPEAR, .0f, .1f);
						xspd = -360;
					}
					break;
				}
				case INVISIBRU: {
					if (flock->getMouseRepel())
					{
						switchMode(APPEAR, .0f, .1f);
						xspd = 360;
					}
					break;
				}
				default: {
					
					break;
				}
			}
		}

		float prevX = x, mx = 45.0f;
		set(mouse);
		xspd += (x - prevX) * deltaTime;

		xspd *= .9f;
	}

	void render(SDL_Renderer* renderer)
	{
		switch (animType)
		{
			case APPEAR:
			{
				SpriteRenderer::renderEx(renderer, SPR_SCARECROW, x, y, animFrame, 90.0f + xspd, animFrame, animFrame);
				break;
			}
			case BE:
			{
				SpriteRenderer::renderEx(renderer, SPR_SCARECROW, x, y, animFrame, 90.0f + xspd, 1.0f, 1.0f);
				break;
			}
			case DISAPPEAR:
			{
				SpriteRenderer::renderEx(renderer, SPR_SCARECROW, x, y, animFrame, 90.0f + xspd, 1.0f - animFrame, 1.0f - animFrame);
				break;
			}
			case INVISIBRU:
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_FRect r = { x - 1,y - 1,3,3 };
				SDL_RenderFillRectF(renderer, &r);
				break;
			}
		}
	}



};


#endif
