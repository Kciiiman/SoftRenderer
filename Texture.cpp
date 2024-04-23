#include "Texture.h"
#include <iostream>
#define INV_SCALE 0.003921568627451

Texture::Texture(const std::string& path) {
	data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
}

Texture::~Texture() {
	if (data) {
		delete data;
	}
    //stbi_image_free(data);假如不释放的话，就不会报错吗

}

bool Texture::loadImage(const std::string& path) {
	if (data) {
		delete data;
	}
	data = nullptr;
	data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
	return  data != nullptr;
}

glm::vec4 Texture::getColor(const int& x, const int& y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return glm::vec4(0, 0, 0, 0);
	}
	int xy = nChannels * (y * width + x);
	return glm::vec4(*(data + xy), *(data + xy + 1), *(data + xy + 2), *(data + xy + 3));
}

//邻近采样
//glm::vec4 Texture::sample2D(const glm::vec2& texCoord) {
//	float x = texCoord.x - floor(texCoord.x);
//	float y = texCoord.y - floor(texCoord.y);
//	return getColor(x * (width - 1), y * (height - 1));
//}

//双线性插值采样
glm::vec4 Texture::sample2D(const glm::vec2& texCoord) {
    glm::vec4 result(0.0f, 0.0f, 0.0f, 1.0f);
    if (data == nullptr)
        return result;

    unsigned int x = 0, y = 0;
    float factorU = 0, factorV = 0;

    // calculate the corresponding coordinate.
    if (texCoord.x >= 0.0f && texCoord.x <= 1.0f && texCoord.y >= 0.0f && texCoord.y <= 1.0f)
    {
        float trueU = texCoord.x * (width - 1);
        float trueV = texCoord.y * (height - 1);
        x = static_cast<unsigned int>(trueU);
        y = static_cast<unsigned int>(trueV);
        factorU = trueU - x;
        factorV = trueV - y;
    }
    else
    {
        // repeating way.
        float u = texCoord.x, v = texCoord.y;
        if (texCoord.x > 1.0f)
            u = texCoord.x - static_cast<int>(texCoord.x);
        else if (texCoord.x < 0.0f)
            u = 1.0f - (static_cast<int>(texCoord.x) - texCoord.x);
        if (texCoord.y > 1.0f)
            v = texCoord.y - static_cast<int>(texCoord.y);
        else if (texCoord.y < 0.0f)
            v = 1.0f - (static_cast<int>(texCoord.y) - texCoord.y);

        float trueU = u * (width - 1);
        float trueV = v * (height - 1);
        x = static_cast<unsigned int>(trueU);
        y = static_cast<unsigned int>(trueV);
        factorU = trueU - x;
        factorV = trueV - y;
    }

    // Get texel colors
    glm::vec4 texelLB = getColor(x, y);
    glm::vec4 texelRB = getColor(x + 1, y);
    glm::vec4 texelLT = getColor(x, y + 1);
    glm::vec4 texelRT = getColor(x + 1, y + 1);

    // Bilinear interpolation
    glm::vec4 texelLerpH = glm::mix(texelLB, texelRB, factorU);
    glm::vec4 texelLerpV = glm::mix(texelLT, texelRT, factorU);

    result = glm::mix(texelLerpH, texelLerpV, factorV);

    return result;
}