#include "Thallium/Scene/Scene.hpp"
#include "Thallium/Scene/Entity.hpp"

#include <iostream>
#include <format>
#include <cassert>

namespace Thallium {

    Scene::Scene() {}

    Scene::~Scene() {
        for (auto& [component_id, entities] : Components) {
            if (RemoveComponentCallbacks.contains(component_id) && RemoveComponentCallbacks[component_id]) {
                for (auto& [entity, component] : entities) {
                    RemoveComponentCallbacks[component_id](*this, entity);
                }
            }
        }
    }

    Ref<Scene> Scene::Create() {
        return new Scene();
    }

    EntityID Scene::CreateEntity() {
        EntityID entity = ++CurrentEntityID;
        Entities.insert({ entity, {} });
        return entity;
    }

    void Scene::DestroyEntity(EntityID entity) {
        assert(EntityExists(entity));
        for (auto& component : Entities.at(entity)) {
            if (RemoveComponentCallbacks.contains(component) && RemoveComponentCallbacks[component]) {
                RemoveComponentCallbacks[component](*this, entity);
            }
            Components.at(component).erase(Components.at(component).find(entity));
        }
        Entities.erase(Entities.find(entity));
    }

    bool Scene::EntityExists(EntityID entity) const {
        return Entities.contains(entity);
    }

    std::any& Scene::AddComponent(EntityID entity, std::any&& value) {
        assert(EntityExists(entity));
        assert(!HasComponent(entity, value.type()));
        if (!Components.contains(value.type())) {
            Components.insert({ value.type(), {} });
        }
        Components.at(value.type()).emplace(entity, std::move(value));
        Entities.at(entity).insert(value.type());
        if (AddComponentCallbacks.contains(value.type()) && AddComponentCallbacks[value.type()]) {
            AddComponentCallbacks[value.type()](*this, entity);
        }
        return Components.at(value.type()).at(entity);
    }

    void Scene::RemoveComponent(EntityID entity, std::type_index type) {
        assert(EntityExists(entity));
        assert(HasComponent(entity, type));
        if (RemoveComponentCallbacks.contains(type) && RemoveComponentCallbacks[type]) {
            RemoveComponentCallbacks[type](*this, entity);
        }
        Entities.at(entity).erase(Entities.at(entity).find(type));
        Components.at(type).erase(Components.at(type).find(entity));
    }

    bool Scene::HasComponent(EntityID entity, std::type_index type) const {
        assert(EntityExists(entity));
        return Entities.at(entity).contains(type);
    }

    std::any& Scene::GetComponent(EntityID entity, std::type_index type) {
        assert(EntityExists(entity));
        assert(HasComponent(entity, type));
        return Components.at(type).at(entity);
    }

}
