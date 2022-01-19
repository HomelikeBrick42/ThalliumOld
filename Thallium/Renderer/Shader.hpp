#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"

namespace Thallium {

    class Renderer;

    class Shader: public IRef {
    protected:
        Shader() = default;
    public:
        Shader(const Shader&) = delete;
        Shader(Shader&&)      = delete;
        virtual ~Shader()     = default;
    public:
        virtual Ref<Renderer> GetRenderer() const = 0;
    };

}
