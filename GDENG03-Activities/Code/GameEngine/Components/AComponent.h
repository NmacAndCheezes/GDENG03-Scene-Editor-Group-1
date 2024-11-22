#pragma once
#include <string>
#include "EComponentTypes.h"
#include "EditorGUI/IRenderableUI.h"


class AGameObject;
class Transform;

class AComponent : public IRenderableUI
{
public:
	AComponent(std::string name, EComponentTypes type);
	virtual ~AComponent();

	virtual void Clone(AComponent* copy) = 0;
	virtual void Perform() = 0;
	virtual void RenderUI() = 0;

	void AttachOwner(AGameObject* owner);
	void DetachOwner();
	AGameObject* GetOwner();
	Transform* GetTransform();

	EComponentTypes GetType();
	std::string GetName();

	bool IsEnabled();
	virtual void SetEnabled(bool flag);
	__declspec(property(get = IsEnabled, put = SetEnabled)) bool Enabled; 
	void SetDeltaTime(float deltaTime);


protected:
	AGameObject* owner;
	Transform* transform;

	EComponentTypes type;
	std::string name;

	bool isEnabled;
	float dt;
};