#include "SoftGeometry.h"
#include "Common/Extensions.h"

SoftGeometry::
SoftGeometry(SDL_Renderer* renderer, Point* V1, Point* V2, Point* V3)
: renderer(renderer),
  V1(V1),
  V2(V2),
  V3(V3),
  length12((V1->getPosition() - V2->getPosition()).getMagnitude()),
  length13((V1->getPosition() - V3->getPosition()).getMagnitude()),
  length23((V2->getPosition() - V3->getPosition()).getMagnitude()),
  target(V1 ? V1->getPosition() : Vector2::Zero)
{

}

SoftGeometry::
~SoftGeometry()
{
}

void 
SoftGeometry::
setTarget(const Vector2& target)
{
	this->target = target;
}

Vector2
SoftGeometry::
getTarget() const
{
	return target;
}


void
SoftGeometry::
update(float deltaTime, const Vector2& gravity, float conservation, float elasticity, float elasticConservation)
{
	if (V1)
	{
		elasticity = std::clamp<float>(elasticity, 0.0, 1.0);

		if (elasticity <= 0)
			V1->setPosition(target);
		else
		{
			auto displacement = V1->getPosition() - target;
			auto springForce = displacement * -elasticity * 500;
			V1->update(deltaTime, springForce, elasticConservation);
		}

		if (V2)
		{
			V2->update(deltaTime, gravity, conservation);

			// V1 - V2
			Vector2 delta = V2->getPosition() - V1->getPosition();
			float deltaLength = delta.getMagnitude();
			float ratio = (deltaLength - length12) / deltaLength;
			
			V2->move(delta * ratio * -1 * elasticity);
		}

		
		if (V3)
		{

			V3->update(deltaTime, gravity, conservation);

			// V2 - V3
			if (V2)
			{
				Vector2 delta = V3->getPosition() - V2->getPosition();
				float deltaLength = delta.getMagnitude();
				float ratio = (deltaLength - length23) / deltaLength;

				V2->move(delta * ratio * 0.5 * elasticity);
				V3->move(delta * ratio * -0.5 * elasticity);
			}
			// V1 - V3
			{
				Vector2 delta = V3->getPosition() - V1->getPosition();
				float deltaLength = delta.getMagnitude();
				float ratio = (deltaLength - length13) / deltaLength;

				V3->move(delta * ratio * -1 * elasticity);
			}
		}
	}
}

void
SoftGeometry::
render()
{

	const Vector2& p1 = V1->getPosition();
	const Vector2& p2 = V2->getPosition();
	const Vector2& p3 = V3->getPosition();

	SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
	SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p3.x, p3.y);

	V1->render();
	V2->render();
	V3->render();
}
