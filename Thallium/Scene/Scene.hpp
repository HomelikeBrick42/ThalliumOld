#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <any>
#include <set>

namespace Thallium {

    using EntityID = uint64_t;

    class Scene final: public IRef {
        friend class Entity;
    private:
        Scene();
    public:
        ~Scene();
        Scene(const Scene&) = delete;
        Scene(Scene&&)      = delete;
    public:
        static Ref<Scene> Create();
    public:
        template<typename T>
        void SetAddComponentCallback(std::function<void(Scene&, EntityID, T&)> callback) {
            AddComponentCallbacks[typeid(T)] = [=](Scene& scene, EntityID id) {
                callback(scene, id, *any_cast<T>(&GetComponent(id, typeid(T))));
            };
        }

        template<typename T>
        void SetRemoveComponentCallback(std::function<void(Scene&, EntityID, T&)> callback) {
            RemoveComponentCallbacks[typeid(T)] = [=](Scene& scene, EntityID id) {
                callback(scene, id, *any_cast<T>(&GetComponent(id, typeid(T))));
            };
        }

        template<typename T>
        void IterateComponent(std::function<void(EntityID, T&)> func) {
            if (Components.contains(typeid(T))) {
                for (auto& [entityID, component] : Components.at(typeid(T))) {
                    func(entityID, *any_cast<float>(&component));
                }
            }
        }

        template<typename... Types>
        void IterateComponents(std::function<void(EntityID, Types&...)> func) {
            IterateComponentsImpl<Types...>({}, func);
        }
    private:
        template<typename ComponentType, typename SecondComponentType, typename... ComponentTypes, typename... Types>
        void IterateComponentsImpl(EntityID entityFromParentCall, std::function<void(EntityID, Types&...)>& func) {
            for (auto& [entity, component] : Components.at(typeid(ComponentType))) {
                if (!EntityExists(entityFromParentCall) || entity == entityFromParentCall) {
                    IterateComponentsImpl<SecondComponentType, ComponentTypes...>(entity, func);
                }
            }
        }

        template<typename ComponentType, typename... Types>
        void IterateComponentsImpl(EntityID entityFromParentCall, std::function<void(EntityID, Types&...)>& func) {
            for (auto& [entity, component] : Components.at(typeid(ComponentType))) {
                if (!EntityExists(entityFromParentCall) || entity == entityFromParentCall) {
                    func(entity, *std::any_cast<Types>(&Components.at(typeid(Types)).at(entity))...);
                }
            }
        }
    private:
        EntityID CreateEntity();
        void DestroyEntity(EntityID entity);
        bool EntityExists(EntityID entity) const;
        std::any& AddComponent(EntityID entity, std::any&& value);
        void RemoveComponent(EntityID entity, std::type_index type);
        bool HasComponent(EntityID entity, std::type_index type) const;
        std::any& GetComponent(EntityID entity, std::type_index type);
    private:
        EntityID CurrentEntityID = 0;
        std::unordered_map<EntityID, std::set<std::type_index>> Entities;
        std::unordered_map<std::type_index, std::unordered_map<EntityID, std::any>> Components;
        std::unordered_map<std::type_index, std::function<void(Scene&, EntityID)>> AddComponentCallbacks;
        std::unordered_map<std::type_index, std::function<void(Scene&, EntityID)>> RemoveComponentCallbacks;
    };

}
