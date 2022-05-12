#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include "entity_manager.hpp"

namespace ecs
{
    class Entity
    {
        public:
            Entity(const EntityID _id, EntityManager* _manager) : id(_id), manager(_manager) { }
            ~Entity() = default;

            const EntityID getId() const { return id; }

            template<typename T, typename... Args>
            void addComponent(Args&&... args)
            {
                manager->addComponent<T>(id, std::forward<Args>(args)...);
            }

            template<typename T>
            void addComponent(T& component)
            {
                manager->addComponent<T>(id, component);
            }

            template<typename T>
            inline T& getComponent()
            {
                return manager->getComponent<T>(id);
            }

            template<typename T>
            inline void removeComponent()
            {
                return manager->removeComponent<T>(id);
            }

            template<typename T>
            inline bool hasComponent()
            {
                return manager->hasComponent<T>(id);
            }

            void destroy()
            {
                manager->destroyEntity(id);
            }

        private:
            EntityID id;
            EntityManager* manager;
    };
}

#endif // ECS_ENTITY_HPP