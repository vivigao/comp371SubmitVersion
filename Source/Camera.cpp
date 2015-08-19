//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Camera.h"
#include <GLM/gtx/transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

Camera::Camera(){}

Camera::~Camera(){}

mat4 Camera::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

mat4 Camera::GetProjectionMatrix() const
{
  return perspective(60.0f, 4.0f / 3.0f, 0.1f, 500.0f);
}
