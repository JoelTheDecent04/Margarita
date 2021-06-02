#pragma once

class BackgroundObject
{

public:
	virtual void Draw() = 0;
	virtual bool Update(double deltatime) = 0;
};