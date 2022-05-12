#ifndef ECS_BASE_SYSTEM_HPP
#define ECS_BASE_SYSTEM_HPP

#include "ecs_types.hpp"

namespace ecs
{
    class BaseSystem
    {
        public:
            BaseSystem() = default;
            virtual ~BaseSystem() = default;

            void removeEntity(const EntityID entity) { entities.erase(entity); }
            void addEntity(const EntityID entity) { entities.insert(entity); }

            const EntitySignature getSignature() const { return signature; }

            template<typename T>
            void addComponentSignature()
            {
                signature.insert(ComponentType<T>());
            }

            virtual void start() { }
            virtual void update() { }
            virtual void render() { }
            virtual void destroy() { }

        protected:
            friend class EntityManager;
            EntitySignature signature;
            std::set<EntityID> entities;
    };
}

#endif // ECS_BASE_SYSTEM_HPP