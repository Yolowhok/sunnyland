#pragma once

#include "ecs/ComponentStore.hpp"
#include "ecs/Entity.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

class World {
public:
    Entity createEntity();
    void destroyEntity(Entity entity);
    bool alive(Entity entity) const;

    template <typename T>
    ComponentStore<T>& components() {
        const std::type_index key(typeid(T));
        auto it = stores_.find(key);
        if (it == stores_.end()) {
            auto store = std::make_unique<StoreHolder<T>>();
            ComponentStore<T>* raw = &store->store;
            stores_.emplace(key, std::move(store));
            return *raw;
        }
        return static_cast<StoreHolder<T>*>(it->second.get())->store;
    }

    template <typename T>
    const ComponentStore<T>& components() const {
        const std::type_index key(typeid(T));
        auto it = stores_.find(key);
        if (it == stores_.end()) {
            static ComponentStore<T> empty;
            return empty;
        }
        return static_cast<const StoreHolder<T>*>(it->second.get())->store;
    }

    template <typename T, typename... Args>
    T& add(Entity entity, Args&&... args) {
        return components<T>().emplace(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    bool has(Entity entity) const {
        return components<T>().has(entity);
    }

    template <typename T>
    T& get(Entity entity) {
        return components<T>().get(entity);
    }

    template <typename T>
    T* tryGet(Entity entity) {
        return components<T>().tryGet(entity);
    }

private:
    struct IStoreHolder {
        virtual ~IStoreHolder() = default;
        virtual void onEntityDestroyed(Entity entity) = 0;
    };

    template <typename T>
    struct StoreHolder : IStoreHolder {
        ComponentStore<T> store;
        void onEntityDestroyed(Entity entity) override { store.onEntityDestroyed(entity); }
    };

    Entity nextEntityId_ = 1;
    std::unordered_map<Entity, bool> alive_;
    std::unordered_map<std::type_index, std::unique_ptr<IStoreHolder>> stores_;
};

} // namespace ecs
