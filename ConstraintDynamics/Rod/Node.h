#pragma once

#include "Common/Texture.h"
#include "Common/Vector2.h"

class Node
{
private: 
    Texture* texture;
	Vector2 position;

public:

	Node(const Vector2& position, Texture* texture);
    ~Node();

    void update(float deltaTime);
	void render();

};

