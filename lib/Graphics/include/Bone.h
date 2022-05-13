#pragma once
#ifndef BONE_H
#define BONE_H

#include "Model.h"

struct KeyPosition {
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation {
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale {
    glm::vec3 scale;
    float timeStamp;
};

class Bone{
private:
    std::string m_Name;
    glm::mat4 m_LocalTransform;
    int m_ID;
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 Bone::InterpolateScaling(float animationTime);

public:
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);
    ~Bone() = default;

    glm::mat4 GetLocalTransform(){ 
        return m_LocalTransform;
    }
    std::string GetBoneName() const{
        return m_Name;
    }
    int GetBoneID(){ 
        return m_ID;
    }

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

    void update(float animationTime);

};
#endif // !BONE_H
