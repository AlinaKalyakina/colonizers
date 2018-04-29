#include "gamefield.h"

Player::Player(const std::string &x) : name(x) {}

Hex::Hex(coord_t pos, Resource t) : pos(pos), type(t) {}

Object::Object(int player, ObjectType t) : owner(player), type(t) {}


template <int N>
coord_t neighbour(coord_t& x) {
    coord_t shifts[6] = {coord_t(1, 0), coord_t(0, 1), coord_t(-1, 1),
                        coord_t(-1, 0), coord_t(0, -1), coord_t(1, -1)};
    return coord_t(x.first + shifts[N].first, x.second +shifts[N].second);
}

bool belong_field(road_pos road) {
    auto x = road.first.first;
    auto y = road.first.second;
    switch (road.second) {
    case RoadCoord::DOWN:
        if (x >= -2 && x <= 1 &&
                y >= (- 2 * (x >= 0) - (2 + x) * (x < 0)) &&
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

bool belong_field(cross_pos cross) {
    auto neigh = neighbour_roads(cross);
    for (auto x : neigh) {
        if (belong_field(x)) {
            return true;
        }
    }
    return false;
}

std::vector<cross_pos> neighbour_crosses(coord_t hex) {
    std::vector<cross_pos> result;
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
            result.push_back(neigh[i]);
        }
    }
    return result;
}

std::vector<cross_pos> neighbour_crosses(road_pos road) {
    cross_pos pos1, pos2;
    if (road.second == RoadCoord::UP ||  road.second ==RoadCoord::RIGHT) {
        pos1 = cross_pos(neighbour<2>(road.first), CrossCoord::BOTTOM);
    } else {
        pos2 = cross_pos(neighbour<5>(road.first), CrossCoord::TOP);
    }
    std::vector<cross_pos> res;
    if (road.second == RoadCoord::UP) {
        pos2 = cross_pos(road.first, CrossCoord::TOP);
    }
    if (road.second == RoadCoord::DOWN) {
        pos1 = cross_pos(road.first, CrossCoord::BOTTOM);
    }
    if (belong_field(pos1)) {
        res.push_back(pos1);
    }
    if (belong_field(pos2)) {
        res.push_back(pos2);
    }
    return res;
}

std::vector<cross_pos> neighbour_crosses(cross_pos cross) {
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
    std::vector<cross_pos> res;
    for (int i = 0; i < 3; i++) {
        if (belong_field(neighbours[i])) {
            res.push_back(neighbours[i]);
        }
    }
    return res;
}

std::vector<road_pos> neighbour_roads(road_pos road) {
    road_pos neighbours[4];
    std::vector<road_pos> res;
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
            res.push_back(neighbours[i]);
        }
    }
    return res;
}

std::vector<road_pos> neighbour_roads (cross_pos cross) {
    road_pos neigh[3];
    std::vector<road_pos> res;
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
            res.push_back(neigh[i]);
        }
    }
    return res;
}

cross_pos next_cross(cross_pos cross, road_pos road) {
    auto crosses = neighbour_crosses(road);
    for (auto x : crosses) {
        if (x != cross) {
            return x;
        }
    }
    return cross_pos(coord_t(-1000, -1000), CrossCoord::BOTTOM);
}
