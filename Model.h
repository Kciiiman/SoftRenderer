#include "Mesh.h"
#include <string>

class Model:public Mesh
{
private:
	glm::vec3 minPoint, maxPoint; //// Bounding box
public:
	Model(const std::string& path);
	virtual ~Model();
	// Size setting.
	glm::mat4 setSize(double sx, double sy, double sz) const;

private:
	// Obj file loader.
	void loadObjFile(const std::string& path);
};