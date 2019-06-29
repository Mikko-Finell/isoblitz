#ifndef minimap_hpp
#define minimap_hpp

#include "common/engine.hpp"
#include "common/coordinate.hpp"
#include <map>
#include <vector>

class Minimap : public EntitySystem {
    Sprite worldsprite;
    std::map<Entity *, Sprite> entity_sprites;
    std::vector<Entity *> needs_update;

    void update(float dt) override;
    void add_entity(Entity & entity) override;
    void remove_entity(Entity & entity) override;

public:
    Minimap(Engine & engine);
};

#endif
