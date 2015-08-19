#pragma once

#include "ParsingHelper.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>

class Light{
public:
	Light(void);
	virtual ~Light(void);

	ci_string GetLightName() { return lName; };

	// Expressed as (r, g, b)
	glm::vec3 GetLightColor() { return lColor; };

	// Expressed as (kc, kl, kq)
	glm::vec3 GetLightCoefficients() { return lCoefficients; };

	// Expressed as (x, y, z, w), wherein w=1 -> point Light, w=0 -> directional Light
	glm::vec4 GetLightPosition() { return lPosition; };

	virtual void SetLightPosition(float x, float y, float z) = 0;

	void Load(ci_istringstream& iss);

//	void Enlighten(unsigned int lightCount = 0);

protected:
	bool ParseLine(const std::vector<ci_string> &token);

	ci_string lName;
	glm::vec3 lColor;
	glm::vec3 lCoefficients;
	glm::vec4 lPosition;
};

class PointLight : public Light
{
public:
	PointLight::PointLight(){};
	PointLight::PointLight(glm::vec3 position){
		SetLightPosition(
			position.x,
			position.y,
			position.z
			);
	};

	PointLight::~PointLight(void){}

	void PointLight::SetLightPosition(float x, float y, float z){
		lPosition.x = x;
		lPosition.y = y;
		lPosition.z = z;
		lPosition.w = 1.f;
	};
};