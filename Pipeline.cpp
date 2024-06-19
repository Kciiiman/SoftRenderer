#include "Pipeline.h"

Pipeline::Pipeline(int width, int height, RenderMode m)
	:width(width), height(height)
	, shader(nullptr), m_frontBuffer(nullptr)
	, m_backBuffer(nullptr), mode(m)
{
	m_eyePos = glm::vec3(0.0f, 0.0f, 0.0f);
}

Pipeline::~Pipeline() {
	if (shader)delete shader;
	if (m_frontBuffer)delete m_frontBuffer;
	if (m_backBuffer)delete m_backBuffer;
	shader = nullptr;
	m_frontBuffer = nullptr;
	m_backBuffer = nullptr;

	for (unsigned int x = 0; x < m_Textures.size(); ++x)
	{
		delete m_Textures[x];
		m_Textures[x] = nullptr;
	}
}

void Pipeline::init() {
	m_Camera = nullptr;
	m_material = nullptr;

	if (m_frontBuffer)
		delete m_frontBuffer;
	if (m_backBuffer)
		delete m_backBuffer;
	if (shader) {
		delete shader;
	}
	viewPortMatrix = getViewPortMatrix(0, 0, width, height);
	shader = new BaseShader;
	m_frontBuffer = new FrameBuffer(width, height);
	m_backBuffer = new FrameBuffer(width, height);
}


bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = (unsigned char*)stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	stbi_set_flip_vertically_on_load(true);
	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

void SaveColorBufferToFile(const char* filename, const std::vector<unsigned char>& colorBuffer, int width, int height) {
	stbi_write_png(filename, width, height, 4, colorBuffer.data(), width * 4);
}

void Pipeline::display() {
	//glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer->colorBuffer.data());
	SaveColorBufferToFile("/Users/kciiiman/Desktop/SoftRenderer/out/output.png", m_backBuffer->colorBuffer, width, height);
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("/Users/kciiiman/Desktop/SoftRenderer/out/output.png", &my_image_texture, &width, &height);
	IM_ASSERT(ret);

	updateCamera();

	ImGui::Begin("veiwport");
	ImGui::Text("pointer = %x", my_image_texture);
	ImGui::Text("size = %d x %d", width, height);
	ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(width, height));
	ImGui::End();
}


void Pipeline::resize(const int& w, const int& h) {
	width = w;
	height = h;
	m_backBuffer->resize(w, h);
	viewPortMatrix = getViewPortMatrix(0, 0, w, h);

	updateCamera();
}

void Pipeline::fillColorBuffer(const glm::vec4& color) {
	m_backBuffer->fillColorBuffer(color);
}

unsigned int Pipeline::loadTexture(const std::string& path)
{
	Texture* tex = new Texture();
	if (!tex->loadImage(path))
		return 0;

	m_Textures.push_back(tex);
	return m_Textures.size() - 1;

}

bool Pipeline::bindTexture(const unsigned int& unit)
{
	if (unit >= m_Textures.size())
		return false;
	shader->bindTexture(m_Textures[unit]);
	return true;
}

bool Pipeline::unbindTexture(const unsigned int& unit)
{
	if (unit >= m_Textures.size())
		return false;
	shader->bindTexture(nullptr);
	return true;
}

void Pipeline::setMaterial(const Material* material)
{
	m_material = material;
	shader->setMaterial(m_material);
}

void Pipeline::setModelMatrix(const glm::mat4& m) {
	shader->setModelMatrix(m);
}

void Pipeline::setViewMatrix(const glm::mat4& m) {
	shader->setViewMatrix(m);
}

void Pipeline::setProjectMatrix(const glm::mat4& m) {
	shader->setProjectMatrix(m);
}

void Pipeline::setDirectionLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
	m_dirLight.setDirectionalLight(amb, diff, spec, dir);
	shader->setLight(&m_dirLight);
}

void Pipeline::setPointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 atte)
{
	m_pointLight.setPointLight(amb, diff, spec, pos, atte);
	shader->setLight(&m_pointLight);
}

void Pipeline::setSpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 atte, double cut)
{
	m_spotLight.setSpotLight(amb, diff, spec, cut, pos, dir, atte);
	shader->setLight(&m_spotLight);
}

void Pipeline::setCamera(Camera* camera)
{
	if (m_Camera)
		delete m_Camera;
	m_Camera = camera;
	updateCamera();
}

void Pipeline::updateCamera()
{
	m_Camera->updateAspact(width, height);
	shader->setViewMatrix(m_Camera->viewMatrix());
	shader->setProjectMatrix(m_Camera->perspectiveMatrix());
}

void Pipeline::swapBuffer()
{
	FrameBuffer* tmp = m_frontBuffer;
	m_frontBuffer = m_backBuffer;
	m_backBuffer = tmp;
}

void Pipeline::setShaderMode(ShadingMode ShaderMode)
{
	if (shader)delete shader;
	shader = nullptr;
	if (ShaderMode == ShadingMode::Simple)
		shader = new BaseShader();
	else if (ShaderMode == ShadingMode::Gouraud)
		shader = new GouraudShader();
	else if (ShaderMode == ShadingMode::Phong)
		shader = new PhongShader();
}

//now

void Pipeline::bresenhamLineRasterization(const Vertex& from, const Vertex& to)
{
	int dx = to.windowPos.x - from.windowPos.x;
	int dy = to.windowPos.y - from.windowPos.y;
	int stepX = 1, stepY = 1;

	// judge the sign
	if (dx < 0)
	{
		stepX = -1;
		dx = -dx;
	}
	if (dy < 0)
	{
		stepY = -1;
		dy = -dy;
	}

	int d2x = 2 * dx, d2y = 2 * dy;
	int d2y_minus_d2x = d2y - d2x;
	int sx = from.windowPos.x;
	int sy = from.windowPos.y;

	Vertex tmp;
	// slope < 1.
	if (dy <= dx)
	{
		int flag = d2y - dx;
		for (int i = 0; i <= dx; ++i)
		{
			// linear interpolation
			tmp = Lerp(from, to, static_cast<double>(i) / dx);

			// depth testing.
			double depth = m_backBuffer->getDepth(sx, sy);
			if (tmp.windowPos.z > depth)
				continue;// fail to pass the depth testing.
			m_backBuffer->setDepth(sx, sy, tmp.windowPos.z);

			// fragment shader
			m_backBuffer->drawPixel(sx, sy, shader->fragmentShader(tmp));
			sx += stepX;
			if (flag <= 0)
				flag += d2y;
			else
			{
				sy += stepY;
				flag += d2y_minus_d2x;
			}
		}
	}
	// slope > 1.
	else
	{
		int flag = d2x - dy;
		for (int i = 0; i <= dy; ++i)
		{
			// linear interpolation
			tmp = Lerp(from, to, static_cast<double>(i) / dy);
			// fragment shader
			m_backBuffer->drawPixel(sx, sy, shader->fragmentShader(tmp));
			sy += stepY;
			if (flag <= 0)
				flag += d2x;
			else
			{
				sx += stepX;
				flag -= d2y_minus_d2x;
			}
		}
	}
}

void Pipeline::scanLinePerRow(const Vertex& left, const Vertex& right)
{
	Vertex current;
	int length = right.windowPos.x - left.windowPos.x + 1;
	for (int i = 0; i <= length; ++i)
	{
		// linear interpolation
		double weight = static_cast<double>(i) / length;
		current = Lerp(left, right, weight);
		current.windowPos.x = left.windowPos.x + i;
		current.windowPos.y = left.windowPos.y;

		// depth testing.
		double depth = m_backBuffer->getDepth(current.windowPos.x, current.windowPos.y);
		if (current.windowPos.z > depth)
			continue;// fail to pass the depth testing.
		m_backBuffer->setDepth(current.windowPos.x, current.windowPos.y, current.windowPos.z);

		if (current.windowPos.x < 0 || current.windowPos.y < 0)
			continue;
		if (current.windowPos.x >= width || current.windowPos.y >= height)
			break;

		current.worldPos /= current.oneDivZ;
		current.texCoord /= current.oneDivZ;
		current.color /= current.oneDivZ;

		// fragment shader
		m_backBuffer->drawPixel(current.windowPos.x, current.windowPos.y,
			shader->fragmentShader(current));
	}
}

void Pipeline::rasterTopTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{
	Vertex left = v2;
	Vertex right = v3;
	Vertex dest = v1;
	Vertex tmp, newleft, newright;
	if (left.windowPos.x > right.windowPos.x)
	{
		tmp = left;
		left = right;
		right = tmp;
	}
	int dy = left.windowPos.y - dest.windowPos.y + 1;

	for (int i = 0; i < dy; ++i)
	{
		double weight = 0;
		if (dy != 0)
			weight = static_cast<double>(i) / dy;
		newleft = Lerp(left, dest, weight);
		newright = Lerp(right, dest, weight);
		newleft.windowPos.y = newright.windowPos.y = left.windowPos.y - i;
		scanLinePerRow(newleft, newright);
	}
}

void Pipeline::rasterBottomTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{
	Vertex left = v1;
	Vertex right = v2;
	Vertex dest = v3;
	Vertex tmp, newleft, newright;
	if (left.windowPos.x > right.windowPos.x)
	{
		tmp = left;
		left = right;
		right = tmp;
	}
	int dy = dest.windowPos.y - left.windowPos.y + 1;


	for (int i = 0; i < dy; ++i)
	{
		double weight = 0;
		if (dy != 0)
			weight = static_cast<double>(i) / dy;
		newleft = Lerp(left, dest, weight);
		newright = Lerp(right, dest, weight);
		newleft.windowPos.y = newright.windowPos.y = left.windowPos.y + i;
		scanLinePerRow(newleft, newright);
	}
}

void Pipeline::edgeWalkingFillRasterization(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	// split the triangle into two part
	Vertex tmp;
	Vertex target[3] = { v1, v2,v3 };
	if (target[0].windowPos.y > target[1].windowPos.y)
	{
		tmp = target[0];
		target[0] = target[1];
		target[1] = tmp;
	}
	if (target[0].windowPos.y > target[2].windowPos.y)
	{
		tmp = target[0];
		target[0] = target[2];
		target[2] = tmp;
	}
	if (target[1].windowPos.y > target[2].windowPos.y)
	{
		tmp = target[1];
		target[1] = target[2];
		target[2] = tmp;
	}

	// bottom triangle
	if (equal(target[0].windowPos.y, target[1].windowPos.y))
	{
		rasterBottomTriangle(target[0], target[1], target[2]);
	}
	// top triangle
	else if (equal(target[1].windowPos.y, target[2].windowPos.y))
	{
		rasterTopTriangle(target[0], target[1], target[2]);
	}
	// split it.
	else
	{
		double weight = static_cast<double>(target[1].windowPos.y - target[0].windowPos.y) / (target[2].windowPos.y - target[0].windowPos.y);
		Vertex newPoint = Lerp(target[0], target[2], weight);
		newPoint.windowPos.y = target[1].windowPos.y;
		rasterTopTriangle(target[0], newPoint, target[1]);
		rasterBottomTriangle(newPoint, target[1], target[2]);
	}
}

void Pipeline::drawIndex(RenderMode mode, const Mesh* mesh)
{
	// renderer pipeline.
	bool line1 = false, line2 = false, line3 = false;
	this->mode = mode;
	int size = mesh->EBO.size();
	if (size == 0)return;

	for (unsigned int i = 0; i < size; i += 3)
	{
		//! assembly to triangle primitive.
		RawVertex p1, p2, p3;
		{

			p1 = *(mesh->VBO.data() + *(mesh->EBO.data() + i));
			p2 = *(mesh->VBO.data() + *(mesh->EBO.data() + i + 1));
			p3 = *(mesh->VBO.data() + *(mesh->EBO.data() + i + 2));
		}

		//! vertex shader stage.
		Vertex v1, v2, v3;
		{
			v1 = shader->vertexShader(p1);
			v2 = shader->vertexShader(p2);
			v3 = shader->vertexShader(p3);
		}

		//! back face culling.
		{
			if (backFaceCutting(v1, v2, v3)) continue;
		}

		//! geometry cliping.
		{

		}

		//! perspective division.
		{
			perspectiveDivision(v1);
			perspectiveDivision(v2);
			perspectiveDivision(v3);
		}

		//! view port transformation.
		{
			v1.windowPos = viewPortMatrix * v1.windowPos;
			v2.windowPos = viewPortMatrix * v2.windowPos;
			v3.windowPos = viewPortMatrix * v3.windowPos;
		}


		//! rasterization and fragment shader stage.
		{
			if (mode == RenderMode::LINE)
			{
				if (!line1)
					bresenhamLineRasterization(v1, v2);
				if (!line2)
					bresenhamLineRasterization(v2, v3);
				if (!line3)
					bresenhamLineRasterization(v3, v1);
			}
			else if (mode == RenderMode::MESH)
			{
				edgeWalkingFillRasterization(v1, v2, v3);
			}
			//drawTriangleBoundary(v1, v2, v3);
		}
	}
}

//ʹ����������ϵ, ���������ΰ�Χ�����
void Pipeline::drawTriangleBoundary(const Vertex & v1, const Vertex & v2, const Vertex & v3) {

	// �����Χ��
	int min_x = triMin(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x);
	int min_y = triMin(v1.windowPos.y, v2.windowPos.y, v3.windowPos.y);
	int max_x = triMax(v1.windowPos.x, v2.windowPos.x, v3.windowPos.x) + 1;
	int max_y = triMax(v1.windowPos.y, v2.windowPos.y, v3.windowPos.y) + 1;

	// ������Χ���еĵ�
#pragma omp parallel for collapse(2)
	for (int x = min_x; x < max_x; x++) {
		for (int y = min_y; y < max_y; y++) {
			glm::vec3 barycentric = getBarycentric(v1, v2, v3, glm::vec2(x, y)); // ������������
			if (barycentric[0] >= 0 && barycentric[1] >= 0 && barycentric[2] >= 0) { // �����������ڲ�
				Vertex current = Lerp(v1, v2, v3, barycentric);
				// depth testing.
				double depth = m_backBuffer->getDepth(current.windowPos.x, current.windowPos.y);
				if (current.windowPos.z > depth)
					continue;// fail to pass the depth testing.
				m_backBuffer->setDepth(current.windowPos.x, current.windowPos.y, current.windowPos.z);

				current.worldPos /= current.oneDivZ;
				current.texCoord /= current.oneDivZ;
				current.color /= current.oneDivZ;

				// fragment shader
				m_backBuffer->drawPixel(current.windowPos.x, current.windowPos.y,
					shader->fragmentShader(current));
				
			}
		}
	}
}

void Pipeline::add(Model* object) {
	m_Models.push_back(object);
}

void Pipeline::drawObject(const Model* obj) {
	if (obj->EBO.empty()) {
		return;
	}
	drawIndex(RenderMode::MESH, obj);
}