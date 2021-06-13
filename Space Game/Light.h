#pragma once
#include "Entity.h"
#include <memory>

class Light : public Entity
{
public:
	std::shared_ptr<Entity> parent;

	Light(std::shared_ptr<Entity> parent);
	bool Update(double deltatime) override;
	void Save(std::fstream& f) override { f << (int)nType << " "; };
	void Load(std::fstream& f) override {};
};