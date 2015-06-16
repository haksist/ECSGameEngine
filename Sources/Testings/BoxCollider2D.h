#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "../CoreEngine/ChmoEngine.h"
#include "Collider2D.h"

class BoxCollider2D : public Collider2D
{
public:
	Vector2 center;
	Vector2 size;
	virtual void Update();
	virtual bool HasCollision(Collider2D* other);
};

#endif