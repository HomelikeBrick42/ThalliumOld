#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"
#include "Thallium/Renderer/Texture.hpp"

#include <span>

namespace Thallium {

    class Renderer;

    class Framebuffer: public IRef {
    protected:
        Framebuffer() = default;
    public:
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&&)      = delete;
        virtual ~Framebuffer()          = default;
    public:
        virtual Ref<Texture> GetColorAttachment() = 0;
    public:
        virtual Ref<Renderer> GetRenderer() = 0;
    };

}
