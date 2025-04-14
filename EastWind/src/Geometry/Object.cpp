#include "EW_pch.h"
#include "Object.h"

#include "EW_Log.h"

namespace EastWind {

  Plane::Plane()
  {
    m_BufferState = EastWind::BufferState::Create();
    float vertices[4*6] = {
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // left bottom
				 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // right bottom 
				 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // right top
				-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // left top
    };

    uint32_t indices[6] = {
      0, 1, 2,
      2, 3, 0
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
 
  /*
   * Sphere parametric representation 
   * (x, y ,z)
   * x = (r · cos(phi)) · cos(theta)
   * y = (r · cos(phi)) · sin(theta)
   * z = r · sin(phi)
   *
   * phi: angle to the xy plane
   * theta: angle to the x axis
   *
   * phi = PI/2 - i * stackStep
   * theta = j * sectorStep
   */
  Sphere::Sphere(float radius, int sectors, int stacks)
  {
    float sectorStep = 2*PI/sectors;
    float stackStep = PI/stacks;

    float x, y, z; // vertex
    float nx, ny, nz; // normal 
    float s, t; // texture coordinates

    std::vector<float> vertices((stacks+1)*(sectors+1)*8);
    for (int i = 0; i <= stacks; ++i){
      float phi = PI/2 - i*stackStep; // PI/2 ~ -PI/2
      float xy = radius * cosf(phi);
      z = radius * sinf(phi);
      for (int j = 0; j <= sectors; ++j){
        float theta = j * sectorStep;
        x = xy * cosf(theta);
        y = xy * sinf(theta);

        nx = x/radius;
        ny = y/radius;
        nz = z/radius;

        s = (float)j / sectors;
        t = (float)i / stacks;

        vertices[(j+i*(sectors+1))*8    ] = x;
        vertices[(j+i*(sectors+1))*8 + 1] = y;
        vertices[(j+i*(sectors+1))*8 + 2] = z;
        vertices[(j+i*(sectors+1))*8 + 3] = nx;
        vertices[(j+i*(sectors+1))*8 + 4] = ny;
        vertices[(j+i*(sectors+1))*8 + 5] = nz;
        vertices[(j+i*(sectors+1))*8 + 6] = 1-s;
        vertices[(j+i*(sectors+1))*8 + 7] = 1-t;
      }
    }

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    uint32_t k1, k2;
    std::vector<uint32_t> indices;
    for (int i = 0; i < stacks; ++i){
      k1 = (uint32_t)i * (sectors + 1); // begining of current stack
      k2 = k1 + sectors + 1; // begining of the next stack
      for (int j = 0; j < sectors; ++j, ++k1, ++k2){
        if (i != 0){
          indices.push_back(k1);
          indices.push_back(k2);
          indices.push_back(k1+1);
        }
        if (i != stacks-1){
          indices.push_back(k1+1);
          indices.push_back(k2);
          indices.push_back(k2+1);
        }
      }
    }

    m_BufferState = EastWind::BufferState::Create();
    // Vertex Buffer
    Ref<VertexBuffer> vertexBuffer;
    BufferLayout layout = {
      { EastWind::ShaderDataType::Float3, "aPos"    },
      { EastWind::ShaderDataType::Float3, "aNormal" },
      { EastWind::ShaderDataType::Float2, "aTexCoord" },
    };
    
    vertexBuffer.reset(VertexBuffer::Create(vertices.data(), sizeof(vertices.data())));
    vertexBuffer->SetLayout(layout);
    m_BufferState->AddVertexBuffer(vertexBuffer);

    // Index Buffer
    Ref<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices.data(), indices.size()));
    m_BufferState->SetIndexBuffer(indexBuffer);

    // m_ActiveShader = "BasicTextureShader";
  }
}
