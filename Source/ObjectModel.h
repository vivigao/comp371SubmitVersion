//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"

class ObjectModel : public Model
{
public:
	ObjectModel(const char * objectpath);
	virtual ~ObjectModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:  
  unsigned int mVertexSize;
  
	unsigned int mVertexArrayID;
  
	unsigned int mVertexBufferID;
  unsigned int mUVBufferID;
  unsigned int mNormalBufferID;
};
