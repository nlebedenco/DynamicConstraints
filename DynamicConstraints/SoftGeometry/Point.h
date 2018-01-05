#pragma once

#include "Common/Texture.h"
#include "Common/Vector2.h"

class Point
{
private: 
    Texture* texture;
	Vector2 position;
	Vector2 previousPosition;

public:
	Point(const Vector2& position, Texture* texture);
    ~Point();

	Vector2 getPosition();

	void setPosition(const Vector2& value);

	void move(const Vector2& offset);

	void update(float deltaTime, const Vector2& force = Vector2::Zero, float conservation = 1.0);

	void render();
};

