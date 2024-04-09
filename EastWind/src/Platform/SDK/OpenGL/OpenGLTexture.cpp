#include "EW_Core.h"
#include "EW_Log.h"
#include "EW_pch.h"

#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace EastWind {

  OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path)
  {
    int width, height, channels; 
    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    EW_ASSERT(data, "Failed to load image!!!");
    m_Width = width;
    m_Height = height;


    glGenTextures(1, &m_TextureID);

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    // set texture parameters
    /*
        glTexParameteri(GL_TEXTURE_2D, [1], [2]);
        - [1] = 
            - GL_TEXTURE_MIN_FILTER, -> when texture are scaled down
            - GL_TEXTURE_MAG_FILTER, -> when texture are scaled up
            - GL_TEXTURE_WRAP_S, -> how to handle texture coordinates outside the range [0, 1]
            - GL_TEXTURE_WRAP_T -> how to handle texture coordinates outside the range [0, 1]

        - [2] = 
            - GL_NEAREST, -> nearest pixel
            - GL_LINEAR, -> linear interpolation
            - GL_REPEAT, -> repeat texture
            - GL_CLAMP_TO_EDGE, -> clamp to edge
            - GL_MIRRORED_REPEAT -> mirror texture
            - GL_CLAMP_TO_BORDER -> clamp to border
            - GL_LINEAR_MIPMAP_LINEAR -> linear interpolation between mipmap levels
            - GL_NEAREST_MIPMAP_NEAREST -> nearest pixel between mipmap levels
            - GL_LINEAR_MIPMAP_NEAREST -> linear interpolation between mipmap levels
            - GL_NEAREST_MIPMAP_LINEAR -> nearest pixel between mipmap levels
    */
    glTexParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // set texture wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data); 
    // set texture data
    switch (channels)
    {
    case 4:
        EW_TRACE("Register 4 Channels Texture RGBA");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        EW_TRACE("Register 3 Channels Texture RGB");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case 1:
        EW_TRACE("Register 1 Channel Texture R");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        break;
    default:
        std::cout << "Error: Unsupported number of channels: " << channels << std::endl;
        break;
    }

    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
  }


  OpenGLTexture2D::~OpenGLTexture2D()
  {
    glDeleteTextures(1, &m_TextureID);
  }


  void OpenGLTexture2D::Bind(uint32_t slot) const 
  {
    glActiveTexture(slot+GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
  }

}
