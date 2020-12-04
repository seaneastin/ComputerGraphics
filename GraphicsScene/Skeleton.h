#pragma once
#include "Actor.h"
#include "Bone.h"
#include <vector>

/**
 * skeleton is an actor that contatains a list of bones that can be added to the skeleton
 */
class Skeleton : public Actor
{
public:
	Skeleton();
	~Skeleton() {}

	bool update(double deltaTime) override;
	bool draw() override;

	void addBone(Bone* bone);

private:
	std::vector<Bone*> m_bones;
};

