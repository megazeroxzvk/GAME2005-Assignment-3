#include "BulletPool.h"
#include <iostream>
#include "CollisionManager.h"


BulletPool::BulletPool()
{
	SoundManager::Instance().load("../Assets/audio/explosion.wav", "explosion", SOUND_SFX);
}

BulletPool::BulletPool(int size)
{
	createPool(size);
	this->size = size;
	SoundManager::Instance().load("../Assets/audio/explosion.wav", "explosion", SOUND_SFX);
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

// bullet resize trial.. hopefully it works..
// it does!
void BulletPool::bulletPoolResize(int new_size)
{
	
	if(new_size > size)			// increasing bullet capacity
	{
		for(int i = size; i < new_size ; i++)
		{
			Bullet* bullet = new Bullet();
			if(inactiveBullets.size() > 0)
			{
				bullet->getRigidBody()->velocity = inactiveBullets[0]->getRigidBody()->velocity;
				bullet->getRigidBody()->acceleration = inactiveBullets[0]->getRigidBody()->acceleration;
			}
			else if (activeBullets.size() > 0)
			{
				bullet->getRigidBody()->velocity = activeBullets[0]->getRigidBody()->velocity;
				bullet->getRigidBody()->acceleration = activeBullets[0]->getRigidBody()->acceleration;
			}
			allBullets.push_back(bullet);
			inactiveBullets.push_back(bullet);
			bullet->setBulletNumber(i);
		}
		size = new_size;
	}
	else if(new_size < size)	// decreasing bullet capacity
	{
		int difference = size - new_size;

		// check inactive size first
		if(inactiveBullets.size() > difference)
		{
			for (int i = 0; i < difference; i++)
			{
				inactiveBullets.pop_back();
			}
			size = new_size;
		}
		else
		{
			// prioritise to keep active ones stable, and remove as little active bullets as possible
			for (int i = 0; i < inactiveBullets.size(); i++)
			{
				inactiveBullets.pop_back();
				difference--;
			}

			for(int i = 0; i < difference; i++)
			{
				activeBullets.pop_back();
			}
			size = new_size;
		}
	}

	inactiveBullets.shrink_to_fit();
	activeBullets.shrink_to_fit();
	allBullets.shrink_to_fit();
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
			SoundManager::Instance().playSound("explosion", 0, 1);
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
