#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"

#include <span>

namespace Thallium {

    class Renderer;

    class Texture: public IRef {
    protected:
        Texture() = default;
    public:
        Texture(const Texture&) = delete;
        Texture(Texture&&)      = delete;
        virtual ~Texture()      = default;
    public:
        virtual void SetPixels(const glm::u8vec4* pixels, size_t width, size_t height) = 0;
        virtual void SetPixels(const glm::vec4* pixels, size_t width, size_t height)   = 0;
        virtual void AllocatePixels(size_t width, size_t height)                       = 0;
        virtual void GetPixels(glm::u8vec4* outPixels)                                 = 0;
        virtual void GetPixels(glm::vec4* outPixels)                                   = 0;
        virtual size_t GetWidth() const                                                = 0;
        virtual size_t GetHeight() const                                               = 0;
    public:
        virtual Ref<Renderer> GetRenderer() const = 0;
    };

}
