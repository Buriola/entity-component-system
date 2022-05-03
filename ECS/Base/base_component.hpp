#ifndef ECS_BASE_COMPONENT_HPP

#include "ecs_types.hpp"

namespace ecs
{
    struct BaseComponent
    {
        BaseComponent() : entityId() { }
        virtual ~BaseComponent() {}

        inline const EntityID getId() const { return entityId; }

        private:
            friend class EntityManager;
            EntityID entityId;
    };
}


#define ECS_BASE_COMPONENT_HPP
#endif //ECS_BASE_COMPONENT_HPP