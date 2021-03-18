#include "Main.h"

#include <iostream>
#include <filesystem>

#include "Util/Util.h"
#include "Util/Ray.h"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        mLoadFile = argv[1];
    }
    SetupMembers();
    
    RecreateWindow();

    while (!glfwWindowShouldClose(mWindow))
    {
        RenderLoop();
    }

    OnExit();
    glfwTerminate();
    return 0;
}

void SetupMembers()
{
    mCamera.Position = glm::vec3(2.81253, 2.1115, 5.30192);
    mNormalStrength = 1.0;
    mShowFPS = 0;
    mFrameCount = 0;
    mFrameTime = 0;
    mHitCube = nullptr;

    mLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}

int SetupOpenGL()
{
    // GLFW Init & Config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, mMultiSample);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW Window create
    mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EZG_ALGO", NULL, NULL);
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

    // GLAD: Load all OpenGL Funtion pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void SetupObjects()
{
    glm::mat4 model(1.0f);

    // Flying Cube
    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
    //model = glm::scale(model, glm::vec3(0.5f));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);

    // Cube on floor
    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);

    // Cube that will rotate
    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, 2.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);

    // Snowman
    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.25001f, 5.0));
    model = glm::scale(model, glm::vec3(0.75));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);
    
    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 1.5002f, 5.0));
    model = glm::scale(model, glm::vec3(0.5));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);

    mCubes.push_back(new Cube());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 2.2503f, 5.0));
    model = glm::scale(model, glm::vec3(0.25f));
    mCubes[mCubes.size() - 1]->SetModelMatrix(model);

    for (int i = 0; i < 1000 && !mLoadFile.empty(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i * 3.0f, 1.0f, sinf(i)));
        Util::LoadObjFile(std::filesystem::absolute("data/models/" + mLoadFile).string(), mCubes, model);
    }

    std::vector<Triangle*> triangles;
    //triangles.reserve(mCubes.size() * Cube::MAX_TRIANGLES);
    for (auto node : mCubes)
    {
        Cube* cube = dynamic_cast<Cube*>(node);
        if (cube != nullptr)
        {
            for (int i = 0; i < Cube::MAX_TRIANGLES; i++)
            {
                triangles.push_back(cube->GetTriangles()[i]);
            }
        }
        else
        {
            Triangle* tri = dynamic_cast<Triangle*>(node);
            triangles.push_back(tri);
        }

    }
    std::cout << "Loaded " << triangles.size() << " triangles" << std::endl;

    mKdTree.BuildTree(&triangles, 50);
}

void SetupMaterials()
{
    // Load and Compile shaders
    mSharedDefaultShader = new Shader();
    mSharedDefaultShader->load("shader/main.vs", "shader/main.fs");
    for (int i = 0; i < mCubes.size(); i++)
    {
        mCubes[i]->GetMaterial()->UseSharedShader(mSharedDefaultShader);
    }
    mMossMat.UseSharedShader(mSharedDefaultShader);
    //mTest->SetMaterial(&mMossMat, false);

    // Load textures
    mCubes[1]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Brick.jpg").string().c_str()), GL_TEXTURE0));
    mCubes[1]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Brick_Normal.jpg").string().c_str()), GL_TEXTURE1));
    mMossMat.mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Moss.jpg").string().c_str()), GL_TEXTURE0));
    mMossMat.mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Moss_Normal.jpg").string().c_str()), GL_TEXTURE1));
    mCubes[0]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Sponge.jpg").string().c_str()), GL_TEXTURE0));
    mCubes[0]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Sponge_Normal.jpg").string().c_str()), GL_TEXTURE1));
    mCubes[2]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Wood.jpg").string().c_str()), GL_TEXTURE0));
    mCubes[2]->GetMaterial()->mTextures.push_back(new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Wood_Normal.jpg").string().c_str()), GL_TEXTURE1));

    mSnowDiffuseTex = new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Snow.jpg").string().c_str()), GL_TEXTURE0);
    mSnowNormalTex =  new Texture(Util::LoadTexture(std::filesystem::absolute("data/textures/Snow_Normal.jpg").string().c_str()), GL_TEXTURE1);
    mSnowDiffuseTex->mShared = true;
    mSnowNormalTex->mShared = true;

    for (int i = 3; i < 6; i++)
    {
        mCubes[i]->GetMaterial()->mTextures.push_back(mSnowDiffuseTex);
        mCubes[i]->GetMaterial()->mTextures.push_back(mSnowNormalTex);
    }

    // Shader config
    mSharedDefaultShader->use();
    mSharedDefaultShader->setInt("uDiffuseMap", 0);
    mSharedDefaultShader->setInt("uNormalMap", 1);
    mSharedDefaultShader->setInt("uShadowMap", 2);

    mDepthMat.UseShader(new Shader());
    mDepthMat.GetShader()->load("shader/depth_pass.vs", "shader/depth_pass.fs");
    mBoundingBoxMat.UseShader(new Shader());
    mBoundingBoxMat.GetShader()->load("shader/bv.vs", "shader/bv.fs");
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
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    glGenVertexArrays(1, &mPlaneVAO);
    glGenBuffers(1, &mPlaneVBO);
    glBindVertexArray(mPlaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mPlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // Confgiure depth map FBO
    glGenFramebuffers(1, &mDepthMapFbo);
    // Create depth texture
    glGenTextures(1, &mDepthMap);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Attach depth map as FBOs depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderLoop()
{
    // Per frame logic
    double currentFrame = glfwGetTime();
    mDeltaTime = currentFrame - mLastFrameTime;
    mLastFrameTime = currentFrame;

    // Handle Input
    ProcessInput(mWindow);

    // Clear Buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!mEditMode)
    {
        mDollyController.NextStep(float(mDeltaTime) * mCamera.MovementSpeed);
        mCamera.Position = mDollyController.TCB();
        mCamera.UpdateRotation(mDollyController.SQUAD());
    }

    // Render Scene from light perspective
    glm::mat4 lightProjection;
    glm::mat4 lightView;
    glm::mat4 lightSpaceMatrix;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, LIGHT_NEAR_PLANE, LIGHT_FAR_PLANE);
    lightView = glm::lookAt(mLightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // Use  Depth Material with special dpeth shader
    mDepthMat.use();
    mDepthMat.GetShader()->setMat4("uLightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderScene(true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Reset Viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Scene with shadows from the depth map
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mSharedDefaultShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = mCamera.GetViewMatrix();
    mSharedDefaultShader->setMat4("uProjection", projection);
    mSharedDefaultShader->setMat4("uView", view);
    mBoundingBoxMat.use();
    mBoundingBoxMat.GetShader()->setMat4("uProjection", projection);
    mBoundingBoxMat.GetShader()->setMat4("uView", view);

    mSharedDefaultShader->use();
    // Set light uniforms
    mSharedDefaultShader->setVec3("uViewPos", mCamera.Position);
    mSharedDefaultShader->setVec3("uLightPos", mLightPos);
    mSharedDefaultShader->setMat4("uLightSpaceMatrix", lightSpaceMatrix);
    mSharedDefaultShader->setFloat("uNormalStrength", static_cast<float>(mNormalStrength));
    mSharedDefaultShader->setVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    RenderScene(false);
    
    std::string text    =  "Normal strength:\t" + std::to_string(mNormalStrength) + "\n";
    text                += "Anti-Aliasing:\t\t";
    text                += mMultiSampleEnabled ? "ON\n" : "OFF\n";
    text                += "Mode:\t\t\t\t";
    text                += mMultiSample == GL_NICEST ? "NICEST\n" : "FASTEST\n";
    text                += "Edit Mode:\t\t\t";
    text                += mEditMode ? "ON\n" : "OFF\n";
    text                += "Camera Speed:\t\t" + std::to_string(mCamera.MovementSpeed);
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
    mKeyHandler.FrameUpdate();
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

void RenderScene(bool depthPass)
{   
    Material* currentMat = &mDepthMat;
    // Floor
    if (!depthPass)
    {
        currentMat = &mMossMat;
    }
    currentMat->use();
    glm::mat4 model = glm::mat4(1.0);
    currentMat->GetShader()->setMat4("uModel", model);
    glBindVertexArray(mPlaneVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (mDrawAlbedo)
    {
        for (int i = 0; i < mCubes.size(); i++)
        {
            if (depthPass)
            {
                mCubes[i]->Draw(&mDepthMat);
            }
            else
            {
                mCubes[i]->Draw();
            }
        }
    }

    //mTest->Draw();
    mBoundingBoxMat.use();
    mCurrentRay.Draw();
    if (mHitCube != nullptr)
    {
        mHitCube->Draw();
    }
    if (mDrawBoundingBoxes)
    {
        mKdTree.DrawTree();
    }
}

void RecreateWindow()
{
    OnExit();
    glfwTerminate();
    
    SetupOpenGL();

    SetupArraysAndBuffers();
    SetupObjects();
    SetupMaterials();

    SetupTextRenderer();
}

void OnExit()
{
    mCubes.clear();
    delete mSharedDefaultShader;
    delete mSnowDiffuseTex;
    delete mSnowNormalTex;
    if (mPlaneVAO != 0)
    {
        glDeleteVertexArrays(1, &mPlaneVAO);
    }
    if (mPlaneVBO != 0)
    {
        glDeleteBuffers(1, &mPlaneVBO);
    }
    if (mDepthMapFbo != 0)
    {
        glDeleteFramebuffers(1, &mDepthMapFbo);
    }
    if (mDepthMap != 0)
    {
        glDeleteTextures(1, &mDepthMap);
    }
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

    if (mKeyHandler.IsKeyDown(GLFW_KEY_KP_ADD))
    {
        mCamera.MovementSpeed += delta * 5;
    }

    if (mKeyHandler.IsKeyDown(GLFW_KEY_KP_SUBTRACT))
    {
        mCamera.MovementSpeed -= delta * 5;
    }
    
    // Handle normal strength
    if (mKeyHandler.IsKeyDown(GLFW_KEY_UP))
    {
        mNormalStrength = glm::clamp(mNormalStrength + mDeltaTime, 0.0, 1.0);
    }
    if (mKeyHandler.IsKeyDown(GLFW_KEY_DOWN))
    {
        mNormalStrength = glm::clamp(mNormalStrength - mDeltaTime, 0.0, 1.0);
    }

    if (mKeyHandler.WasKeyReleased(GLFW_KEY_B))
    {
        mDrawBoundingBoxes = !mDrawBoundingBoxes;
    }
    if (mKeyHandler.WasKeyPressed(GLFW_KEY_V))
    {
        mDrawAlbedo = !mDrawAlbedo;
    }
    // Handle MSAA
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_M))
    {
        EnableMultiSample(!mMultiSampleEnabled);
    }
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_1))
    {
        SetMutlisampleMode(GL_FASTEST);
    }
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_2))
    {
        SetMutlisampleMode(GL_NICEST);
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

    // Raycast
    if (mKeyHandler.WasKeyReleased(GLFW_KEY_R))
    {
        mCurrentRay = Ray(mCamera.Position, mCamera.Front);
        mCurrentRay.UpdateLine();
        if (mKdTree.mLastHit != 0)
        {
            mKdTree.mLastHit->mIsHit = false;
        }
        mKdTree.CheckIntersect(mCurrentRay);
        if (mKdTree.mLastHit != 0)
        {
            mKdTree.mLastHit->mIsHit = true;
            mCurrentRay.mHitDistance = mKdTree.mHitDistance;
            //mCurrentRay.UpdateLine();
            std::cout << "Distance: " << mKdTree.mHitDistance << std::endl;
            std::cout << "HitPosition: " << mKdTree.mLastHitPosition.x << ", " << mKdTree.mLastHitPosition.y << ", " << mKdTree.mLastHitPosition.z << std::endl;
            Cube* t = new Cube();
            t->SetMaterial(&mBoundingBoxMat, false);
            glm::mat4 model(1.0f);
            model = glm::translate(model, mKdTree.mLastHitPosition);
            model = glm::scale(model, glm::vec3(0.02f));
            t->SetModelMatrix(model);
            std::swap(t, mHitCube);
            delete t;
        }
    }
}

void EnableMultiSample(bool enabled)
{
    mMultiSampleEnabled = enabled;
    if (mMultiSampleEnabled)
    {
        glEnable(GL_MULTISAMPLE);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
    }
}

void SetMutlisampleMode(unsigned int mode)
{
    if (mMultiSample == mode)
    {
        return;
    }
    mMultiSample = mode;
    RecreateWindow();
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
