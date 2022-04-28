#pragma once
#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include <map>

struct OBB {
    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec2 p3;
    glm::vec2 p4;
};

class CollisionDetector {
private:
    std::map<int, OBB> obbSet;
    bool check(const OBB& obb1, const OBB& obb2);

public:
    CollisionDetector() = default;
    ~CollisionDetector() = default;
    void insert(const OBB& obb, const int key); 
    void update(const OBB& obb, const int key);
    bool checkCollision(const int key);
};

#endif
