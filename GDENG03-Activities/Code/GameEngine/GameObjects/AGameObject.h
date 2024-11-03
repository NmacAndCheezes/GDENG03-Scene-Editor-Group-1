#pragma once
#include <string>
#include <vector>
#include "../Components/AComponent.h"
#include "../Components/Transform.h"


class AGameObject
{
public:
	// Constructor-Destructor
	AGameObject(std::string name);
	~AGameObject();

	// Game-related methods
	bool IsInitialized();
	virtual void Initialize() = 0;
	//virtual void ProcessInputs(WPARAM wParam, LPARAM lParam);
	virtual void Update(float dt);
	virtual void Draw(); 

	unsigned int GetInstanceID();
	std::string GetName();
	void SetName(std::string newName);
	__declspec(property(get = GetName, put = SetName)) std::string Name;
	bool IsEnabled();
	void SetEnabled(bool flag);
	__declspec(property(get = IsEnabled, put = SetEnabled)) bool Enabled;

	// Inheritance-related methods
	void AttachChild(AGameObject* child);
	void DetachChild(AGameObject* child);
	int GetLevel();
	AGameObject* GetParent();
	std::vector<AGameObject*> GetChildList();

	// Component-related methods
	Transform* GetTransform();
	void AttachComponent(AComponent* component);
	void DetachComponent(AComponent* component);
	AComponent* FindComponentByName(std::string name);
	AComponent* FindComponentByType(EComponentTypes type, std::string name);
	std::vector<AComponent*> GetComponentsOfType(EComponentTypes type);
	std::vector<AComponent*> GetComponentsInChildrenOfType(EComponentTypes type);

private:
	void SetParent(AGameObject* parent);


protected:
	unsigned int instanceID;

	std::string name;
	bool enabled;
	bool isInitialized;
	Transform* transform;

	int level;
	AGameObject* parent;
	std::vector<AGameObject*> childList;
	std::vector<AComponent*> componentList; 

private:
	static int currentID;

private:
	friend class InspectorUI;
};

