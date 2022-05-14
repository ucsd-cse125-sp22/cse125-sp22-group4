#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"

class Animator {
private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;

public:
    Animator(Animation* Animation);
    ~Animator();

    void update(float dt);
    void play(Animation* pAnimation);

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4> GetFinalBoneMatrices() {
        return m_FinalBoneMatrices;
    }
};
#endif // !ANIMATOR_H
