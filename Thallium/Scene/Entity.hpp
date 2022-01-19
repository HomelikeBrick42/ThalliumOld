#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Scene/Scene.hpp"

#include <utility>
#include <typeinfo>

namespace Thallium {

    class Entity {
    public:
        Entity() : ID(UINT64_MAX), Scene(nullptr) {}
        Entity(Ref<Scene> scene) : ID(UINT64_MAX), Scene(std::move(scene)) {
            ID = Scene->CreateEntity();
        }
        Entity(EntityID id, Ref<Scene> scene) : ID(id), Scene(std::move(scene)) {}
    public:
        void Destroy() {
            Scene->DestroyEntity(ID);
        }

        template<typename T>
        T& AddComponent(T&& value = T()) {
            return *std::any_cast<T>(&Scene->AddComponent(ID, std::make_any<T>(std::move(value))));
        }

        template<typename T>
        void RemoveComponent() {
            Scene->RemoveComponent(ID, typeid(T));
        }

        template<typename T>
        bool HasComponent() const {
            return Scene->HasComponent(ID, typeid(T));
        }

        template<typename T>
        T& GetComponent() {
            return *std::any_cast<T>(&Scene->GetComponent(ID, typeid(T)));
        }

        explicit operator bool() const {
            return Scene && Scene->EntityExists(ID);
        }

        bool operator==(const Entity& other) const {
            return this->ID == other.ID && this->Scene == other.Scene;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }
    private:
        EntityID ID;
        Ref<Scene> Scene;
    };

}
