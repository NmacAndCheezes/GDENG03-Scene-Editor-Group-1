#pragma once
#include <vector>
#include <string>
#include "../AComponent.h"
#include "GameEngine/Graphics/Materials/AMaterial.h"
#include "DirectXClasses/Buffers/IndexBuffer.h"
#include "DirectXClasses/Buffers/ConstantBuffer.cpp"
#include "DirectXClasses/Buffers/BufferDataTypes.h"


class ARenderer : public AComponent
{
public:
	ARenderer(std::string compoonentName);
	ARenderer(std::string compoonentName, AMaterial* material);
	~ARenderer();

	virtual void Clone(AComponent* copy) override;
	virtual bool Release();
	virtual void RenderUI() = 0;
	void Perform() override;

	LPCWSTR GetShaderType();
	AMaterial* GetMaterial();
	void SetMaterial(AMaterial* newMaterial);

protected:
	void InitRenderer();


protected:
	AMaterial* material;

	D3D11_PRIMITIVE_TOPOLOGY topologyType;
	AD3D11Object* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexConstantBuffer<TMatrix>* tMatrixBuffer;
};