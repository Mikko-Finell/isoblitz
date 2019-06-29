#ifndef types_hpp
#define types_hpp

#include "stl.hpp"

namespace actions {
using Type = std::string;
const Type default_action{"idle"};
}
namespace directions {
using Type = std::string;
const Type default_direction{"down"};
}

using UID = unsigned long long;
static constexpr UID nullUID{0};

using EntityType = std::string;
#define DefaultEntityType EntityType{"DEFAULT"}

using TileIDType = std::size_t;
static constexpr TileIDType DefaultTileID{0};

#endif
