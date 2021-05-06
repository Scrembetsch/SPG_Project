#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "glBasics/Camera.h"
#include "glBasics/Material.h"
#include "glBasics/TextRenderer.h"

#include "Device/KeyHandler.h"
#include "Util/DollyController.h"

#include "Primitives/Plane.h"
#include "Particle/ParticleSystem.h"
#include "Util/Ray.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void ProcessInput(GLFWwindow* window);

void SetupMembers();
int SetupOpenGL();
void SetupMaterials();
void SetupArraysAndBuffers();
bool SetupTextRenderer();

void HandleInput();

void HandlePreFrameLogic();
void RenderLoop();
void HandleEndFrameLogic();

void UpdateScene();
void RenderShadowPass();
void RenderDefaultPass();
void DrawText();

void UpdateGeneratedGeometry();
void UpdateParticleSystem();

void RenderScene();
void RenderGeneratedGeometry(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& lightSpace);
void RenderParallaxObjects(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& lightSpace);
void RenderBackground(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& lightSpace);
void RenderParticleSystem(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& lightSpace);


void OnExit();
void CreateWindow();

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

const float LIGHT_NEAR_PLANE = 0.5f;
const float LIGHT_FAR_PLANE = 100.0f;

unsigned int CAMERA_WIDTH = 0;
unsigned int CAMERA_HEIGHT = 0;

const float ROCK_SCALE = 0.2f;
const unsigned int ROCK_WIDTH = 96;
const unsigned int ROCK_HEIGHT = 256;
const unsigned int ROCK_DEPTH = 96;

Camera mCamera;
double mLastX = SCR_WIDTH / 2.0;
double mLastY = SCR_HEIGHT / 2.0;
bool mFirstMouse;

double mDeltaTime;
double mLastFrameTime;

int mShowFPS;

int mFrameCount;
double mFrameTime;

GLFWwindow* mWindow;

TextRenderer mTextRenderer;
KeyHandler mKeyHandler;
DollyController mDollyController;

Material mGenerateRock;
Material mRock;
Material mParallax;
Material mBackground;
Material mFloor;
Material mLine;

bool mEditMode = true;
bool mWireframe = false;
unsigned int mFbo;
unsigned int mFboTex;

Plane* mParallaxPlane;
Plane* mBackgroundPlane;
Plane* mFloorPlane;
Plane* mShadowPlane;
Plane* mShadowPlane2;

unsigned int mRockVao;
unsigned int mRockVbo;

unsigned int mEmptyVao;
unsigned int mEmptyVbo;

unsigned int mDepthMapFbo;
unsigned int mDepthMap;

float mShadowPass;

float mHeight;
float mHeightScale;
int mSteps;
int mRefinmentSteps;

float mVerticesRock[6][2] = {{-1.0f, -1.0f}, {-1.0, 1.0}, {1.0, -1.0}, {1.0f, 1.0f}, {-1.0, 1.0}, {1.0, -1.0}};
glm::vec3 mLightPos;
glm::vec3 mLightDir;

ParticleSystem mParticleSystem;

Ray mCurrentRay;