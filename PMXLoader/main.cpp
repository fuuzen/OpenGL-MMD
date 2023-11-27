#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <windows.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/assimploader.hpp>

int main()
{
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texIndex;
	std::vector<std::string> texs;
	std::vector<std::string> toons;
	bool res = loadAssImp("../model/hutao/hutao.pmx", indices, vertices, uvs, normals, texIndex, texs, toons);

	OutputDebugStringW((L"顶点数：" + std::to_wstring(vertices.size()) + L"\n").c_str());
	OutputDebugStringW((L"面数：" + std::to_wstring(indices.size() / 3) + L"\n").c_str());

	return 0;
}