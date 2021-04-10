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

#include "Device/KeyHandler.h"
#include "Util/DollyController.h"

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
void RenderLoop();
void HandleEndFrameLogic();
void HandlePreFrameLogic();
void HandleInput();
void RenderDefaultPass();
void DrawText();
void RenderScene();
void RenderGeneratedGeometry();
void RenderParallaxObjects();
void RenderQuad();
void OnExit();
void CreateWindow();

void PrintErrors();

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

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

bool mEditMode = true;
bool mWireframe = false;
unsigned int mFbo;
unsigned int mFboTex;

unsigned int mRockVao;
unsigned int mRockVbo;

unsigned int mEmptyVao;
unsigned int mEmptyVbo;

unsigned int mQuadVao;
unsigned int mQuadVbo;

float mHeight;
float mHeightScale;
int mSteps;
int mRefinmentSteps;

float mVerticesRock[6][2] = {{-1.0f, -1.0f}, {-1.0, 1.0}, {1.0, -1.0}, {1.0f, 1.0f}, {-1.0, 1.0}, {1.0, -1.0}};
glm::vec3 mLightPos;
