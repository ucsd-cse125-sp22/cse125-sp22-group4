#include "CollisionDetector.h"

OBB CollisionDetector::computeOBB(float maxX, float maxZ,
    float minX, float minZ,
    const glm::mat4& model) {
    glm::vec4 p1(maxX, 0, maxZ, 1);
    glm::vec4 p2(minX, 0, maxZ, 1);
    glm::vec4 p3(minX, 0, minZ, 1);
    glm::vec4 p4(maxX, 0, minZ, 1);

    p1 = model * p1;
    p2 = model * p2;
    p3 = model * p3;
    p4 = model * p4;

    OBB obb{
        {p1.x, p1.z},
        {p2.x, p2.z},
        {p3.x, p3.z},
        {p4.x, p4.z}
    };

    return obb;
}

