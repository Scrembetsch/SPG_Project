#include "Util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int Util::LoadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

bool Util::LoadObjFile(std::string path, std::vector<Node*>& triangles, glm::mat4& matrix)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    std::ifstream file(path);

    std::string line;
    char buffer = ' ';
    double x, y, z;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);

        std::string lineHeader;
        std::getline(lineStream, lineHeader, ' ');
        // else : parse lineHeader
        if (lineHeader == "v")
        {
            lineStream >> x >> buffer >> y >> buffer >> z;
            temp_vertices.push_back(glm::vec3(x, y, z));
        }
        else if (lineHeader == "vt")
        {
            lineStream >> x >> buffer >> y;
            temp_uvs.push_back(glm::vec2(x, y));
        }
        else if (lineHeader == "vn")
        {
            lineStream >> x >> buffer >> y >> buffer >> z;
            temp_normals.push_back(glm::vec3(x, y, z));
        }
        else if (lineHeader == "f")
        {
            int vx, vy, vz;
            int ux, uy, uz;
            int nx, ny, nz;
            lineStream >> vx >> buffer >> ux >> buffer >> nx;
            lineStream >> vy >> buffer >> uy >> buffer >> ny;
            lineStream >> vz >> buffer >> uz >> buffer >> nz;
            vertexIndices.push_back(vx);
            vertexIndices.push_back(vy);
            vertexIndices.push_back(vz);
            uvIndices.push_back(ux);
            uvIndices.push_back(uy);
            uvIndices.push_back(uz);
            normalIndices.push_back(nx);
            normalIndices.push_back(ny);
            normalIndices.push_back(nz);
        }
    }
    Triangle* tri = new Triangle();
    int aC = 0;
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        tri->mVertices[aC++] = glm::vec4(vertex, 1.0f);
        if (aC == 3)
        {
            tri->SetMaterial(new Material(), true);
            tri->SetModelMatrix(matrix);
            triangles.push_back(tri);
            tri->UpdateArrayBuffers();
            tri = new Triangle();
            aC = 0;
        }
    }
    return true;
}
