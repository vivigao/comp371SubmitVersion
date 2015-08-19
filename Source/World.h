// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#pragma once
#include "ParsingHelper.h"
#include "Billboard.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include <vector>

class Camera;
class Model;
class WaterModel;
class ObjectModel;
class Animation;
class AnimationKey;
class ParticleSystem;
class BSpline;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
	AnimationKey* FindAnimationKey(ci_string keyName);

    const Camera* GetCurrentCamera() const;
    void AddBillboard(Billboard* b);
	void AddAnimationKey(AnimationKey* ak);
	void AddAnimation(Animation* a);
    void RemoveBillboard(Billboard* b);
    void AddParticleSystem(ParticleSystem* particleSystem);
    void RemoveParticleSystem(ParticleSystem* particleSystem);
    
	CubeModel *ourGuy;
	SphereModel *ourSphere;

private:
  static World* instance;
  
  std::vector<Model*> mModel;
  WaterModel* mWater;
  ObjectModel* mMap;
  std::vector<Animation*> mAnimation;
  std::vector<AnimationKey*> mAnimationKey;
  std::vector<Camera*> mCamera;
  std::vector<ParticleSystem*> mParticleSystemList;
  std::vector<BSpline*> mBSpline;
  unsigned int mCurrentCamera;
  
  BillboardList* mpBillboardList;
};