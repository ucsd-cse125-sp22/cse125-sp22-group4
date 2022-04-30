#include "CollisionDetector.h"

unsigned int CollisionDetector::currID = 0;

int CollisionDetector::insert(const OBB& obb) {
    obbSet[currID] = obb;
    int prevID = currID;
    ++currID;
    return prevID;
}

void CollisionDetector::update(const OBB& obb, const int ID) {
    obbSet[ID] = obb;
}

int CollisionDetector::check(const int ID) {
    OBB& obb = obbSet[ID];
    for (auto& element : obbSet) {
        if (element.first != ID) {
            if (check(obb, element.second)) {
                return element.first;
            }
        }
    }

    return -1;
}

bool CollisionDetector::check(const OBB& obb1, const OBB& obb2) {
    std::vector<glm::vec2> axes;
    axes.push_back(glm::normalize(obb1.p1 - obb1.p2));
    axes.push_back(glm::normalize(obb1.p1 - obb1.p4));
    axes.push_back(glm::normalize(obb2.p1 - obb2.p2));
    axes.push_back(glm::normalize(obb2.p1 - obb2.p4));

    float max1, min1, max2, min2;
    float proj1, proj2;
    // project all 4 points and check if max min overlap
    for (auto& axis : axes) {
        max1 = max2 = -FLT_MAX;
        min1 = min2 = FLT_MAX;
        // p1
        proj1 = glm::dot(obb1.p1, axis);
        proj2 = glm::dot(obb2.p1, axis);
        max1 = max1 > proj1 ? max1 : proj1;
        max2 = max2 > proj2 ? max2 : proj2;
        min1 = min1 < proj1 ? min1 : proj1;
        min2 = min2 < proj2 ? min2 : proj2;
        // p2
        proj1 = glm::dot(obb1.p2, axis);
        proj2 = glm::dot(obb2.p2, axis);
        max1 = max1 > proj1 ? max1 : proj1;
        max2 = max2 > proj2 ? max2 : proj2;
        min1 = min1 < proj1 ? min1 : proj1;
        min2 = min2 < proj2 ? min2 : proj2;
        // p3
        proj1 = glm::dot(obb1.p3, axis);
        proj2 = glm::dot(obb2.p3, axis);
        max1 = max1 > proj1 ? max1 : proj1;
        max2 = max2 > proj2 ? max2 : proj2;
        min1 = min1 < proj1 ? min1 : proj1;
        min2 = min2 < proj2 ? min2 : proj2;
        // p4
        proj1 = glm::dot(obb1.p4, axis);
        proj2 = glm::dot(obb2.p4, axis);
        max1 = max1 > proj1 ? max1 : proj1;
        max2 = max2 > proj2 ? max2 : proj2;
        min1 = min1 < proj1 ? min1 : proj1;
        min2 = min2 < proj2 ? min2 : proj2;

        if (min2 > max1 || max2 < min1) {
            return false;
        }
    }

    // collision if all axis projection contains overlap
    return true;
}

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

