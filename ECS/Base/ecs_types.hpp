#ifndef ECS_TYPES_HPP
#define ECS_TYPES_HPP

#include <set>
#include <type_traits>
#include <algorithm>
#include <cassert>

namespace ecs
{
    struct BaseSystem;
    struct BaseComponent;

    // Constants
    constexpr size_t MAX_ENTITY_COUNT = 5000;
    constexpr size_t MAX_COMPONENT_COUNT = 32;

    // Custom Aliases
    using EntityID = unsigned int;
    using SystemTypeID = unsigned int;
    using ComponentTypeID = unsigned int;
    using EntitySignature = std::set<ComponentTypeID>;

    inline static const ComponentTypeID getRuntimeComponentTypeId()
    {
        static ComponentTypeID typeId = 0u;
        return typeId++;
    }

    inline static const SystemTypeID getRuntimeSystemTypeId()
    {
        static SystemTypeID typeId = 0u;
        return typeId++;
    }

    template<typename T>
    inline static const ComponentTypeID ComponentType() noexcept 
    {
        static_assert((std::is_base_of<BaseComponent, T>::value && !std::is_same<BaseComponent, T>::value), "INVALID TEMPLATE TYPE!");
        static const ComponentTypeID typeId = getRuntimeComponentTypeId();
        return typeId;
    }

    template<typename T>
    inline static const SystemTypeID SystemType() noexcept
    {
        static_assert((std::is_base_of<BaseSystem, T>::value && !std::is_same<BaseSystem, T>::value), "INVALID TEMPLATE TYPE!");
        static const SystemTypeID typeId = getRuntimeSystemTypeId();
        return typeId;
    }
}

#endif // ECS_TYPES_HPP