#pragma once

#include "Point.h"

class SoftGeometry
{
	SDL_Renderer* renderer;
	Point* V1;
	Point* V2;	
	Point* V3;

	float length12;
	float length13;
	float length23;

	Vector2 target;

public:
	SoftGeometry(SDL_Renderer* renderer, Point* V1, Point* V2, Point* V3);

	~SoftGeometry();

	void setTarget(const Vector2& target);

	Vector2 getTarget() const;

	void update(float deltaTime, const Vector2& gravity = Vector2::Zero, float conservation = 1.0, float elasticity = 0.9, float elasticConservation = 0.7);

	void render();
};

