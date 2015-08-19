
#pragma once

#include "ParsingHelper.h"
#include "Model.h"

#include <vector>

#include <GLM/glm.hpp>

class Animation;
class ParticleSystem;
class Model;

class Collision
{
public:
	Collision();
	virtual ~Collision();
	Model *mModel;
	float levelPosition;
	bool reachLevel(glm::vec3 p);
	bool modelMeet(Model *m1, Model *m2);


protected:
	//virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

    // Makes the model follow a list of Animation Keys so it's world transform changes over time
    Animation *mAnimation;
    ParticleSystem *mParticleSystem;
    
	friend class Animation;
};
