#ifndef ECS_ENTITY_MANAGER_HPP
#define ECS_ENTITY_MANAGER_HPP

#include "ecs_types.hpp"
#include "base_system.hpp"
#include "base_component.hpp"
#include "component_list.hpp"

// std
#include <queue>
#include <map>
#include <memory>

namespace ecs
{
    class EntityManager
    {
        public:
            EntityManager() : entityCount(0)
            {
                for (EntityID entity = 0u; entity < MAX_ENTITY_COUNT; entity++)
                {
                    availableEntities.push(entity);
                }
            }

            ~EntityManager()
            {

            }

            void update()
            {
                for(auto& system : registeredSystems)
                {
                    system.second->update();
                }
            }

            void render()
            {
                for(auto& system : registeredSystems)
                {
                    system.second->render();
                }
            }

            const EntityID addNewEntity()
            {
                const EntityID entityId = availableEntities.front();
                addEntitySignature(entityId);
                availableEntities.pop();
                entityCount++;
                return entityId;
            }

            void destroyEntity(const EntityID entity)
            {
                assert(entity < MAX_ENTITY_COUNT && "Entity ID out of range!");
                entitiesSignatures.erase(entity);

                for(auto& array : componentsArrays)
                {
                    array.second->erase(entity);
                }

                for(auto& system : registeredSystems)
                {
                    system.second->removeEntity(entity);
                }

                entityCount--;
                availableEntities.push(entity);
            }

            template<typename T, typename... Args>
            void addComponent(const EntityID entity, Args&&... args)
            {
                assert(entity < MAX_ENTITY_COUNT && "EntityID out of range!");
                assert(getEntitySignature(entity)->size() < MAX_COMPONENT_COUNT && "Component count limit reached!");

                T component(std::forward<Args>(args)...);
                component.entityID = entity;
                getEntitySignature(entity)->insert(ComponentType<T>());
                getComponentList<T>()->insert(component);
                updateEntityTargetSystems(entity);
            }

            template<typename T>
            void addComponent(const EntityID entity, T& component)
            {

            }

            template<typename T>
            void removeComponent(const EntityID entity)
            {
                assert(entity < MAX_ENTITY_COUNT && "EntityID out of range!");
                
                const ComponentTypeID compType = ComponentType<T>();
                getEntitySignature(entity)->erase(compType);
                getComponentList<T>()->erase(entity);
                updateEntityTargetSystems(entity);
            }

            template<typename T>
            T& getComponent(const EntityID entity)
            {
                assert(entity < MAX_ENTITY_COUNT && "EntityID out of range!");

                const ComponentTypeID compType = ComponentType<T>();
                return getComponentList<T>()->get(entity);
            }

            template<typename T>
            const bool hasComponent(const EntityID entity)
            {
                assert(entity < MAX_ENTITY_COUNT && "EntityID out of range!");

                const EntitySignature signature = entitiesSignatures.at(entity));
                const ComponentTypeID compType = ComponentType<T>();
                return signature.count(compType) > 0;
            }

            template<typename T>
            void registerSystem()
            {
                const SystemTypeID systemType = SystemType<T>();
                assert(registeredSystems.count(systemType) == 0 && "System already registered!");

                auto system = std::make_shared<T>();

                // add entity to the new system
                for(EntityID i = 0; i < entityCount; i++)
                {
                    addEntityToSystem(i, system.get());
                }

                system->start();
                registeredSystems[systemType] = std::move(system);
            }

            template<typename T>
            void unregisterSystem()
            {
                const SystemTypeID systemType = SystemType<T>();
                assert(registeredSystems.count(systemType) != 0 && "System not registered!");

                registeredSystems.erase(systemType);
            }

        private:

            template<typename T>
            void addComponentList()
            {
                const ComponentTypeID compType = ComponentType<T>();
                assert(componentsArrays.find(compType) == componentsArrays.end() && "Component list already registered!");
                componentsArrays[compType] = std::move(std::make_shared<ComponentList<T>>());
            }

            template<typename T>
            std::shared_ptr<ComponentList<T>> getComponentList()
            {
                const ComponentTypeID compType = ComponentType<T>();
                
                if(componentsArrays.count(compType) == 0) 
                    addComponentList<T>();

                return std::static_pointer_cast<ComponentList<T>>(componentsArrays.at(compType));
            }

            void addEntitySignature(const EntityID entity)
            {
                assert(entitiesSignatures.find(entity) == entitiesSignatures.end() && "Signature found!");
                entitiesSignatures[entity] = std::move(std::make_shared<EntitySignature>());
            }

            std::shared_ptr<EntitySignature> getEntitySignature(const EntityID entity)
            {
                assert(entitiesSignatures.find(entity) != entitiesSignatures.end() && "Signature not found!");
                return entitiesSignatures.at(entity);
            }

            void updateEntityTargetSystems(const EntityID entity)
            {
                for(auto& system : registeredSystems)
                {
                    addEntityToSystem(entity, system.second.get());
                }
            }

            void addEntityToSystem(const EntityID entity, BaseSystem* system)
            {
                if(belongToSystem(entity, system->signature))
                {
                    system->entities.insert(entity);
                    return;
                }

                system->entities.erase(entity);
            }

            bool belongToSystem(const EntityID entity, const EntitySignature& systemSignature)
            {
                for(const auto compType : systemSignature)
                {
                    if(getEntitySignature(entity)->count(compType) == 0)
                        return false;
                }

                return true;
            }

            EntityID entityCount;
            std::queue<EntityID> availableEntities;
            std::map<EntityID, std::shared_ptr<EntitySignature>> entitiesSignatures;
            std::map<SystemTypeID, std::unique_ptr<BaseSystem>> registeredSystems;
            std::map<ComponentTypeID, std::shared_ptr<IComponentList>> componentsArrays;
    };
}


#endif // ECS_ENTITY_MANAGER_HPP