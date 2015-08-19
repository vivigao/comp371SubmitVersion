// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#pragma once

#include <vector>
using namespace std;

#include <GLM/glm.hpp>
using namespace glm;

// Simple Texture Loader Class
class ObjectLoader
{
public:
  static bool LoadObject(const char * path, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals);

private:
    
};