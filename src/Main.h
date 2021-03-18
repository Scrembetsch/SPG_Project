#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "glBasics/camera.h"
#include "glBasics/material.h"
#include "glBasics/text_renderer.h"
#include "Primitives/Node.h"
#include "Primitives/Cube.h"

#include "Device/KeyHandler.h"
#include "Util/DollyController.h"
#include "Util/KdTree.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void ProcessInput(GLFWwindow* window);

void SetupMembers();
int SetupOpenGL();
void SetupObjects();
void SetupMaterials();
void SetupArraysAndBuffers();
bool SetupTextRenderer();
void RenderLoop();
void RenderScene(bool depthPass = false);
void OnExit();
void RecreateWindow();

void EnableMultiSample(bool enabled);
void SetMutlisampleMode(unsigned int mode);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;
const float LIGHT_NEAR_PLANE = 0.5f;
const float LIGHT_FAR_PLANE = 15.0f;

Camera mCamera;
double mLastX = SCR_WIDTH / 2.0;
double mLastY = SCR_HEIGHT / 2.0;
bool mFirstMouse;

double mDeltaTime;
double mLastFrameTime;

int mShowFPS;

int mFrameCount;
double mFrameTime;

unsigned int mPlaneVAO = 0;
unsigned int mPlaneVBO = 0;

unsigned int mDepthMapFbo = 0;
unsigned int mDepthMap = 0;

unsigned int mMultiSample = GL_FASTEST;
bool mMultiSampleEnabled = false;

GLFWwindow* mWindow;

std::vector<Node*> mCubes;

Shader* mSharedDefaultShader;
Texture* mSnowDiffuseTex;
Texture* mSnowNormalTex;
Material mMossMat;
Material mDepthMat;
Material mBoundingBoxMat;

TextRenderer mTextRenderer;
KeyHandler mKeyHandler;
DollyController mDollyController;
KdTree mKdTree;
Ray mCurrentRay;
double mNormalStrength;

bool mEditMode = true;
bool mDrawBoundingBoxes = false;
bool mDrawAlbedo = true;
glm::vec3 mLightPos;

Cube* mHitCube;

std::string mLoadFile;

Triangle* mTest;