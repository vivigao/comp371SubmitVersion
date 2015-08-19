//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Collision.h"
#include "Animation.h"
#include "World.h"
#include "ParticleEmitter.h"
#include "ParticleDescriptor.h"
#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

Collision::Collision() 
: mModel(), levelPosition(0.0f), mAnimation(nullptr), mParticleSystem(nullptr)
{ 
}

Collision::~Collision()
{
}

bool Collision::reachLevel(vec3 p){

	
	if (p.x == levelPosition)
	{
	return true;
	}
	else 
		return false;
	
}

bool Collision::modelMeet(Model *m1, Model *m2){
 vec3 mp1 = m1->GetCurrentPosition();
 vec3 mp2 = m2->GetCurrentPosition();
	
	if ((mp1.x == mp2.x) &&(mp1.y == mp2.y) &&(mp1.z == mp2.z))
	{
	return true;
	}
	else 
		return false;
	
}



