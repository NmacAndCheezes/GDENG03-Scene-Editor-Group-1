#pragma once
#include "../GameEngine/Graphics/GraphicsEngine.h"


class AD3D11Object
{
public:
	AD3D11Object(GraphicsEngine* gfx) : gfx(gfx) {};
	~AD3D11Object() {};

	virtual bool Init() = 0;
	virtual void BindToPipeline() = 0;
	virtual bool Release() = 0;


protected:
	GraphicsEngine* gfx;
};