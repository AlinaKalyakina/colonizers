#include "gamefield.h"

Player::Player(const std::string &x, int num) : id(num), name(x) {}

Hex::Hex(coord_t pos, Resource t) : pos(pos), type(t) {}

Object::Object(int player, ObjectType t) : owner(player), type(t) {}


template <int N>
coord_t neighbour(const coord_t& x) noexcept{
    coord_t shifts[6] = {coord_t(1, 0), coord_t(0, 1), coord_t(-1, 1),
                        coord_t(-1, 0), coord_t(0, -1), coord_t(1, -1)};
    return coord_t(x.first + shifts[N].first, x.second +shifts[N].second);
}

bool belong_field(const road_pos &road) noexcept{
    auto x = road.first.first;
    auto y = road.first.second;
    switch (road.second) {
    case RoadCoord::DOWN:
        if (x >= -2 && x <= 1 &&
                y >= (- 1 * (x >= 0) - (2 + x) * (x < 0)) &&
                y <= ((2 - x) * (x >= 0) + 2 * (x < 0))) {
            return true;
        }
        break;
    case RoadCoord::RIGHT:
        if (x >= -2 && x <= 1 &&
                y >= (- 2 * (x >= 0) - (2 + x) * (x < 0)) &&
                y <= ((1 - x) * (x >= 0) + 2 * (x < 0))) {
            return true;
        }
        break;
    case RoadCoord::UP:
       if (x >= -2 && x <= 2 &&
               y >= (- 2 * (x >= 0) - (2 + x) * (x < 0)) &&
               y <= ((1 - x) * (x >= 0) + 2 * (x < 0))) {
               return true;
        }
    }
    return false;
}

bool belong_field(const cross_pos &cross) noexcept{
    auto neigh = neighbour_roads(cross);
    return !neigh.empty();
}

std::set<cross_pos> neighbour_crosses(const coord_t & hex) noexcept{
    std::set<cross_pos> result;
    cross_pos neigh[6];
    neigh[0].first = neigh[1].first = hex;
    neigh[2].first = neighbour<1>(hex);
    neigh[3].first = neighbour<2>(hex);
    neigh[0].second = neigh[2].second =
            neigh[3].second = CrossCoord::BOTTOM;
    neigh[4].first = neighbour<4>(hex);
    neigh[5].first = neighbour<5>(hex);
    neigh[1].second = neigh[4].second =
            neigh[5].second = CrossCoord::TOP;
    for (int i = 0; i < 6; i++) {
        if (belong_field(neigh[i])) {
            result.insert(neigh[i]);
        }
    }
    return result;
}

std::set<cross_pos> neighbour_crosses(const road_pos &road) noexcept{
    cross_pos pos1, pos2;
    if (road.second == RoadCoord::UP ||  road.second ==RoadCoord::RIGHT) {
        pos1 = cross_pos(neighbour<1>(road.first), CrossCoord::BOTTOM);
    } else {
        pos2 = cross_pos(neighbour<5>(road.first), CrossCoord::TOP);
    }
    std::set<cross_pos> res;
    if (road.second == RoadCoord::UP) {
        pos2 = cross_pos(road.first, CrossCoord::TOP);
    }
    if (road.second == RoadCoord::DOWN) {
        pos1 = cross_pos(road.first, CrossCoord::BOTTOM);
    }
    if (road.second == RoadCoord::RIGHT) {
        pos2 = cross_pos(neighbour<5>(road.first), CrossCoord::TOP);
    }
    if (belong_field(pos1)) {
        res.insert(pos1);
    }
    if (belong_field(pos2)) {
        res.insert(pos2);
    }
    return res;
}

std::set<cross_pos> neighbour_crosses(const cross_pos &cross) noexcept{
    cross_pos neighbours[3];
    if (cross.second == CrossCoord::BOTTOM) {
        neighbours[0] = cross_pos(neighbour<4>(cross.first), CrossCoord::TOP);
        neighbours[1] = cross_pos(coord_t(cross.first.first + 1, cross.first.second - 2),
                                  CrossCoord::TOP);
        neighbours[2] = cross_pos(neighbour<5>(cross.first), CrossCoord::TOP);
    } else {
        neighbours[0] = cross_pos(coord_t(cross.first.first - 1, cross.first.second + 2),
                                  CrossCoord::BOTTOM);
        neighbours[1] = cross_pos(neighbour<2>(cross.first), CrossCoord::BOTTOM);
        neighbours[2] = cross_pos(neighbour<1>(cross.first), CrossCoord::BOTTOM);
    }
    std::set<cross_pos> res;
    for (int i = 0; i < 3; i++) {
        if (belong_field(neighbours[i])) {
            res.insert(neighbours[i]);
        }
    }
    return res;
}

std::set<road_pos> neighbour_roads(const road_pos& road) noexcept{
    road_pos neighbours[4];
    std::set<road_pos> res;
    if (road.second == RoadCoord::UP || road.second == RoadCoord::RIGHT) {
        neighbours[0] = road_pos(neighbour<1>(road.first), RoadCoord::DOWN);
    }
    if (road.second == RoadCoord::DOWN || road.second == RoadCoord::RIGHT) {
        neighbours[1] = road_pos(neighbour<5>(road.first), RoadCoord::UP);
    }
    if (road.second == RoadCoord::UP) {
        neighbours[1] = road_pos(neighbour<2>(road.first), RoadCoord::RIGHT);
        neighbours[2] = road_pos(neighbour<2>(road.first), RoadCoord::DOWN);
    }
    if (road.second == RoadCoord::DOWN) {
        neighbours[0] = road_pos(neighbour<4>(road.first), RoadCoord::RIGHT);
        neighbours[2] = road_pos(neighbour<4>(road.first), RoadCoord::UP);
    }
    if (road.second == RoadCoord::UP || road.second == RoadCoord::DOWN) {
        neighbours[3] = road_pos(road.first, RoadCoord::RIGHT);
    }
    if (road.second == RoadCoord::RIGHT) {
        neighbours[2] = road_pos(road.first, RoadCoord::UP);
        neighbours[3] = road_pos(road.first, RoadCoord::DOWN);
    }
    for (int i = 0; i < 4; i++) {
        if (belong_field(neighbours[i])) {
            res.insert(neighbours[i]);
        }
    }
    return res;
}

std::set<road_pos> neighbour_roads (const cross_pos &cross) noexcept{
    road_pos neigh[3];
    std::set<road_pos> res;
    if (cross.second == CrossCoord::TOP) {
        neigh[0] = road_pos(cross.first, RoadCoord::UP);
        neigh[1] = road_pos(neighbour<2>(cross.first), RoadCoord::RIGHT);
        neigh[2] = road_pos(neighbour<2>(cross.first), RoadCoord::DOWN);
    } else {
        neigh[0] = road_pos(cross.first, RoadCoord::DOWN);
        neigh[1] = road_pos(neighbour<4>(cross.first), RoadCoord::RIGHT);
        neigh[2] = road_pos(neighbour<4>(cross.first), RoadCoord::UP);
    }
    for (int i = 0; i < 3; i++) {
        if (belong_field(neigh[i])) {
            res.insert(neigh[i]);
        }
    }
    return res;
}


std::set<coord_t> neighbour_hexes(const cross_pos &pos) noexcept{
    coord_t neigh[3];
    std::set<coord_t> res;
    neigh[0] = pos.first;
    if (pos.second == CrossCoord::TOP) {
        neigh[1] = neighbour<1>(pos.first);
        neigh[2] = neighbour<2>(pos.first);
    } else {
        neigh[1] = neighbour<4>(pos.first);
        neigh[2] = neighbour<5>(pos.first);
    }
    for (int i = 0; i < 3; i++) {
        if (neigh[i].second >= - 2 * (neigh[i].first >= 0) - (2 + neigh[i].first) * (neigh[i].first < 0)
                && neigh[i].second <= (2 - neigh[i].first) * (neigh[i].first >= 0) + 2 * (neigh[i].first < 0)) {
            res.insert(neigh[i]);
        }
    }
    return res;
}

cross_pos next_cross(const cross_pos &cross, const road_pos &road) noexcept{
    auto crosses = neighbour_crosses(road);
    for (auto x : crosses) {
        if (x != cross) {
            return x;
        }
    }
    return cross_pos(coord_t(-1000, -1000), CrossCoord::BOTTOM);
}
