#include <iostream>
#include "interface.h"
#include "gamefield.h"
using namespace std;

template <class T, class S>
void check_cross(map<T, S> ans) {
    for (auto x : ans) {
        if (neighbour_crosses(x.first) != x.second) {
            std::cout << "!!!" << x.first;
        }
    }
}

template <class T, class S>
void check_road(map<T, S> ans) {
    for (auto x : ans) {
        if (neighbour_roads(x.first) != x.second) {
            std::cout << "!!!" << x.first;
        }
    }
}

template <class T, class S>
void check_hex(map<T, S> ans) {
    for (auto x : ans) {
        if (neighbour_hexes(x.first) != x.second) {
            std::cout << "!!!" << x.first;
        }
    }
}

int main() {
    map<cross_pos, bool> crosses = 
    {{cross_pos(coord_t(0, 0), CrossCoord::TOP), true}, {cross_pos(coord_t(0, 2), CrossCoord::TOP), false},
     {cross_pos(coord_t(-2, 0), CrossCoord::BOTTOM), true}, {cross_pos(coord_t(2, 0), CrossCoord::TOP), true},
     {cross_pos(coord_t(-3, -10), CrossCoord::BOTTOM), false}, {cross_pos(coord_t(10, 1), CrossCoord::BOTTOM), false}};
    for (auto x : crosses) {
        if (belong_field(x.first) != x.second) {
            std::cout << "!!!" << x.first;
        }
    }

    map<coord_t, set<cross_pos>> neigh_crosses_to_hex =
    {{coord_t(0, 0),
      {cross_pos(coord_t(0, 0), CrossCoord::TOP), cross_pos(coord_t(0, 0), CrossCoord::BOTTOM),
      cross_pos(coord_t(0, 1), CrossCoord::BOTTOM), cross_pos(coord_t(0, -1), CrossCoord::TOP),
      cross_pos(coord_t(-1, 1), CrossCoord::BOTTOM), cross_pos(coord_t(1, -1), CrossCoord::TOP)}},
     {coord_t(2, 0),
      {cross_pos(coord_t(2, 0), CrossCoord::TOP), cross_pos(coord_t(2, 0), CrossCoord::BOTTOM),
      cross_pos(coord_t(1, 1), CrossCoord::BOTTOM), cross_pos(coord_t(2, -1), CrossCoord::TOP),}},
     {coord_t(5, 10), {}}};
    check_cross(neigh_crosses_to_hex);

    map<road_pos, set<cross_pos>> neigh_crosses_to_road =
    {{road_pos(coord_t(0, 0), RoadCoord::RIGHT),
      {cross_pos(coord_t(0,1), CrossCoord::BOTTOM), cross_pos(coord_t(1, -1), CrossCoord::TOP)}},
     {road_pos(coord_t(-3,1), RoadCoord::UP),{}},
     {road_pos(coord_t(0, 2), RoadCoord::DOWN),
      {cross_pos(coord_t(0, 2), CrossCoord::BOTTOM), cross_pos(coord_t(1, 1), CrossCoord::TOP)}}};
    check_cross(neigh_crosses_to_road);

    map<cross_pos, set<cross_pos>> neigh_crosses_to_crosses =
    {{cross_pos(coord_t(0, 0), CrossCoord::TOP),
      {cross_pos(coord_t(0,1), CrossCoord::BOTTOM), cross_pos(coord_t(-1, 1), CrossCoord::BOTTOM),
      cross_pos(coord_t(-1, 2), CrossCoord::BOTTOM)}},
     {cross_pos(coord_t(2, -1), CrossCoord::BOTTOM),
      {cross_pos(coord_t(2, -2), CrossCoord::TOP)}},
     {cross_pos(coord_t(5, 10), CrossCoord::BOTTOM), {}}};
     check_cross(neigh_crosses_to_crosses);

     map<road_pos, set<road_pos>> neigh_roads_to_roads =
     {{road_pos(coord_t(0,0), RoadCoord::RIGHT),
       {road_pos(coord_t(0,0), RoadCoord::UP), road_pos(coord_t(0,0), RoadCoord::DOWN),
       road_pos(coord_t(0,1), RoadCoord::DOWN), road_pos(coord_t(1,-1), RoadCoord::UP)}},
      {road_pos(coord_t(5, 100), RoadCoord::DOWN), {}},
      {road_pos(coord_t(-2, 1), RoadCoord::UP),
       {road_pos(coord_t(-2,1), RoadCoord::RIGHT), road_pos(coord_t(-2, 2), RoadCoord::DOWN)}}};
     check_road(neigh_roads_to_roads);

     map<cross_pos, set<coord_t>> neigh_hex_to_cross =
     {{cross_pos(coord_t(0, 0), CrossCoord::TOP),
       {coord_t(0, 0), coord_t(0,1), coord_t(-1, 1)}},
      {cross_pos(coord_t(-1,3), CrossCoord::BOTTOM),
       {coord_t(-1,2), coord_t(0, 2)}},
      {cross_pos(coord_t(5, 10), CrossCoord::BOTTOM), {}}};
     check_hex(neigh_hex_to_cross);

     map<std::pair<cross_pos, road_pos>, cross_pos> next =
     {{{cross_pos(coord_t(0, 0), CrossCoord::TOP), road_pos(coord_t(-1,1), RoadCoord::RIGHT)},
       cross_pos(coord_t(-1, 2), CrossCoord::BOTTOM)},
      {{cross_pos(coord_t(5, 10), CrossCoord::BOTTOM), road_pos(coord_t(5, 10), RoadCoord::DOWN)},
      cross_pos(cross_pos(coord_t(-1000, -1000), CrossCoord::BOTTOM))}
     };
     for (auto x : next) {
         if (next_cross(x.first.first, x.first.second) != x.second) {
             std::cout << "!!!" << x.first.first;
         }
     }
}
