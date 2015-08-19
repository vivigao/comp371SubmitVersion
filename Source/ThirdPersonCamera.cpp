// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.

#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "Animation.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 position) :  Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(2.5f)
{}

ThirdPersonCamera::~ThirdPersonCamera()
{}

void ThirdPersonCamera::Update(float dt){
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();


	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	// Mouse motion to get the variation in angle
	mHorizontalAngle -= EventManager::GetMouseMotionX() * mAngularSpeed * dt;
	mVerticalAngle   -= EventManager::GetMouseMotionY() * mAngularSpeed * dt;

	// Clamp vertical angle to [-85, 85] degrees
	mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
	if (mHorizontalAngle > 360)
	{
		mHorizontalAngle -= 360;
	}
	else if (mHorizontalAngle < -360)
	{
		mHorizontalAngle += 360;
	}

	float theta = radians(mHorizontalAngle); 
	float phi = radians(mVerticalAngle);

	// targetModlePosition = mTargetModel->GetPosition();
	vec3 targetModlePosition =mTargetModel->GetCurrentPosition();

		mat4 transform(1.0f);
	transform = mTargetModel -> GetWorldMatrix();
	vec4 modelPosition = transform * vec4(mTargetModel->GetCurrentPosition(),1.0f);
	targetModlePosition = vec3( modelPosition[0],(modelPosition[1]+75.0f), modelPosition[2]);

	mLookAt = normalize(
		mTargetModel->GetCurrentPosition());
	// vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
	
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);

	// A S D W for motion along the camera basis vectors
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
		targetModlePosition += mLookAt * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
		targetModlePosition -= mLookAt * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
		targetModlePosition += sideVector * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
		targetModlePosition -= sideVector * dt * mSpeed;
	
	 mTargetModel->SetPosition(targetModlePosition);
	 mPosition = targetModlePosition- mLookAt*5.0f;
} 

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{return glm::lookAt(	mPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) ); 
//vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
	//return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}

void ThirdPersonCamera::SetTargetModel(Model* m){	mTargetModel = m;}