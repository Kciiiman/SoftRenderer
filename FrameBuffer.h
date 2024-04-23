#ifndef __FrameBuffer_H__
#define __FrameBuffer_H__
#include <vector>
#include "glm/glm.hpp"

class FrameBuffer {
private:
	int width, height;
	std::vector<float> depthBuffer;
public:
	std::vector<unsigned char> colorBuffer;

	FrameBuffer(const int& w, const int& h);
	~FrameBuffer() = default;

	int getWidth() { return width; }
	int getHeight() { return height; }
	unsigned char* getColorBuffer() { return colorBuffer.data(); }

	void resize(const int& w, const int& h);
	void fillColorBuffer(const glm::vec4& color);
	void drawPixel(const int& x, const int& y, const glm::vec4& color);

	float getDepth(const int& x, const int& y);
	void setDepth(const int& x, const int& y, const float& depth);
};


#endif