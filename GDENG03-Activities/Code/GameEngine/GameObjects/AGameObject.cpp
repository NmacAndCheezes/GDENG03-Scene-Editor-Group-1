#include "AGameObject.h"
#include "../Components/Inputs/GenericInputController.h"
#include "../Components/Renderer/ARenderer.h"


int AGameObject::currentID = 0;

#pragma region Constructor-Destructor
AGameObject::AGameObject(std::string name)
{
	this->instanceID = currentID; currentID++;
	this->name = name;
	this->parent = NULL;
	this->enabled = true;
	this->isInitialized = false;
	this->level = 0;

	transform = new Transform();
	AttachComponent(transform);
}

AGameObject::~AGameObject()
{
	for (size_t i = 0; i < this->childList.size(); i++)
	{
		delete this->childList[i];
	}

	for (size_t i = 0; i < this->componentList.size(); i++) 
	{
		delete this->componentList[i]; 
	}

	transform = nullptr;

	this->childList.clear();
	this->childList.shrink_to_fit(); 
	this->componentList.clear(); 
	this->componentList.shrink_to_fit(); 
	 
	if (this->parent != NULL) 
	{
		this->parent = NULL; 
	}
}
#pragma endregion


#pragma region Game-related methods
bool AGameObject::IsInitialized()
{
	return isInitialized;
}

//void AGameObject::ProcessInputs(WPARAM wParam, LPARAM lParam)
//{
//	if (!this->enabled) return;
//
//	std::vector<AComponent*> componentList = this->GetComponentsOfType(EComponentTypes::Input);
//
//	for (size_t i = 0; i < componentList.size(); i++)
//	{
//		GenericInputController* inputController = (GenericInputController*)componentList[i];
//		if (inputController->Enabled)
//		{
//			inputController->ProcessInputs(wParam, lParam);
//			inputController->Perform(); 
//		}
//	}
//
//	for (size_t i = 0; i < this->childList.size(); i++)
//	{
//		this->childList[i]->ProcessInputs(wParam, lParam);
//	}
//}

void AGameObject::Update(float dt) 
{
	if (!this->enabled) return;

	std::vector<AComponent*> componentList = this->GetComponentsOfType(EComponentTypes::Script); 

	for (size_t i = 0; i < componentList.size(); i++)
	{
		if (componentList[i]->Enabled)
		{
			componentList[i]->SetDeltaTime(dt); 
			componentList[i]->Perform(); 
		}
	}

	for (size_t i = 0; i < childList.size(); i++)
	{
		childList[i]->Update(dt);  
	}
}

void AGameObject::Draw()
{
	if (!this->enabled) return;

	std::vector<AComponent*> rendererList = this->GetComponentsOfType(EComponentTypes::Renderer); 

	for (size_t i = 0; i < rendererList.size(); i++) 
	{
		ARenderer* renderer = (ARenderer*)rendererList[i];
		if (renderer->Enabled) renderer->Perform();
	}

	for (size_t i = 0; i < childList.size(); i++) 
	{
		this->childList[i]->Draw(); 
	}
}

unsigned int AGameObject::GetInstanceID()
{
	return this->instanceID;
}

std::string AGameObject::GetName()
{
	return this->name;
}

void AGameObject::SetName(std::string newName)
{
	this->name = newName;
}

bool AGameObject::IsEnabled()
{
	return enabled;
}

void AGameObject::SetEnabled(bool flag)
{
	enabled = flag;

	for (size_t i = 0; i < childList.size(); i++)
	{
		this->childList[i]->SetEnabled(flag);
	}
}
#pragma endregion


#pragma region Inheritance-related methods
void AGameObject::AttachChild(AGameObject* child)
{
	if (child == this || child == nullptr) return;

	if (child->parent != nullptr)
	{
		child->parent->DetachChild(child);
	}

	this->childList.push_back(child); 
	child->SetParent(this);
	child->level = this->level + 1;
	if (!enabled) child->SetEnabled(false);

	if (!child->isInitialized) child->Initialize(); 
	child->transform->RecalculateChildTransformWithParent(this->transform);
}

void AGameObject::DetachChild(AGameObject* child)
{
	if (child == this || child == nullptr) return;

	int index = -1;

	for (size_t i = 0; i < this->childList.size(); i++)
	{
		if (this->childList[i] == child) 
		{
			index = (int)i; 
		}
	}

	if (index != -1)
	{
		this->childList.erase(this->childList.begin() + index); 
	}

	child->level = 0; 
	child->SetParent(NULL); 
	child->transform->RecalculateChildTransformWithoutParent();

	//this->childList.erase(std::remove(this->childList.begin(), this->childList.end(), child), this->childList.end());
}

int AGameObject::GetLevel()
{
	return this->level;
}

AGameObject* AGameObject::GetParent()
{
	return this->parent;
}

std::vector<AGameObject*> AGameObject::GetChildList()
{
	return this->childList;
}

void AGameObject::SetParent(AGameObject* parent) 
{
	this->parent = parent; 
}
#pragma endregion


#pragma region Component-related methods
Transform* AGameObject::GetTransform()
{
	return transform;
}

void AGameObject::AttachComponent(AComponent* component)
{
	if (component->GetType() == EComponentTypes::NotSet ||
		(component->GetType() == EComponentTypes::Transform && component != transform)) return;

	auto itr = std::find_if(componentList.begin(), componentList.end(), [&component](AComponent* componentInList)
		{
			return (component == componentInList);
		});
	if (itr != componentList.end()) return;

	this->componentList.push_back(component);  
	component->AttachOwner(this);  
}

void AGameObject::DetachComponent(AComponent* component)
{
	int index = -1; 

	for (size_t i = 0; i < this->componentList.size(); i++) 
	{
		if (this->componentList[i] == component) 
		{
			index = (int)i; 
			this->componentList[i]->DetachOwner(); 
			break;
		}
	}

	if (index != -1) 
	{
		this->componentList.erase(this->componentList.begin() + index); 
	}
}

AComponent* AGameObject::FindComponentByName(std::string name)
{
	for (size_t i = 0; i < componentList.size(); i++) 
	{
		if (this->componentList[i]->GetName() == name) 
		{
			return this->componentList[i]; 
		}
	}

	return NULL;
}

AComponent* AGameObject::FindComponentByType(EComponentTypes type, std::string name)
{
	for (size_t i = 0; i < componentList.size(); i++)
	{
		if (this->componentList[i]->GetType() == type && this->componentList[i]->GetName() == name) 
		{
			return this->componentList[i]; 
		}
	}

	return NULL; 
}

std::vector<AComponent*> AGameObject::GetComponentsOfType(EComponentTypes type)
{
	if (type == EComponentTypes::Transform)
	{
		return { transform };
	}

	std::vector<AComponent*> foundList; 

	for (size_t i = 0; i < this->componentList.size(); i++)
	{
		if (this->componentList[i]->GetType() == type)
		{
			foundList.push_back(this->componentList[i]); 
		}
	}

	return foundList; 
}

std::vector<AComponent*> AGameObject::GetComponentsInChildrenOfType(EComponentTypes type)
{
	std::vector<AComponent*> foundList;

	for (int i = 0; i < childList.size(); i++)
	{
		std::vector<AComponent*> foundListInChild = childList[i]->GetComponentsOfType(type); ;
		
		/*std::vector<AComponent*> foundListInChild = (childList[i]->childList.size() > 0) ? 
			childList[i]->GetComponentsInChildrenOfType(type) :
			childList[i]->GetComponentsOfType(type);*/
		
		foundList.insert(foundList.end(), foundListInChild.begin(), foundListInChild.end());
	}

	return foundList;
}
#pragma endregion