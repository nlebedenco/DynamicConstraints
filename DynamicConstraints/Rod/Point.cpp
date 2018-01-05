#include "Point.h"

#include "Common/Extensions.h"

Point::
Point(const Vector2& position, Texture* textureNormal, Texture* textureSelected)
: textureNormal(textureNormal),
  textureSelected(textureSelected),
  position(position),
  previousPosition(position),
  selected(false),
  grabbed(false)
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
update(float deltaTime, float gravity, float conservation)
{
	setPosition(position + (position * conservation) - (previousPosition * conservation) + (Vector2::Down * gravity * deltaTime * deltaTime));
}

void
Point::
render()
{
	Texture* texture = selected ? textureSelected : textureNormal;

	float rx = position.x - (texture->getWidth() / 2.0);
	float ry = position.y - (texture->getHeight() / 2.0);
	texture->render(rx, ry);
}
