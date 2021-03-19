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
void OnExit();
void RecreateWindow(bool firstCall = false);

void DrawErrors();

void EnableMultiSample(bool enabled);
void SetMutlisampleMode(unsigned int mode);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const unsigned int ROCK_WIDTH = 64;
const unsigned int ROCK_HEIGHT = 64;
const unsigned int ROCK_DEPTH = 64;

Camera mCamera;
double mLastX = SCR_WIDTH / 2.0;
double mLastY = SCR_HEIGHT / 2.0;
bool mFirstMouse;

double mDeltaTime;
double mLastFrameTime;

int mShowFPS;

int mFrameCount;
double mFrameTime;

unsigned int mMultiSample = GL_FASTEST;
bool mMultiSampleEnabled = false;

GLFWwindow* mWindow;

TextRenderer mTextRenderer;
KeyHandler mKeyHandler;
DollyController mDollyController;

Material mGenerateRock;
Material mRock;
Material lightShader;
bool mEditMode = true;

unsigned int mFbo;
unsigned int mFboTex;

unsigned int mRockVao;
unsigned int mRockVbo;

unsigned int mEmptyVao;
unsigned int mEmptyVbo;

float mVerticesRock[6][2] = {{-1.0f, -1.0f}, {-1.0, 1.0}, {1.0, -1.0}, {1.0f, 1.0f}, {-1.0, 1.0}, {1.0, -1.0}};