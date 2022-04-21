#include "Util.h"

void GraphicsUtil::moveLocal(glm::mat4& model, const glm::vec3& v) {
	model = model * glm::translate(glm::mat4(1), v);
}

std::string GeneralUtil::generateRandomString(int len) {
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(len, 0);
    std::generate_n(str.begin(), len, randchar);
    return str;
}