#include "Rod.h"
#include "Common/Extensions.h"

Rod::
Rod(SDL_Renderer* renderer, Point* a, Point* b)
: renderer(renderer),
  a(a),
  b(b),
  length((a->getPosition() - b->getPosition()).getMagnitude()),
  dragPointCorrection(0),
  endPointCorrection(1)
{

}

Rod::
~Rod()
{
}

Point*
Rod::
getPointAt(const Vector2& position, float radius)
{
	if (a && Vector2::Distance(position, a->getPosition()) <= radius)
		return a;
	else if (b && Vector2::Distance(position, b->getPosition()) <= radius)
		return b;
	else 
		return NULL;
}

void
Rod::
update(float deltaTime, float gravity, float conservation)
{
	Point* dragPoint = NULL;
	Point* endPoint = NULL;

	if (a->grabbed)
	{
		dragPoint = a;
		endPoint = b;
	}
	else if (b->grabbed)
	{
		dragPoint = b;
		endPoint = a;
	}

	if (dragPoint)
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto screenWidth = DM.w;
		auto screenHeight = DM.h;
		int x, y;
		SDL_GetMouseState(&x, &y);
		x = std::clamp(x, 0, screenWidth);
		y = std::clamp(y, 0, screenHeight);

		dragPoint->setPosition(Vector2(x, y));

		if (endPoint)
		{
			endPoint->update(deltaTime, gravity, conservation);

			Vector2 delta = endPoint->getPosition() - dragPoint->getPosition();
			float deltaLength = delta.getMagnitude();
			float ratio = (deltaLength - length) / deltaLength;
			
			dragPoint->move(delta * ratio * dragPointCorrection);
			endPoint->move(delta * ratio * -endPointCorrection);
		}
	}
}

void
Rod::
render()
{

	const Vector2& posA = a->getPosition();
	const Vector2& posB = b->getPosition();

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawLine(renderer, posA.x, posA.y, posB.x, posB.y);

	a->render();
	b->render();
}
