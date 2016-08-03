#include "Node.h"

#include "Common/Extensions.h"

Node::
Node(const Vector2& position, Texture* texture)
: position(position),
  texture(texture)
{

}

Node::
~Node()
{

}

void
Node::
update(float deltaTime)
{

}

void
Node::
render()
{
	float rx = position.x - (texture->getWidth() / 2.0);
	float ry = position.y - (texture->getHeight() / 2.0);
	texture->render(rx, ry);
}


