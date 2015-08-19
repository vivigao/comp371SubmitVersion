// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "WaterModel.h"
#include "Renderer.h"
#include "Camera.h"
#include "World.h"
#include "TextureLoader.h"

#include <math.h>       /* fmod */
#include <stdio.h>
// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

//	vec3 halfSize = size * 0.5f;

// Material Coefficients
const float ka = 0.2f;
const float kd = 0.8f;
const float ks = 0.2f;
const float n = 90.0f;

// Light Coefficients
const vec3 lightColor(1.0f, 1.0f, 1.0f);
const float lightKc = 0.0f;
const float lightKl = 0.0f;
const float lightKq = 1.0f;
const vec4 lightPosition(20.0f, -40.0f, 20.0f, 1.0f); // If w = 1.0f, we have a point light

WaterModel::WaterModel(const char * texturepath) : Model()
{
  mTextureID = TextureLoader::LoadTexture(texturepath);
  assert(mTextureID != 0);

    mRows = 51;
  mColumns = 51;
  
  waveTime = 3,
  waveWidth = 1,
  waveHeight = 0.8f,
  waveFreq = 0.03f;
  
  /***** Populate Index buffer uniforms
   */
  short int row, col;
  
  for (row=0; row<mRows-1; row++) { // 0 -> 1
    
    for (col=0; col<mColumns; col++) { // 0 -> 2
      mIndexBuffer.push_back(col + (mRows*row));
      mIndexBuffer.push_back(col + (mRows*(row+1)));
    }
    
    // degenerate
    if (row != mRows-2) {
      mIndexBuffer.push_back(mColumns-1 + (mRows*(row+1))); // last value
      mIndexBuffer.push_back(0 + (mRows*(row+1))); // first value
    }
    
  }
  
  /***** Populate Vertex buffer
   */
  short int halfRow = (mRows-1)/2;
  short int halfColumn = (mColumns-1)/2;
  
  int vertexIndex = 0;
  bool firstRow = true;
  bool firstCol = true;
  Vertex vv;
  for (row = -halfRow; row <= halfRow; row++) {
    for (col = -halfColumn; col <= halfColumn; col++) {
      vec3 position = vec3(col, 0.0f, row);
      vec3 normal = vec3(0.0f, -1.0f, 0.0f);
      vec3 colour = vec3(0.709803922f, 0.866666667f, 0.921568627f);
      vec2 uv;
      
      if (firstRow) {
        if (firstCol) {
          uv = vec2(0, 1);
        } else {
          uv = vec2(1, 1);
        }
      } else {
        if (firstCol) {
          uv = vec2(0, 0);
        } else {
          uv = vec2(1, 0);
        }
      }
      firstCol = !firstCol;
      vv.position = position;
	  vv.normal = normal;
	  vv.color = colour;
	  vv.uv = uv;
	  vertexBuffer.push_back(vv);
      //vertexBuffer.push_back({position, normal, colour, uv});
      vertexIndex++;
    }
    firstRow = !firstRow;
  }
  
  // Create a vertex array
  glGenVertexArrays(1, &mVertexArrayID);
  
  // Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
  glGenBuffers(1, &mVertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &mElementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.size() * sizeof(unsigned short), &mIndexBuffer[0] , GL_STATIC_DRAW);
}

WaterModel::~WaterModel(){
  // Free the GPU from the Vertex Buffer
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteVertexArrays(1, &mVertexArrayID);
}

void WaterModel::Update(float dt){
  Model::Update(dt);
  
  timer+=dt;
  
  float two_pies = 3.14f*2.0f;
  //  float wavePosition = fmodf(timer, 2*pie);
  //  float waveProgression = wavePosition / (2.0f*pie);
  int waveLength = 10;
  float waveSegmentLength = two_pies/(waveLength);
  
  float waveProgression = fmodf(timer, waveLength)/waveLength;
  
  for (int i=0; i<mColumns; i++) {
    for (int j=0; j<mRows; j++) {
      int index = i + (j*mColumns);
      float yCoord = sinf(((i%waveLength)*waveSegmentLength)+(waveProgression*two_pies));
      
      vertexBuffer[index].position.y = yCoord;
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);
}

void WaterModel::Draw()
{
  unsigned int prevShader = Renderer::GetCurrentShader();
  Renderer::SetShader(SHADER_WATER_TRANSPARENT);
  glUseProgram(Renderer::GetShaderProgramID());
  
  glBindVertexArray(mVertexArrayID);
  
  
  /***** Send uniforms
   */
  
  const Camera* cam = World::GetInstance()->GetCurrentCamera();

  GLuint WorldMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
  glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
  
  GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &cam->GetViewMatrix()[0][0]);
  
  GLuint ProjectionMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectionTransform");
  glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &cam->GetProjectionMatrix()[0][0]);
  
  // Get a handle for Light Attributes uniform
  GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPosition");
  GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightColor");
  GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");
  
  // Get a handle for Material Attributes uniform
  GLuint MaterialAmbientID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialAmbient");
  GLuint MaterialDiffuseID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialDiffuse");
  GLuint MaterialSpecularID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialSpecular");
  GLuint MaterialExponentID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialExponent");
  
  glUniform1f(MaterialAmbientID, ka);
  glUniform1f(MaterialDiffuseID, kd);
  glUniform1f(MaterialSpecularID, ks);
  glUniform1f(MaterialExponentID, n);
  
  glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
  glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
  glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);
  
  
  /***** Send texture
   */
  
  GLuint textureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glUniform1i(textureID, 0);				// Set our Texture sampler to user Texture Unit 0
  
  
  /***** Send buffers
   */
  
  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(0,                // attribute
                        3,                // size
                        GL_FLOAT,         // type
                        GL_FALSE,         // normalized?
                        sizeof(Vertex),                // stride
                        (void*)0          // array buffer offset
                        );
  
  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void*)sizeof(vec3)
                        );
  
  
  // 3rd attribute buffer : colours
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(2,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void*)(2*sizeof(vec3))
                        );
  
  // 4th attribute buffer : texture coordinates
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(3,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void*) (2*sizeof(vec3) + sizeof(vec4)) // texture coordinates are Offseted by 2 vec3 (see class Vertex) and a vec4
                        );
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
  
  /***** Draw & cleanup
   */
  
  glDrawElements(GL_TRIANGLE_STRIP,           // mode
                 (int) mIndexBuffer.size(),   // count
                 GL_UNSIGNED_SHORT,           // type
                 (void*)0                     // element array buffer offset
                 );
  
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  
  Renderer::CheckForErrors();
  Renderer::SetShader((ShaderType) prevShader);
}

bool WaterModel::ParseLine(const std::vector<ci_string> &token){
  if (token.empty())
    return true;
  else
    return Model::ParseLine(token);
}