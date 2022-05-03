#ifndef ECS_COMPONENT_LIST_HPP
#define ECS_COMPONENT_LIST_HPP

#include "ecs_types.hpp"
#include <vector>

namespace ecs
{
    class IComponentList
    {
        public:
            IComponentList() = default;
            virtual ~IComponentList() = default;
            virtual void erase(const EntityID entity) = 0;
    };

    template<typename T>
    class ComponentList : public IComponentList
    {
        public:
            ComponentList() = default;
            ~ComponentList() = default;

            void insert(const T& component)
            {
                auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getId() == component.getId(); });
                if(comp != data.end())
                {
                    data.push_back(component);
                }
            }

            T& get(const EntityID entity)
            {
               auto component = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getId() == entity; });
               assert(component != data.end() && "Trying to get non-existant component!");
               return *component;
            }

            void erase(const EntityID entity) override final
            {
                auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getId() == entity; });
                if(comp != data.end())
                {
                    data.erase(comp);
                }
            }

            std::vector<T> data;
    };
}

#endif // ECS_COMPONENT_LIST_HPP