#pragma once

#include "Common/Texture.h"
#include "Common/Vector2.h"

class Point
{
private: 
    Texture* textureNormal;
	Texture* textureSelected;
	Vector2 position;
	Vector2 previousPosition;

public:

	bool selected;
	bool grabbed;

public:
	Point(const Vector2& position, Texture* textureNormal, Texture* textureSelected);
    ~Point();

	Vector2 getPosition();

	void setPosition(const Vector2& value);

	void move(const Vector2& offset);

	void update(float deltaTime, float gravity = 9.8, float conservation = 1.0);

	void render();
};

