#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#if _WIN32
#define  GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <memory>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define	STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include <base/Path.h>
#include <base/File.h>
#include <base/UnicodeUtil.h>
#include <base/Time.h>
#include <base/PMDModel.h>
#include <base/PMXModel.h>
#include <base/VMDFile.h>
#include <base/VMDAnimation.h>
#include <base/VMDCameraAnimation.h>
int main(void)
{
	return 0;
}