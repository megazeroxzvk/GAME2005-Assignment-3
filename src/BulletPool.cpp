#include "BulletPool.h"
#include <iostream>
#include "CollisionManager.h"


BulletPool::BulletPool()
{
}

BulletPool::BulletPool(int size)
{
	createPool(size);
}

BulletPool::~BulletPool()
 = default;


Bullet* BulletPool::Spawn()
{
	// Spawn the bullets from the pool
	Bullet* bullet = NULL;
	if(inactiveBullets.size() > 0)
	{
		bullet = inactiveBullets.back();
		bullet->m_reset();
		bullet->active = true;
		inactiveBullets.pop_back();
		activeBullets.push_back(bullet);
		std::cout << "Bullet " << bullet->getBulletNumber() << " Spawned." << std::endl;
	}
	else
	{
		std::cout << "Bullets cannot be spawned due to reaching max capacity!" << std::endl;
	}
	return bullet;
}

void BulletPool::Despawn(Bullet* bullet)
{
	// Despawn the bullet
	bullet->m_reset();
	inactiveBullets.push_back(bullet);

	for(std::vector<Bullet*>::iterator iterationBullet = activeBullets.begin(); iterationBullet != activeBullets.end()
		; iterationBullet++)
	{
		if(*iterationBullet == bullet)
		{
			std::cout << "Bullet " << bullet->getBulletNumber() << " Despawned" << std::endl;
			activeBullets.erase(iterationBullet);
			break;
		}
	}
}

void BulletPool::Update()
{
	for (int i = 0; i < activeBullets.size(); i++)
	{
		activeBullets[i]->update();
		if (activeBullets[i]->active == false)
		{
			Despawn(activeBullets[i]);
		}
	}

	//std::cout << "Active Bullet Vector Count = " << activeBullets.size() << std::endl;
	//std::cout << "Inactive Bullet Vector Count = " << inactiveBullets.size() << std::endl;
	//std::cout << "All Bullet Vector Count = " << allBullets.size() << std::endl;
}

void BulletPool::Draw()
{
	for (int i = 0; i < activeBullets.size(); i++)
	{
		activeBullets[i]->draw();
	}
}

//--------------------

void BulletPool::collisionCheck(Jet* jet)
{
	for(int i = 0; i < activeBullets.size(); i++)
	{
		glm::vec2 point;
		point.x = activeBullets[i]->getTransform()->position.x + (activeBullets[i]->getWidth() * 0.5);
		point.y = activeBullets[i]->getTransform()->position.y + (activeBullets[i]->getHeight() * 0.5);
		//std::cout << "Width of Bullet = " << activeBullets[i]->getWidth() << std::endl;
		if ((CollisionManager::pointRectCheck(
			 point,
	{ jet->getCollisionBox1().x ,jet->getCollisionBox1().y},jet->getCollisionBox1().w,jet->getCollisionBox1().h)
			) 
			|| 
			(CollisionManager::pointRectCheck(
				point,
				{ jet->getCollisionBox2().x ,jet->getCollisionBox2().y }, jet->getCollisionBox2().w, jet->getCollisionBox2().h)
				))
		{
			// Colliding!!!!
			activeBullets[i]->active = false;
			std::cout << "Bullet " << activeBullets[i]->getBulletNumber() << " has collided!!" << std::endl;
		}

		/*if (CollisionManager::AABBCheck(activeBullets[i], jet))
		{
			std::cout << "Collision with Jet Box 1 (AABB)" << std::endl;
		}*/
	}
	
}

void BulletPool::useBulletPool()
{
	Bullet* bullet = Spawn();
}

// Create the pool
void BulletPool::createPool(int size)
{
	for (int i = 0; i < size; i++)
	{
		Bullet* bullet = new Bullet();
		allBullets.push_back(bullet);
		inactiveBullets.push_back(bullet);
		bullet->setBulletNumber(i);
	}
	std::cout << "Bullet Pool created with size = " << size << std::endl;
}

// Destroy the pool
void BulletPool::destroyPool()
{
	for(int i = 0; i < allBullets.size(); i++)
	{
		delete allBullets[i];
		allBullets[i] = NULL;
	}
	allBullets.shrink_to_fit();
	activeBullets.shrink_to_fit();
	inactiveBullets.shrink_to_fit();
	std::cout << "Size of Bullet Pool after deleting = " << allBullets.size() << std::endl;
	std::cout << "Size of active Bullet Pool after deleting = " << activeBullets.size() << std::endl;
	std::cout << "Size of inactive Bullet Pool after deleting = " << inactiveBullets.size() << std::endl;
}
