#include "EW_pch.h"
#include "Object.h"

#include "EW_Log.h"

namespace EastWind {


  Cube::Cube()
  {
    m_BufferState = EastWind::BufferState::Create();
    float vertices[144] = {
				-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // index 0
				-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

				 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, // index 4
				 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, // index 8
				 1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f,
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f,

				 1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f, // index 12
				 1.0f,  1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f,

				-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, // index 16
				-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,

				 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // index 20
				-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    uint32_t indices[36] = {
			0, 1, 2, 
			2, 3, 0, 
			4, 5, 6, 
			6, 7, 4, 
			8, 9, 10, 
			10, 11, 8, 
			12, 13, 14, 
			14, 15, 12, 
			16, 17, 18, 
			18, 19, 16, 
			20, 21, 22, 
			22, 23, 20
		};
    // Vertex Buffer
    Ref<VertexBuffer> vertexBuffer;
    BufferLayout layout = {
      { EastWind::ShaderDataType::Float3, "aPos"    },
      { EastWind::ShaderDataType::Float3, "aNormal" },
    };
    
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    vertexBuffer->SetLayout(layout);
    m_BufferState->AddVertexBuffer(vertexBuffer);

    // Index Buffer
    Ref<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
    m_BufferState->SetIndexBuffer(indexBuffer);
  }
 
}
