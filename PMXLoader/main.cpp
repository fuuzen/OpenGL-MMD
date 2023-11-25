#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <windows.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

int main()
{
	std::string path = "model/hutao/hutao.pmx";
	std::wstring wpath(path.begin(), path.end());
	OutputDebugStringW((L"加载模型：" + wpath + L"\n").c_str());
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		OutputDebugStringA(importer.GetErrorString());
		return 0;
	}
	OutputDebugString(L"加载模型成功\n");
	// mRootNode：根节点，mMetaData：元数据
	// mMeshes：网格，mMaterials：材质
	int vericesNum = 0, facesNum = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		OutputDebugStringW((L"网格" + std::to_wstring(i) + L"顶点数：" + std::to_wstring(scene->mMeshes[i]->mNumVertices)).c_str());
		OutputDebugStringW((L" 面数：" + std::to_wstring(scene->mMeshes[i]->mNumFaces) + L"\n").c_str());
		vericesNum += scene->mMeshes[i]->mNumVertices;
		facesNum += scene->mMeshes[i]->mNumFaces;
	}
	OutputDebugString(std::to_wstring(vericesNum).c_str());
	OutputDebugString(L" ");
	OutputDebugString(std::to_wstring(facesNum).c_str());
	OutputDebugString(L"\n");
	return 0;
}