#include "ARenderer.h"
#include "../Transform.h"
#include "../../GameObjects/AGameObject.h"


ARenderer::ARenderer(std::string compoonentName, LPCWSTR shaderType) : AComponent(compoonentName, EComponentTypes::Renderer), shaderType(shaderType)
{
	topologyType = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; 
	indexBuffer = nullptr; 
}

ARenderer::~ARenderer()
{

}

void ARenderer::Clone(AComponent* copy)
{
	ARenderer* copyRenderer = (ARenderer*)copy;
	if (copyRenderer == nullptr) return;

	// consider creating copy for each buffer instead taking the ref from another renderer
	Release();
	InitRenderer();
	shaderType = copyRenderer->shaderType;
	topologyType = copyRenderer->topologyType;
	buffersList = copyRenderer->buffersList;
	indexBuffer = copyRenderer->indexBuffer;
	//tMatrixBuffer = copyRenderer->tMatrixBuffer;
}

bool ARenderer::Release()
{
	for (int i = (int)buffersList.size() - 1; i >= 0; i--)
	{
		buffersList[i]->Release();
	}
	buffersList.clear();
	buffersList.shrink_to_fit();

	if (indexBuffer)
	{
		indexBuffer->Release();
	}

	if (tMatrixBuffer)
	{
		tMatrixBuffer->Release();
	}

	return true;
}

void ARenderer::Perform()
{
	if (buffersList.size() == 0) return;

	// Bind every buffer stored in this renderer
	for (auto& buffer : buffersList)
	{
		buffer->BindToPipeline();
	}

	indexBuffer->BindToPipeline();

	Transform* transform = owner->GetTransform();
	tMatrixBuffer->SetConstants(transform->GetTransformationMatrix());
	tMatrixBuffer->BindToPipeline();

	// Set the topology type, then draw to the GPU
	GraphicsEngine::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(topologyType);
	GraphicsEngine::GetInstance()->GetDeviceContext()->DrawIndexed(indexBuffer->GetIndexCount(), 0u, 0u);
}

LPCWSTR ARenderer::GetShaderType()
{
	return shaderType;
}

void ARenderer::InitRenderer()
{
	tMatrixBuffer = new VertexConstantBuffer<TMatrix>(GraphicsEngine::GetInstance(), 0u);
	tMatrixBuffer->Init();
}