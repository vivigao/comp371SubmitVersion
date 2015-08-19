//
// COMP 371 Final Project
//
// Created by Emma Saboureau
//
//

#pragma once

#include "Model.h"

class WaterModel : public Model
{
public:
	WaterModel(const char * texturepath);
	virtual ~WaterModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
    glm::vec2 uv;
	};
  
  std::vector<unsigned short> mIndexBuffer;
  std::vector<Vertex> vertexBuffer;
//  std::vector<glm::vec3> vertexBuffer;
//  std::vector<glm::vec3> normalBuffer;
//  std::vector<glm::vec3> colourBuffer;
  
  int mTextureID;
  
  float timer;
  
  unsigned int mRows;
  unsigned int mColumns;
  
  float waveTime,
  waveWidth,
  waveHeight,
  waveFreq;
  
  int waveTimeLocation,
  waveWidthLocation,
  waveHeightLocation;
 
  
  unsigned int mElementBuffer;
	unsigned int mVertexArrayID;
  unsigned int mVertexBufferID;
  unsigned int mNormalBufferID;
  unsigned int mColourBufferID;
  unsigned int mUVBUfferID;
};
