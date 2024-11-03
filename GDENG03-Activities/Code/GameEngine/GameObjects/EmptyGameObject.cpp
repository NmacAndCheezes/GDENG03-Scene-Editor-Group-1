#include "EmptyGameObject.h"


// constructor and destructor of the EmptyGameObject Class
EmptyGameObject::EmptyGameObject(std::string name) : AGameObject(name)
{

}


EmptyGameObject::~EmptyGameObject()
{

}


// public methods of the EmptyGameObject Class
void EmptyGameObject::Initialize()
{
	isInitialized = true;
}