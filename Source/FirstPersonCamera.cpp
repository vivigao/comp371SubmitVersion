// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>
using namespace glm;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position) :  Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(2.5f)
{}

FirstPersonCamera::~FirstPersonCamera(){}

void FirstPersonCamera::Update(float dt){
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	mat4 transform(1.0f);
	transform = targetModel -> GetWorldMatrix();
	vec4 modelPosition = transform *  vec4(targetModel->GetCurrentPosition(),1.0f);
	vec3 position = vec3( modelPosition[0],modelPosition[1], modelPosition[2]);

	vec4 nextModelPosition = transform * vec4(targetModel->GetNextPosition(),1.0f);
	vec3 next = vec3( nextModelPosition[0],(nextModelPosition[1]), nextModelPosition[2]);
	mPosition = vec3( (modelPosition[0]),(modelPosition[1] + 10.0f), modelPosition[2]);


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
		mHorizontalAngle -= 360;
	else if (mHorizontalAngle < -360)
		mHorizontalAngle += 360;

	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);
	//vec4 l = transform * vec4 (targetModel->GetNextPosition(),1.0f);
	
	mLookAt =  normalize(position);
		//normalize(vec3 (l[0],l[1],l[2]));
		//vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		//normalize(targetModel->GetNextPosition());//

	upVector = vec3(0.0f, 1.0f, 0.0f);
		//vec3 (mLookAt[1], -mLookAt[0], mLookAt[2]);
	
	vec3 sideVector = glm::cross(mLookAt, upVector);
	glm::normalize(sideVector);

	// A S D W for motion along the camera basis vectors
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
		mPosition += mLookAt * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
		mPosition -= mLookAt * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
		mPosition += sideVector * dt * mSpeed;

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
		mPosition -= sideVector * dt * mSpeed;
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const{
	return glm::lookAt(	mPosition, mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}