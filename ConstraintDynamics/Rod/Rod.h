#pragma once

#include "Point.h"

class Rod
{
	SDL_Renderer* renderer;
	Point* a;
	Point* b;	
	float length;
	float dragPointCorrection;
	float endPointCorrection;

public:
	Rod(SDL_Renderer* renderer, Point* a, Point* b);

	~Rod();

	Point* getPointAt(const Vector2& position, float radius = 5);

	void update(float deltaTime, float gravity = 9.8, float conservation = 1.0);

	void render();
};

