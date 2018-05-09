#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include <map>
#include <set>
#include <memory>
#include <vector>

enum class Limits: int {WIN_SCORE = 5, MAGIC_NUM = 7, MAX_NUM_OF_PLAYERS = 4, MIN_NUM_OF_PLAYERS = 3, MAX_RECOURCE_CARD_NUM = 19,
                                  MAX_TOWN_NUM = 4, MAX_CITY_NUM = 3, MAX_ROAD_NUM = 15, EXCHANGE_RATE = 2};

enum class Resource : int {NUMBER = 5, GRAIN = 0, CLAIM = 1, ORE = 2, WOOL = 3, WOOD = 4, NO_RESOURCE = 6};
enum class RoadCoord{UP, RIGHT, DOWN};
enum class CrossCoord{TOP, BOTTOM};
enum class ObjectType: int {NUMBER = 3, ROAD = 0, CITY = 1, TOWN = 2};


typedef std::pair<int, int> coord_t; 
typedef std::pair<coord_t, RoadCoord> road_pos;
typedef std::pair<coord_t, CrossCoord> cross_pos;


template <class T>
struct Pack {
    int card[int(T::NUMBER)];
    Pack(int grain, int claim, int ore, int wool, int wood);
    Pack(int roads, int towns, int cities);
    int& operator[] (T n);
    Pack<T>& operator- (const Pack<T>& x);
    Pack<T>& operator-= (const Pack<T>& x);
    Pack<T>& operator+= (const Pack<T>& x);
    bool operator< (const Pack<T>& x);
    bool operator>= (const Pack<T>& x);
    int sum();
};

template <class T>
Pack<T>::Pack(int grain, int claim, int ore, int wool, int wood) {
    card[0] = grain;
    card[1] = claim;
    card[2] = ore;
    card[3] = wool;
    card[4] = wood;
}

template<class T>
Pack<T>::Pack(int roads, int towns, int cities) {
    card[0] = roads;
    card[1] = towns;
    card[2] = cities;
}

template <class T>
int& Pack<T>::operator[] (T n) {
    return card[int(n)];
}

template <class T>
Pack<T>& Pack<T>::operator-= (const Pack<T>& x) {
    for (int i = 0; i < int(T::NUMBER); i++) {
        card[i] -= x.card[i];
    }
    return *this;
}

template <class T>
Pack<T>& Pack<T>::operator+= (const Pack<T>& x) {
    for (int i = 0; i < int(T::NUMBER); i++) {
        card[i] += x.card[i];
    }
    return *this;
}

template <class T>
bool Pack<T>::operator< (const Pack<T>& x) {
    for (int i = 0; i < int(T::NUMBER); i++) {
        if (card[i] >= x.card[i]) return false;
    }
    return true;
}

template<class T>
bool Pack<T>::operator>= (const Pack<T>& x) {
    for (int i = 0; i < int(T::NUMBER); i++) {
        if (card[i] < x.card[i]) return false;
    }
    return true;
}

template <class T>
int Pack<T>::sum() {
    int sum = 0;
    for(int i = 0; i < int(T::NUMBER); i++) {
        sum += card[i];
    }
    return sum;
}

struct Player
{
    long id;
    std::string name;
    Pack<Resource> resource_cards = {0,0,0,0,0};
    Pack<ObjectType> object_cards = Pack<ObjectType>(int(Limits::MAX_ROAD_NUM),
              int(Limits::MAX_CITY_NUM), int(Limits::MAX_TOWN_NUM));
    int score = 0;
    Player(const std::string &name = "", int num = -1);
};

typedef std::shared_ptr<Player> player_ptr;

struct Hex {
    coord_t pos;
    Resource type = Resource::NO_RESOURCE;
    Hex(coord_t = std::make_pair(0, 0), Resource type = Resource::NO_RESOURCE);
};

struct Object {
    long owner;
    ObjectType type;
    Object(int owner, ObjectType type = ObjectType::ROAD);
};

struct GameField {
    std::multimap<int, Hex> hexes;
    std::map<road_pos, Object> roads;
    std::map<cross_pos, Object> settlements;
    Pack<Resource> bank = {19, 19, 19, 19, 19};
    coord_t robber_pos;
    int dice_score = 0;
};

typedef std::shared_ptr<GameField> field_ptr;

std::set<coord_t> neighbour_hexes(const cross_pos &) noexcept;
std::set<cross_pos> neighbour_crosses(const coord_t &) noexcept;
std::set<cross_pos> neighbour_crosses(const road_pos&) noexcept;
std::set<cross_pos> neighbour_crosses(const cross_pos&) noexcept;
std::set<road_pos> neighbour_roads (const road_pos&) noexcept;
std::set<road_pos> neighbour_roads (const cross_pos&) noexcept;
bool belong_field(const cross_pos& cross) noexcept;
bool belong_field(const road_pos &road) noexcept;
cross_pos next_cross(const cross_pos&, const road_pos&) noexcept;
#endif // GAMEFIELD_H
