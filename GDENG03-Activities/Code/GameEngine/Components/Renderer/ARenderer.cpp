#include "ARenderer.h"
#include "../Transform.h"
#include "GameEngine/GameObjects/AGameObject.h"
#include "GameEngine/Graphics/Materials/UnlitColorMaterial.h"


ARenderer::ARenderer(std::string compoonentName) : AComponent(compoonentName, EComponentTypes::Renderer)
{
	topologyType = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; 
	indexBuffer = nullptr; 
	material = new UnlitColorMaterial(Vector3( 
		MathUtils::RandFloatWithRange(), 
		MathUtils::RandFloatWithRange(), 
		MathUtils::RandFloatWithRange() 
	));
}

ARenderer::ARenderer(std::string compoonentName, AMaterial* material) : AComponent(compoonentName, EComponentTypes::Renderer)
{
	topologyType = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	indexBuffer = nullptr;
	this->material = material;
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
	topologyType = copyRenderer->topologyType;
	vertexBuffer = copyRenderer->vertexBuffer;
	indexBuffer = copyRenderer->indexBuffer;
	//tMatrixBuffer = copyRenderer->tMatrixBuffer;
}

bool ARenderer::Release()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}

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
	if (!vertexBuffer || !indexBuffer || !tMatrixBuffer || !material) return; 

	material->Render(); 
	vertexBuffer->BindToPipeline(); 
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
	return material->GetShaderType();
}

void ARenderer::SetMaterial(AMaterial* newMaterial)
{
	if (material) delete material;

	material = newMaterial;
}

void ARenderer::InitRenderer()
{
	tMatrixBuffer = new VertexConstantBuffer<TMatrix>(GraphicsEngine::GetInstance(), 0u);
	tMatrixBuffer->Init();
}