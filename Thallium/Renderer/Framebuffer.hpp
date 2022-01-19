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
        virtual void Resize(size_t width, size_t height) = 0;
        virtual void CopyInto(Ref<Framebuffer> dest)     = 0;
        virtual Ref<Texture> GetColorAttachment() const  = 0;
        virtual size_t GetWidth() const                  = 0;
        virtual size_t GetHeight() const                 = 0;
    public:
        virtual Ref<Renderer> GetRenderer() const = 0;
    };

}
