#include "Thallium/Scene/Scene.hpp"
#include "Thallium/Scene/Entity.hpp"

#include <iostream>
#include <format>
#include <cassert>

namespace Thallium {

    Scene::Scene() {}

    Scene::~Scene() {}

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
        return Components.at(value.type()).at(entity);
    }

    void Scene::RemoveComponent(EntityID entity, std::type_index type) {
        assert(EntityExists(entity));
        assert(HasComponent(entity, type));
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
