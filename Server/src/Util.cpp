#include "Util.h"

void GraphicsUtil::moveLocal(glm::mat4& model, const glm::vec3& v) {
	model = model * glm::translate(glm::mat4(1), v);
}

