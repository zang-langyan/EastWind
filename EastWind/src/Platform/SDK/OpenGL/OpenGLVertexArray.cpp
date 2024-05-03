#include "EW_pch.h"
#include "EW_Core.h"
#include "EW_Log.h"
#include "OpenGLVertexArray.h"


#include <glad/glad.h>

namespace EastWind {
  /* *****************************************************************************
   *  Utility Function
   * *****************************************************************************/
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type) {
      case ShaderDataType::Float:   return GL_FLOAT;
      case ShaderDataType::Float2:  return GL_FLOAT;
      case ShaderDataType::Float3:  return GL_FLOAT;
      case ShaderDataType::Float4:  return GL_FLOAT;
      case ShaderDataType::Int:     return GL_INT;
      case ShaderDataType::Int2:    return GL_INT;
      case ShaderDataType::Int3:    return GL_INT;
      case ShaderDataType::Int4:    return GL_INT;
      case ShaderDataType::Mat3:    return GL_FLOAT;
      case ShaderDataType::Mat4:    return GL_FLOAT;
      case ShaderDataType::Bool:    return GL_BOOL;
      case ShaderDataType::None:    return 0;
    }

    EW_ASSERT(false, "Unknown ShaderDataType!")
    return 0;
  }

  // *****************************************************************************
  OpenGLVertexArray::OpenGLVertexArray()
  {
    glGenVertexArrays(1, &m_VertexArrayId);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    glDeleteVertexArrays(1, &m_VertexArrayId);
  }


  void OpenGLVertexArray::Bind() const
  {
    glBindVertexArray(m_VertexArrayId);
  }

  void OpenGLVertexArray::Unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {
    EW_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Added Vertex Buffer has no Layout!");

    this->Bind(); // Vertex Array Bind
    vertexBuffer->Bind(); // Vertex Buffer Bind

    uint32_t idx = 0;
    const auto& layout = vertexBuffer->GetLayout();
    /*
     void glVertexAttribPointer(
       GLuint index, // layout location
       GLint size, // the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
       GLenum type, // Specifies the data type of each component in the array
       GLboolean normalized, // specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
       GLsizei stride, // Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
       const void * pointer
     ); 
     */
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    for (const auto& ele: layout){
      glEnableVertexAttribArray(idx);
      glVertexAttribPointer(
        idx,
        ele.GetComponentCount(),
        ShaderDataTypeToOpenGLBaseType(ele.Type),
        ele.Normalized ? GL_TRUE : GL_FALSE,
        layout.GetStride(),
        reinterpret_cast<const void*>(ele.Offset)
      );
      ++idx;
    }

    m_vertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    this->Bind();
    indexBuffer->Bind();

    m_indexBuffer = indexBuffer;
  }
}
