#pragma once

#include "Base.hpp"
#include "Ref.hpp"

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
        virtual Ref<Renderer> GetRenderer() = 0;
    };

}
