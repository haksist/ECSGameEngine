#ifndef ENEMY_H
#define ENEMY_H

#include "EngineInclude.h"
#include "SimplePool.h"

class Collider2D;
class Enemy : public GameLogic
{
	friend class EnemyManager;
public:
	bool isUsed = false;
	bool isSeperated = false;
	float speed = 0.5f;

	virtual void Init();
	virtual void Update();

	void setDirection(Vector3 dir);
private:
	Transform* transform_;
	Vector3 pos_;
	Vector3 camPos_;
};
#endif