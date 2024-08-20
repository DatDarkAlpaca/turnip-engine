#pragma once
#include "CommonGL.hpp"
#include "Graphics/Framebuffer.hpp"

namespace tur::gl
{
    inline constexpr gl_handle GetFramebufferAttachmentType(AttachmentType type)
    {
        switch (type) 
        {
            case AttachmentType::COLOR:
                return GL_COLOR_ATTACHMENT0;

            case AttachmentType::DEPTH:
                return GL_DEPTH_ATTACHMENT;

            case AttachmentType::STENCIL:
                return GL_STENCIL_ATTACHMENT;

            case AttachmentType::DEPTH_STENCIL:
                return GL_DEPTH_STENCIL_ATTACHMENT;

            default:
                TUR_LOG_CRITICAL("OpenGL does not support the following attachment type: {}", (uint32_t)type);
        }

        return invalid_handle;
    }
}