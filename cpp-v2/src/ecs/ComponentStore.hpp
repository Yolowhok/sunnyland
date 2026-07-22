#pragma once

#include "ecs/Entity.hpp"

#include <unordered_map>
#include <utility>
#include <vector>

namespace ecs {

template <typename T>
class ComponentStore {
public:
    bool has(Entity entity) const { return data_.find(entity) != data_.end(); }

    T& get(Entity entity) { return data_.at(entity); }
    const T& get(Entity entity) const { return data_.at(entity); }

    T* tryGet(Entity entity) {
        auto it = data_.find(entity);
        return it == data_.end() ? nullptr : &it->second;
    }

    const T* tryGet(Entity entity) const {
        auto it = data_.find(entity);
        return it == data_.end() ? nullptr : &it->second;
    }

    template <typename... Args>
    T& emplace(Entity entity, Args&&... args) {
        auto [it, inserted] = data_.emplace(entity, T{std::forward<Args>(args)...});
        (void)inserted;
        return it->second;
    }

    void remove(Entity entity) { data_.erase(entity); }

    void onEntityDestroyed(Entity entity) { remove(entity); }

    template <typename Fn>
    void forEach(Fn&& fn) {
        for (auto& [entity, component] : data_) {
            fn(entity, component);
        }
    }

    template <typename Fn>
    void forEach(Fn&& fn) const {
        for (const auto& [entity, component] : data_) {
            fn(entity, component);
        }
    }

    std::size_t size() const { return data_.size(); }

private:
    std::unordered_map<Entity, T> data_;
};

} // namespace ecs
