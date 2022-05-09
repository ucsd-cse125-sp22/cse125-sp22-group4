#pragma once
#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include <map>
#include <vector>

struct OBB {
    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec2 p3;
    glm::vec2 p4;
};

class CollisionDetector {
private:
    static unsigned int currID;
    std::map<int, OBB> obbSet;

public:
    CollisionDetector() = default;
    ~CollisionDetector() = default;
    int insert(const OBB& obb); 
    void update(const OBB& obb, const int ID);
    std::vector<int> check(const int ID);
    static bool check(const OBB& obb1, const OBB& obb2);
    static OBB computeOBB(float maxX, float maxZ,
                          float minX, float minZ,
                          const glm::mat4& model);
    static OBB computeOBB(const OBB& oldOBB, const glm::mat4& model);
    std::map<int, OBB> getObbset() {
        return obbSet;
    }
};

#endif
