#pragma once
#include <vector>
#include <string>
#include "../AComponent.h"
#include "../../../DirectXClasses/Buffers/IndexBuffer.h"
#include "../../../DirectXClasses/Buffers/ConstantBuffer.cpp"
#include "../../../DirectXClasses/Buffers/BufferDataTypes.h"


class ARenderer : public AComponent
{
public:
	ARenderer(std::string compoonentName, LPCWSTR shaderType);
	~ARenderer();

	virtual void Clone(AComponent* copy) override;
	virtual bool Release();
	void Perform() override;
	LPCWSTR GetShaderType();

protected:
	void InitRenderer();


protected:
	LPCWSTR shaderType;
	D3D11_PRIMITIVE_TOPOLOGY topologyType;

	std::vector<AD3D11Object*> buffersList;
	IndexBuffer* indexBuffer;
	VertexConstantBuffer<TMatrix>* tMatrixBuffer;
};