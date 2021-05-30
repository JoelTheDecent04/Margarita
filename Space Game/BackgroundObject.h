#pragma once

class BackgroundObject
{

public:
	virtual void Draw() = 0;
	virtual void Update(double deltatime) = 0;
};