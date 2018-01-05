#include "Point.h"

#include "Common/Extensions.h"

Point::
Point(const Vector2& position, Texture* texture)
: texture(texture),
  position(position),
  previousPosition(position)
{

}

Point::
~Point()
{

}

Vector2
Point::
getPosition()
{
	return position;
}

void
Point::
setPosition(const Vector2& value)
{
	previousPosition = position;
	position = value;
}

void
Point::
move(const Vector2& offset)
{
	position += offset;
}

void
Point::
update(float deltaTime, const Vector2& force, float conservation)
{
	setPosition(position + (position * conservation) - (previousPosition * conservation) + (force * deltaTime * deltaTime));
}

void
Point::
render()
{
	float rx = position.x - (texture->getWidth() / 2.0);
	float ry = position.y - (texture->getHeight() / 2.0);
	texture->render(rx, ry);
}
