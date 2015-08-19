// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"

#include "BSpline.h"
#include "CubeModel.h"
#include "WaterModel.h"
#include "ObjectModel.h"
#include "SphereModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"

#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

using namespace std;
using namespace glm;

#if defined(PLATFORM_OSX)
std::string objPathPrefix = "Objects/";
std::string texturePathPrefix = "Textures/";
#else
std::string objPathPrefix = "../Assets/Objects/";
std::string texturePathPrefix = "../Assets/Textures/";
#endif

World* World::instance;

World::World()
{
  instance = this;
  ourGuy = new CubeModel();
  ourSphere = new SphereModel();
  
  vec3 guyPosition = ourGuy->GetPosition();
  
  ThirdPersonCamera *newThirdCamera = new ThirdPersonCamera(guyPosition);
  newThirdCamera->SetTargetModel(ourGuy);
  FirstPersonCamera *newFirstCamera = new FirstPersonCamera(guyPosition);
  newFirstCamera->setTargetModel(ourGuy);
  
  mCamera.push_back(newThirdCamera);
  mCamera.push_back(newFirstCamera);
  
  // Setup Camera
  mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
  mCamera.push_back(new StaticCamera(vec3(0.5f, 0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
  mCurrentCamera = 0;
    // string textureFile = "BillboardTest.bmp";
    string textureFile = "Particle.png";
   string path = texturePathPrefix + textureFile;
  int billboardTextureID = TextureLoader::LoadTexture(path.c_str());
 
    assert(billboardTextureID != 0);
    mpBillboardList = new BillboardList(2048, billboardTextureID);
  //	mBSpline.push_back(new BSpline( vec3(1.0, 1.0, 1.0) ) );


}

World::~World()
{
  // Models
  for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
    delete *it;
  mModel.clear();
  
  for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    delete *it;
  mAnimation.clear();
  
  for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    delete *it;
  mAnimationKey.clear();
  
  // Camera
  for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
    delete *it;
  mCamera.clear();
  
  // clear Spline
  for (vector<BSpline*>::iterator it = mBSpline.begin(); it < mBSpline.end(); ++it)
    delete *it;
  mBSpline.clear();
  
  delete mWater;
  
  delete mMap;
  
  delete mpBillboardList;
}

World* World::GetInstance()
{
  return instance;
}

void World::Update(float dt)
{
  // User Inputs
  // 0 1 2 to change the Camera
  if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
    mCurrentCamera = 0;
  else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
    if (mCamera.size() > 1)
      mCurrentCamera = 1;
    else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
      if (mCamera.size() > 2)
        mCurrentCamera = 2;
  
  // Spacebar to change the shader
  if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
    Renderer::SetShader(SHADER_SOLID_COLOR);
  else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
    Renderer::SetShader(SHADER_BLUE);
  
  // Update animation and keys
  for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    (*it)->Update(dt);
     for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
   (*it)->Update(dt);//
  
  // Update current Camera
  mCamera[mCurrentCamera]->Update(dt);
  
  // Update models
  for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
    (*it)->Update(dt);//*/
  
    // Update billboards
   for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it != mParticleSystemList.end(); ++it)
   (*it)->Update(dt);
   mpBillboardList->Update(dt);//
  
  mWater->Update(dt);
  
  // Update Spline
  /*	for (vector<BSpline*>::iterator it = mBSpline.begin(); it != mBSpline.end(); ++it)
   (*it)->Update(dt);//*/
}

void World::Draw()
{
  Renderer::BeginFrame();
  
  // Set shader to use
  glUseProgram(Renderer::GetShaderProgramID());
  
  // This looks for the MVP Uniform variable in the Vertex Program
  GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
  
  // Send the view projection constants to the shader
  mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
  glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
  
  // Draw models
  for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
    (*it)->Draw();
  
  // Draw Path Lines
  // Set Shader for path lines
  unsigned int prevShader = Renderer::GetCurrentShader();
  Renderer::SetShader(SHADER_PATH_LINES);
  glUseProgram(Renderer::GetShaderProgramID());
  
  // Send the view projection constants to the shader
  VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
  glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
  
  for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it){
    mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
    glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
    
    (*it)->Draw();
  }
  /*	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it){
   mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
   glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
   
   (*it)->Draw();
   }//*/
  Renderer::CheckForErrors();
  
  
  Renderer::SetShader((ShaderType) prevShader);
  
  mWater->Draw();
  
  mMap->Draw();
  
  // Draw Billboards
      mpBillboardList->Draw();
  
  //Draw Spline
  /*	for (vector<BSpline*>::iterator it = mBSpline.begin(); it < mBSpline.end(); ++it)
   (*it)->Draw();//*/
  
  // Restore previous shader
  //	Renderer::SetShader((ShaderType) prevShader);
  Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
  // Using case-insensitive strings and streams for easier parsing
  ci_ifstream input;
  input.open(scene_path, ios::in);
  
  // Invalid file
  if(input.fail() ){
    fprintf(stderr, "Error loading file: %s\n", scene_path);
    getchar();
    exit(-1);
  }
  
  ci_string item;
  while( std::getline( input, item, '[' ) ){
    ci_istringstream iss( item );
    
    ci_string result;
    if( std::getline( iss, result, ']') ){
      if (result == "cube"){
        // Box attributes
        //* cube = new CubeModel();
        ourGuy->Load(iss);
        mModel.push_back(ourGuy);
      }
      else if (result == "pillar"){
        CubeModel* pillar = new CubeModel();
        pillar->Load(iss);
        mModel.push_back(pillar);
      }
      else if( result == "sphere" ){
        SphereModel* sphere = new SphereModel();
        sphere->Load(iss);
        mModel.push_back(sphere);
      }
      else if ( result == "animationkey" ){
        AnimationKey* key = new AnimationKey();
        key->Load(iss);
        mAnimationKey.push_back(key);
      }
      else if (result == "animation"){
        Animation* anim = new Animation();
        anim->Load(iss);
        mAnimation.push_back(anim);
      }
      else if (result == "map") {
        string path = objPathPrefix + "map.obj";
        mMap = new ObjectModel(path.c_str());
        mMap->Load(iss);
      }
      else if (result == "bspline"){
        BSpline* spline = new BSpline();
        spline->Load(iss);
        mBSpline.push_back(spline);
      }
      else if (result == "water"){
        string path = texturePathPrefix + "Water.bmp";
        mWater = new WaterModel(path.c_str());
        mWater->Load(iss);
      }
      else if ( result.empty() == false && result[0] == '#'){
        // this is a comment line
      }
      else{
        fprintf(stderr, "Error loading scene file... !");
        getchar();
        exit(-1);
      }
    }
  }
  input.close();
  
  // Set Animation vertex buffers
  for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    // Draw model
    (*it)->CreateVertexBuffer();
}

Animation* World::FindAnimation(ci_string animName)
{
  for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    if((*it)->GetName() == animName)
      return *it;
  return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
  for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    if((*it)->GetName() == keyName)
      return *it;
  return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
  return mCamera[mCurrentCamera];
}

void World::AddBillboard(Billboard* b)
{
  mpBillboardList->AddBillboard(b);
}

void World::RemoveBillboard(Billboard* b)
{
  mpBillboardList->RemoveBillboard(b);
}

void World::AddParticleSystem(ParticleSystem* particleSystem)
{
  mParticleSystemList.push_back(particleSystem);
}

void World::RemoveParticleSystem(ParticleSystem* particleSystem)
{
  vector<ParticleSystem*>::iterator it = std::find(mParticleSystemList.begin(), mParticleSystemList.end(), particleSystem);
  mParticleSystemList.erase(it);
}

void World::AddAnimationKey(AnimationKey* ak)
{
  mAnimationKey.push_back(ak);
}

void World::AddAnimation(Animation* a)
{
  mAnimation.push_back(a);
}
