//
// Created by adrien on 29/01/23.
//

#include "../include/Texture.h"
#include <glad/glad.h> // helps loads drivers
#include "../lib/stb_image.h"
#include <iostream>

Texture::Texture(const char* texturePath) {
    int width, height, nrChannels;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Yo, an image failed to load. that's whack!!!" << std::endl;
    }
    stbi_image_free(imageData);
}
