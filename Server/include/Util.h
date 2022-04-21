#pragma once
#include "Graphics/include/PrimitiveMesh.h"
#include <string>
#include <algorithm>  //for std::generate_n


namespace GraphicsUtil {
	void moveLocal(glm::mat4& model, const glm::vec3& v);
}

// Generic Util functions
namespace GeneralUtil {
	std::string generateRandomString(int len);
}