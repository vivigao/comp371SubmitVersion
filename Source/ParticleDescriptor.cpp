// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
#include "ParticleDescriptor.h"
using namespace glm;

ParticleDescriptor::ParticleDescriptor(){    SetFountainDescriptor();}

/*IMPORTNAT:
If you do implement this version, please keep the old version in the project but commented out for the demo
*/
void ParticleDescriptor::SetFountainDescriptor()
{
    velocity = vec3(0.0f, 17.0f, 0.0f);
    velocityDeltaAngle = 15;
    
    acceleration = vec3(0.0f, -7.0f, 0.0f);
    
    initialSize = vec2(0.4f, 0.4f);
    initialSizeDelta = vec2(0.1f, 0.1f);
    sizeGrowthVelocity = vec2(1.4f, 1.4f);
    
    initialColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    midColor = vec4(0.9f, 0.9f, 1, 1.0f);
    endColor = vec4(0.5f, 0.6f, 1.0f, 0.0f);

    emissionRate = 50.0f;
    fadeInTime = 0.2f;
    fadeOutTime = 4.5f;
    totalLifetime = 5.0f;
    totalLifetimeDelta = 0.3f;
}

void ParticleDescriptor::SetSnowDescriptor()
{
    velocity = vec3(0.0f, 17.0f, 0.0f);
    velocityDeltaAngle = 15;
    
    acceleration = vec3(0.0f, -7.0f, 0.0f);
    
    initialSize = vec2(0.4f, 0.4f);
    initialSizeDelta = vec2(0.1f, 0.1f);
    sizeGrowthVelocity = vec2(1.4f, 1.4f);
    
    initialColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    midColor = vec4(0.9f, 0.9f, 1, 1.0f);
    endColor = vec4(0.5f, 0.6f, 1.0f, 0.0f);

    emissionRate = 50.0f;
    fadeInTime = 0.2f;
    fadeOutTime = 4.5f;
    totalLifetime = 5.0f;
    totalLifetimeDelta = 0.3f;
}