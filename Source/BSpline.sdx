#include <GL/glew.h>
#include "Model.h"
#include <vector>
using namespace std;
using namespace glm;

class BSpline : public Model{
public:
	BSpline(vec3 color = vec3(0.0f, 0.0f, 1.0f));
	virtual~BSpline(void);

	virtual void Draw();
	virtual void Update(float dt);

	float* GetPoint(const int& i, float(&cpts)[92][3]);
	static void setFunctions();

	vector<vec3> Spline;
protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	struct Vertex{
		glm::vec3 position;// the control points for the curve
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int numOfVertices;	//*/
	int noOfPoints;
	unsigned int LOD;
};