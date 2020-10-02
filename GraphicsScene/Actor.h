#pragma once

class Actor
{
public:
	Actor() {}
	~Actor() {}

	virtual bool update(double deltaTime) { return true; }
	virtual bool draw() { return true; }
};

