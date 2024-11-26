#include "AComponent.h"
#include "../GameObjects/AGameObject.h"


// constructor and destructor of the AComponent Class
AComponent::AComponent(std::string name, EComponentTypes type)
{
	this->name = name;
	this->owner = NULL;
	this->type = type;
	this->isEnabled = true;
}

AComponent::~AComponent()
{
	this->owner = NULL;
	this->type = EComponentTypes::NotSet;
}

void AComponent::Initialize()
{
}

void AComponent::Clone(AComponent* copy)
{
}

void AComponent::Perform()
{
}

void AComponent::RenderUI()
{
}


// public methods of the AComponent Class
void AComponent::AttachOwner(AGameObject* object)
{
	this->owner = object;
	this->transform = object->GetTransform();
}


void AComponent::DetachOwner()
{
	delete this;
}


AGameObject* AComponent::GetOwner()
{
	return this->owner;
}

Transform* AComponent::GetTransform()
{
	return this->transform;
}


EComponentTypes AComponent::GetType()
{
	return this->type;
}


std::string AComponent::GetName()
{
	return this->name;
}

bool AComponent::IsEnabled()
{
	return isEnabled;
}

void AComponent::SetEnabled(bool flag)
{
	isEnabled = flag;
}

void AComponent::SetDeltaTime(float deltaTime)
{
	this->dt = deltaTime;
}