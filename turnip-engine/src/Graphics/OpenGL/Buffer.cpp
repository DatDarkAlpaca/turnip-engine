#include "pch.h"
#include "Buffer.h"

namespace tur
{
    Buffer::Buffer(U32 target)
        : m_Target(target)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(m_Target, m_ID);
    }

    Buffer::~Buffer()
    {
        if (m_ID == TUR_GL_INVALID)
            return;

        Destroy();
    }

    void Buffer::Initialize(size_t sizeInBytes, const void* data, GLenum usage)
    {
        if (m_ID == TUR_GL_INVALID)
            TUR_CORE_WARN("The buffer in use has an invalid ID.");

        glBufferData(m_Target, sizeInBytes, data, usage);
    }

    void Buffer::Destroy()
    {
        TUR_ASSERT(m_ID != TUR_GL_INVALID, "Attempted to destroy a non-initialized buffer.");

        glDeleteBuffers(1, &m_ID);
        m_ID = TUR_GL_INVALID;
    }

    // Todo: test.
    void Buffer::SetData(U64 offset, U32 sizeInBytes, const void* data)
    {
        glBufferSubData(m_Target, offset, sizeInBytes, data);
    }
}