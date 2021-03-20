#include "Main.h"

#include <iostream>
#include <filesystem>

#include "glBasics/texture_2d.h"
#include "glBasics/texture_3d.h"

#include "Util/Util.h"

int main(int argc, char** argv)
{
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    SetupMembers();
    CreateWindow();

    while (!glfwWindowShouldClose(mWindow))
    {
        RenderLoop();
    }

    OnExit();
    return 0;
}

void SetupMembers()
{
    mCamera.Position = glm::vec3(48.0f, 39.0f, -19.0f);
    mCamera.UpdateRotation(glm::quat(0.891428f, -0.313256f, -0.20025f, 0.259088f));
    mShowFPS = 0;
    mFrameCount = 0;
    mFrameTime = 0;
    mHeight = 0;
}

int SetupOpenGL()
{
    // GLFW Init & Config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, mMultiSample);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW Window create
    mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SPG", NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
    glfwSetCursorPosCallback(mWindow, MouseCallback);
    glfwSetScrollCallback(mWindow, ScrollCallback);
    glfwSetKeyCallback(mWindow, KeyCallback);
    glfwSetMouseButtonCallback(mWindow, MouseClickCallback);

    // Capture mouse
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: Load all OpenGL Function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 0;
}

void DrawErrors()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << err << std::endl;
    }
}

void SetupMaterials()
{
    mGenerateRock.UseShader(new Shader());
    mGenerateRock.GetShader()->load("shader/rock.vs", "shader/rock.fs");

    mRock.UseShader(new Shader());
    mRock.GetShader()->load("shader/renderRock.vs", "shader/renderRock.fs", "shader/renderRock.gs");
    mRock.mTextures.push_back(new Texture3D(mFboTex, GL_TEXTURE0));
}

bool SetupTextRenderer()
{
    mTextRenderer.LoadShader("shader/text.vs", "shader/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    mTextRenderer.SetProjection(projection, "uProjection");
    return mTextRenderer.SetupFreetype(std::filesystem::absolute("data/fonts/Consolas.ttf").string());
}

void SetupArraysAndBuffers()
{
    // Framebuffer & 3D texture
    glGenTextures(1, &mFboTex);
    glBindTexture(GL_TEXTURE_3D, mFboTex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, ROCK_WIDTH, ROCK_HEIGHT, ROCK_DEPTH, 0, GL_RED, GL_FLOAT, nullptr);

    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

    GLenum c = GL_COLOR_ATTACHMENT0;
    glFramebufferTexture3D(GL_FRAMEBUFFER, c, GL_TEXTURE_3D, mFboTex, 0, 0);
    glDrawBuffers(1, &c);

    // VAO & VBO
    glGenVertexArrays(1, &mEmptyVao);

    glGenVertexArrays(1, &mRockVao);
    glGenBuffers(1, &mRockVbo);

    glBindVertexArray(mRockVao);
    glBindBuffer(GL_ARRAY_BUFFER, mRockVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVerticesRock), mVerticesRock, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindVertexArray(0);
}

void RenderLoop()
{
    // Per frame logic
    HandlePreFrameLogic();

    HandleInput();

    RenderDefaultPass();

    DrawText();

    // Frame End Updates
    HandleEndFrameLogic();
}

void HandleEndFrameLogic()
{
    mKeyHandler.FrameUpdate();
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

void HandlePreFrameLogic()
{
    double currentFrame = glfwGetTime();
    mDeltaTime = currentFrame - mLastFrameTime;
    mLastFrameTime = currentFrame;

    DrawErrors();
}

void HandleInput()
{
    ProcessInput(mWindow);

    if (!mEditMode)
    {
        mDollyController.NextStep(float(mDeltaTime) * mCamera.MovementSpeed);
        mCamera.Position = mDollyController.TCB();
        mCamera.UpdateRotation(mDollyController.SQUAD());
    }
}

void RenderDefaultPass()
{
    // Set light uniforms
    RenderScene();
}

void DrawText()
{
    glEnable(GL_BLEND);

    std::string text;
    text += "Edit Mode:\t\t\t";
    text += mEditMode ? "ON\n" : "OFF\n";
    text += "Camera Speed:\t\t" + std::to_string(mCamera.MovementSpeed);
    mTextRenderer.RenderFormattedText(text, 5.0f, SCR_HEIGHT - 20.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);

    if (mFrameTime >= 0.5)
    {
        mShowFPS = static_cast<int>(mFrameCount / mFrameTime);
        mFrameTime = 0;
        mFrameCount = 0;
    }
    mFrameTime += mDeltaTime;
    mFrameCount++;

    text = "FPS:" + std::to_string(mShowFPS);
    mTextRenderer.RenderText(text, SCR_WIDTH - 100.0f, SCR_HEIGHT - 20.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));

    text = "+";
    mTextRenderer.RenderText(text, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

    glDisable(GL_BLEND);
}

void RenderScene()
{
    // Reset polygon mode before drawing noise (otherwise result is not correct)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glViewport(0, 0, ROCK_WIDTH, ROCK_HEIGHT);
    mGenerateRock.use();
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glClear(GL_COLOR_BUFFER_BIT);
    mGenerateRock.GetShader()->setFloat("uHeight", mHeight);
    for (int i = 0; i < ROCK_DEPTH; i++)
    {
        float layer = float(i) / float(ROCK_DEPTH - 1.0f);
        mGenerateRock.GetShader()->setFloat("uLayer", layer);
        glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, mFboTex, 0, i);
        glBindVertexArray(mRockVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mRock.use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = mCamera.GetViewMatrix();
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
    mRock.GetShader()->setMat4("uProjection", projection);
    mRock.GetShader()->setMat4("uView", view);
    mRock.GetShader()->setMat4("uModel", modelMatrix);
    mRock.GetShader()->setInt("uWidth", ROCK_WIDTH);
    mRock.GetShader()->setInt("uHeight", ROCK_HEIGHT);
    mRock.GetShader()->setInt("uDepth", ROCK_DEPTH);
    if (mWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBindVertexArray(mEmptyVao);
    glDrawArrays(GL_POINTS, 0, ROCK_WIDTH * ROCK_HEIGHT * ROCK_DEPTH);
}

void CreateWindow()
{
    SetupOpenGL();

    SetupArraysAndBuffers();
    SetupMaterials();

    SetupTextRenderer();
}

void OnExit()
{
    glDeleteFramebuffers(1, &mFbo);
    glDeleteTextures(1, &mFboTex);
    glDeleteVertexArrays(1, &mRockVao);
    glDeleteBuffers(1, &mRockVbo);
    glDeleteBuffers(1, &mEmptyVbo);

    glfwTerminate();
}

// Process input
void ProcessInput(GLFWwindow* window)
{
    float delta = static_cast<float>(mDeltaTime);

    // Close window
    if (mKeyHandler.WasKeyPressed(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Handle Camera movment
    if (mKeyHandler.IsKeyDown(GLFW_KEY_W))
    {
        mCamera.ProcessKeyboard(Camera::Camera_Movement::FORWARD, delta);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_S))
    {
        mCamera.ProcessKeyboard(Camera::Camera_Movement::BACKWARD, delta);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_A))
    {
        mCamera.ProcessKeyboard(Camera::Camera_Movement::LEFT, delta);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_D))
    {
        mCamera.ProcessKeyboard(Camera::Camera_Movement::RIGHT, delta);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_SPACE))
    {
        mHeight += delta;
        //mCamera.ProcessKeyboard(Camera::Camera_Movement::UP, delta);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        mHeight -= delta;

        //mCamera.ProcessKeyboard(Camera::Camera_Movement::DOWN, delta);
    }

    if (mKeyHandler.IsKeyDown(GLFW_KEY_KP_ADD))
    {
        mCamera.MovementSpeed += delta * 5;
    }

    if (mKeyHandler.IsKeyDown(GLFW_KEY_KP_SUBTRACT))
    {
        mCamera.MovementSpeed -= delta * 5;
    }

    if (mKeyHandler.WasKeyReleased(GLFW_KEY_H))
    {
        mWireframe = !mWireframe;
    }

    if (mKeyHandler.WasKeyReleased(GLFW_KEY_J))
    {
        std::cout << mCamera.Position.x << ", " << mCamera.Position.y << ", " <<mCamera.Position.z << std::endl;
        std::cout << mCamera.Rotation.w << ", " << mCamera.Rotation.x << ", " << mCamera.Rotation.y << ", " << mCamera.Rotation.z << std::endl;
    }

    // Dolly Controller
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_E))
    {
        if (mEditMode && mDollyController.mPositions.size() > 0)
        {
            mEditMode = false;
        }
        else
        {
            mEditMode = true;
        }
        mDollyController.Reset();
    }
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_BACKSPACE))
    {
        if (mEditMode)
        {
            mDollyController.mPositions.clear();
            mDollyController.mDirections.clear();
        }
    }
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_ENTER))
    {
        if (mEditMode)
        {
            mDollyController.mPositions.push_back(mCamera.Position);
            mDollyController.mDirections.push_back(mCamera.Rotation);
        }
    }
}

void MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
            mKeyHandler.KeyPressed(key);
            break;

        case GLFW_RELEASE:
            mKeyHandler.KeyReleased(key);
            break;

        default:
            break;
    }
}

// GLFW Callback when window changes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// GLFW Callback when mouse moves
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (mEditMode)
    {
        if (mFirstMouse)
        {
            mLastX = xpos;
            mLastY = ypos;
            mFirstMouse = false;
        }

        double xoffset = xpos - mLastX;
        double yoffset = mLastY - ypos; // Reverse: y-coordinates go from bottom to top

        mLastX = xpos;
        mLastY = ypos;

        mCamera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}

// GLFW Callback when mousewheel is used
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (mEditMode)
    {
        mCamera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}
