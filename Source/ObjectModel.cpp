// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "ObjectModel.h"
#include "Renderer.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

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
const vec4 lightPosition(0.0f, 20.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light

ObjectModel::ObjectModel(const char * objectpath) : Model()
{
  vector<vec3> vertices;
  vector<vec2> uvs;
  vector<vec3> normals;
  bool result = ObjectLoader::LoadObject(objectpath, vertices, uvs, normals);
  
  if (!result) {
    printf("Could not initialize object, object did not load: %s", "Objects/map.obj");
    exit(0);
  }
  
  mVertexSize = (int) vertices.size();
  
  // Create a vertex array
  glGenVertexArrays(1, &mVertexArrayID);
  
  // Load buffers
  glGenBuffers(1, &mVertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &mUVBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &mNormalBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

ObjectModel::~ObjectModel()
{
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mUVBufferID);
  glDeleteVertexArrays(1, &mVertexArrayID);
}

void ObjectModel::Update(float dt)
{
  Model::Update(dt);
}

void ObjectModel::Draw()
{
  unsigned int prevShader = Renderer::GetCurrentShader();
  Renderer::SetShader(SHADER_PHONG);
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
  
  
  /***** Send buffers
   */
  
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(0,  // The attribute we want to configure
                        3,                            // size
                        GL_FLOAT,                     // type
                        GL_FALSE,                     // normalized?
                        0,                            // stride
                        (void*)0                      // array buffer offset
                        );
  
  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
  glVertexAttribPointer(1,                   // The attribute we want to configure
                        3,                            // size : U+V => 2
                        GL_FLOAT,                     // type
                        GL_FALSE,                     // normalized?
                        0,                            // stride
                        (void*)0                      // array buffer offset
                        );
  
  
  /***** Draw & cleanup
   */
  
  glDrawArrays(GL_TRIANGLES, 0, mVertexSize);
  
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  
  Renderer::CheckForErrors();
  Renderer::SetShader((ShaderType) prevShader);
}

bool ObjectModel::ParseLine(const std::vector<ci_string> &token)
{
  if (token.empty())
    return true;
  else
    return Model::ParseLine(token);
}