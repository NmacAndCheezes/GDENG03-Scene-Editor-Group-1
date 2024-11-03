#pragma once
#include "AMesh.h"
#include "../MathUtils.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

template <typename T>
class SphereMesh : public AMesh<T>
{
public:
	float radius = 0.5f;
	int longitudinalSize = 12;
	int latitudinalSize = 18;

	VertexBuffer<T>* CreateVertexBuffer() override
	{
		int total = (longitudinalSize - 1) * latitudinalSize + 2;
		int indexReduc = 0;
		std::vector<T> vertices(total);

		// loop to go from top to bottom
		for (int i = 0; i <= longitudinalSize; i++)
		{
			// calculate longitudinal angle, then calculate the current y pos
			float lon = MathUtils::Map((float)i, 0.0f, (float)longitudinalSize, (float)0, (float)M_PI); 
			float y = radius * cos(lon);

			// loop to create a circle along the XZ plane
			for (int j = 0; j < latitudinalSize; j++)
			{
				// if y pos is at top-most or bottom-most part of sphere, only generate one vert to prevent duplicates
				if (MathUtils::ApproximatelyEqualTo(std::fmod(lon, M_PI), 0.f) && j > 0)
				{
					indexReduc += latitudinalSize - 1;
					break;
				}

				// calculate latitudinal angle, then calculate the current x and z pos
				float lat = MathUtils::Map((float)j, 0.0f, (float)latitudinalSize, (float)0, (float)2 * M_PI); 
				float x = radius * sin(lon) * cos(lat);
				float z = radius * sin(lon) * sin(lat);

				// add new vert to list
				int index = i * latitudinalSize + j - indexReduc;
				vertices[index].pos = { x, y, z };
				vertices[index].vColor = { 1.0f, 1.0f, 1.0f };
			}
		}

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), vertices); 
	}

	IndexBuffer* CreateIndexBuffer() override
	{		
		std::vector<unsigned short> indices;

		int secondLastRow = -1;

		// generate indices for the entire sphere except the bottom bit near the south pole
		for (int i = 0; i < longitudinalSize - 1; i++) 
		{
			int bottomStart = i * latitudinalSize + 1;
			int topStart = (i - 1) * latitudinalSize + 1;

			if (i == longitudinalSize - 2) secondLastRow = bottomStart; // to be used for the next for loop at the very bottom

			for (int j = 0; j < latitudinalSize; j++)
			{
				int bottomA = bottomStart + j;
				int bottomB = (j == latitudinalSize - 1) ? bottomStart : bottomStart + j + 1;

				if (i == 0)
				{
					int mid = 0;

					// insert triangle from the north pole
					indices.push_back(mid); 
					indices.push_back(bottomB); 
					indices.push_back(bottomA);
				}
				else
				{
					int topA = topStart + j; 
					int topB = (j == latitudinalSize - 1) ? topStart : topStart + j + 1;
					
					// insert top-left triangle A
					indices.push_back(topA); 
					indices.push_back(topB); 
					indices.push_back(bottomA);
					// insert bottom-right triangle B
					indices.push_back(topB); 
					indices.push_back(bottomB); 
					indices.push_back(bottomA);
				}	
			}
		}

		// connect the verts to the south pole
		for (int i = 0; i < latitudinalSize; i++)
		{
			int mid = secondLastRow + latitudinalSize;
			int topA = secondLastRow + i; 
			int topB = (i == latitudinalSize - 1) ? secondLastRow : secondLastRow + i + 1;  

			indices.push_back(mid); 
			indices.push_back(topA); 
			indices.push_back(topB); 
		}

		return new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	}
};