#include "ecs/World.hpp"

namespace ecs {

Entity World::createEntity() {
    const Entity id = nextEntityId_++;
    alive_[id] = true;
    return id;
}

void World::destroyEntity(Entity entity) {
    if (!alive(entity)) {
        return;
    }
    for (auto& [type, store] : stores_) {
        (void)type;
        store->onEntityDestroyed(entity);
    }
    alive_.erase(entity);
}

bool World::alive(Entity entity) const {
    auto it = alive_.find(entity);
    return it != alive_.end() && it->second;
}

} // namespace ecs
