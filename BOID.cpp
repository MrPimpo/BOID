#ifndef BOID_cpp
#define BOID_cpp

#include "SpriteRenderer.cpp"
#include "TextRenderer.cpp"
#include <iostream>
#include <list>
#include "Rect.cpp"
#include <time.h>
#include <random>

class BOID
{
public:
	Vec2D position, velocity;
	float anim = .0f;
	int ID;
	float scale = .5f;

	BOID(Vec2D _position, Vec2D _velocity, float _scale, int id)
	{
		this->position.set(_position);
		this->velocity.set(_velocity);
		this->scale = _scale;
		this->ID = id;
	}	
};

class FLOCK {
	Rect* SCREEN;
	float sight_range;
	float repel_range;
	float treshold;
	float flock_count;
	std::list<BOID*> flock;
	// this "queue" is used to increment BOID's animation states in batches ( so they appear to flap their wings separately )
	int sprite_queue_length = 25,
		sprite_queue = 0;
	Sprite* SPR_BOID = NULL;
	bool mouseRepel = false;

public:
	void init(Rect* screen, int instances_count, bool mouse_repel = false)
	{
		SPR_BOID = SpriteRenderer::getSprite("CROWX");
		SCREEN = screen;
		mouseRepel = mouse_repel;
		flock_count = instances_count;
		treshold = 250.0f;
		sight_range = 300.0f;
		repel_range = 30.0f;
		for (float i = .0f; i < instances_count; i+=1.0f)
		{
			flock.push_back(new BOID({ (float)(rand() % SCREEN->w),(float)(rand() % SCREEN->h) }, {0,0 },(40+rand()%20)*.01f, i));
		}
	}

	void rule123(BOID &BD, float deltaTime)
	{
		//	'p' is used to calculate a mean of neighbours position,
		//	'r' is used to store velocity repeling current boid from neighbours that are too close
		//	and 'v' is used to calculate a mean of neighbours velocity
		
		Vec2D v, p, r;
		int n = 0.0f;

		for (auto& b : flock)
		{
			if (b->ID != BD.ID)
			{
				float dist = BD.position.getDistance(b->position);
				if (dist < sight_range)
				{
					v += b->velocity;
					p += b->position;
					n += 1.0f;
				}
				if (dist < repel_range)
				{
					r += (BD.position - b->position)*.01;
				}
			}
		}

		p = ((p / n) - BD.position) * .0002f;
		v =  ((v / n)) * .08f;
		BD.velocity += (v+p+r) * deltaTime;
	}

	void update(Vec2D mouse, float deltaTime)
	{
		int i = 0;
		if (mouseRepel)
		{
			for (auto& b : flock)
			{
				float dist = b->position.getDistance(mouse);
				if (dist < sight_range)
				{
					b->velocity += (b->position - mouse) * ((sight_range - dist) / sight_range) * .01 * deltaTime;
				}
			}
		}

		for (auto& b : flock)
		{
			if (sprite_queue == (i / sprite_queue_length))
				b->anim += b->velocity.magnitude() * (((int)(b->anim) % 6) == 0 ? .01f : .5f) * deltaTime;
			rule123(*b, deltaTime);

			float mod3 = 1.0f;

			if (b->position.x < treshold)
				b->velocity.x += mod3 * deltaTime;
			if (b->position.x > (SCREEN->w - treshold))
				b->velocity.x -= mod3 * deltaTime;

			if (b->position.y < treshold)
				b->velocity.y += mod3 * deltaTime;
			if (b->position.y > (SCREEN->h - treshold))
				b->velocity.y -= mod3 * deltaTime;

			Vec2D normalized = b->velocity.normalize();

			/*
			// better to "trim" velocity to its max value
			float drag = .02;
			if (b->velocity.magnitude() > drag * deltaTime)
				b->velocity -= normalized * drag * deltaTime;
			*/

			float max_velocity = 20.f * b->scale;
			if (b->velocity.magnitude() > max_velocity)
				b->velocity = normalized * max_velocity;

			b->position += b->velocity * deltaTime;
			i++;
		}
		sprite_queue++;
		if (sprite_queue * sprite_queue_length > flock_count)
			sprite_queue = 0;
	}

	void render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
		for (auto& b : flock)
			SpriteRenderer::renderEx(renderer, SPR_BOID, b->position.x, b->position.y, b->anim,
				b->velocity.getAngle()+90.0f, b->scale*(b->velocity.x<0?1.0f:-1.0f), b->scale);
	}

	void renderDebug(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
		for (auto& b : flock)
		{
			SDL_FRect r = {b->position.x - 5.0f, b->position.y - 5.0f, 10.0f, 10.0f};
			SDL_RenderFillRectF(renderer, &r);
			r = { b->position.x - 500.0f, b->position.y - 500.0f, 1000.0f, 1000.0f };
			SDL_RenderDrawLineF(renderer, b->position.x, b->position.y, b->position.x + b->velocity.x * 3.0f, b->position.y + b->velocity.y * 3.0f);
		}

	}

	void mouseRepelSwitch(bool mouse_repel)
	{
		mouseRepel = mouse_repel;
	}

	void mouseRepelSwitch()
	{
		mouseRepel = !mouseRepel;
	}

	bool getMouseRepel()
	{
		return mouseRepel;
	}

};


#endif
