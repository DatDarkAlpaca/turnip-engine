#pragma once
#include "CommonGraphics.hpp"
#include "Texture.hpp"

namespace tur
{
	using framebuffer_handle = handle_type;

    struct FramebufferAttachmentDescriptor 
    {
        uint32_t width;
        uint32_t height;
        AttachmentType type;
        TextureFormat textureFormat;
        bool useRenderbuffer;

        WrapMode wrapS = WrapMode::REPEAT;
        WrapMode wrapT = WrapMode::REPEAT;
        FilterMode minFilter = FilterMode::NEAREST;
        FilterMode magFilter = FilterMode::NEAREST;
    };

    struct FramebufferDescriptor 
    {
        std::vector<FramebufferAttachmentDescriptor> attachments;
        uint32_t width;
        uint32_t height;
    };
}