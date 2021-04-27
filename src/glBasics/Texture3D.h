#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H

#include <glad/glad.h>

#include "Texture.h"

class Texture3D : public Texture
{
public:
    Texture3D(unsigned int tex = 0, unsigned int texLocation = GL_TEXTURE0)
        :Texture(tex, texLocation)
    {
    }

    ~Texture3D()
    {
    }

    void use() override
    {
        glActiveTexture(mTexLocation);
        glBindTexture(GL_TEXTURE_3D, mTex);
    }
};

// TEXTURE_3D_H
#endif