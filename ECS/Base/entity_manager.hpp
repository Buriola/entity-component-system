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

            const EntityID addNewEntity()
            {
                const EntityID entityId = availableEntities.front();
                availableEntities.pop();
                entityCount++;
                return entityId;
            }

            void destroyEntity(const EntityID entity)
            {

            }

        private:
            EntityID entityCount;
            std::queue<EntityID> availableEntities;
            std::map<EntityID, EntitySignature> entitiesSignatures;
            std::map<SystemTypeID, std::unique_ptr<BaseSystem>> registeredSystems;
            std::map<ComponentTypeID, std::shared_ptr<IComponentList>> componentsArrays;
    };
}


#endif // ECS_ENTITY_MANAGER_HPP