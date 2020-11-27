#pragma once
#ifndef __BULLETPOOL__
#define __BULLETPOOL__
#include "Bullet.h"
#include "Jet.h"

class BulletPool
{
public:
	BulletPool();
	BulletPool(int);
	~BulletPool();

	Bullet* Spawn();
	void Despawn(Bullet*);

	std::vector<Bullet*> allBullets;

	void collisionCheck(Jet*);
	void useBulletPool();
	void createPool(int);
	void destroyPool();
	void Update();
	void Draw();

	void bulletPoolResize(int new_size);
	
private:
	Bullet* m_pBullets;

	int size;
	int index = 0;
	
	std::vector<Bullet*> activeBullets;
	std::vector<Bullet*> inactiveBullets;
};

#endif /* defined (__BULLETPOOL__) */

