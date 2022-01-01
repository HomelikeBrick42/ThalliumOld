#pragma once

#include "Base.hpp"
#include "Ref.hpp"

#include <span>

class Renderer;

class IndexBuffer: public IRef {
protected:
    IndexBuffer() = default;
public:
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&)      = delete;
    virtual ~IndexBuffer()          = default;
public:
    virtual void SetIndices(const std::span<uint32_t>& indices) = 0;
    virtual size_t GetIndexCount()                              = 0;
public:
    virtual Ref<Renderer> GetRenderer() = 0;
};
