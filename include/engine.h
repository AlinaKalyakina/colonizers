#ifndef ENGINE_H
#define ENGINE_H
#include "gamefield.h"
#include <vector>
#include <map>
#include "interface.h"
using std::string;

const Pack<Resource> road_request = Pack<Resource>(0, 1, 0, 0, 1);
const Pack<Resource> town_request = Pack<Resource>(1, 1, 0, 1, 1);
const Pack<Resource> city_request = Pack<Resource>(2, 0, 3, 0, 0);

struct Request {
    int player_from;
    int what[5], into[5];
    Request(int from, int what[5], int into[5]);
};

class Engine
{
    Interface interface;
    field_ptr field = nullptr;
    enum class GameState{INTRODUCTION, PLAYERS_REGISTRATION, INFRASTRUCTURES_REGISTRATION, PUT_INITIAL_INFRASTRUCTURES,
                         STAGE1_DICE, STAGE1_DROP_RECOURCES, STAGE1_MOVE_ROBBER, STAGE1_ROBBING, STAGE2, STAGE3};

    GameState state = GameState::INTRODUCTION;
    int player_num = 0;
    std::multimap<int, Request> requests;
    std::vector<Player> players;
    bool check_way(cross_pos pos, road_pos prev_road);
    cross_pos initial_registration(const Player&);
    void get_init_resource(cross_pos, Player&);
    void get_resources();
    void join_player(const string &player_name);
    void start_game();
    void register_road(int player_num, road_pos);
    void register_settlement(int player_num, const cross_pos&, ObjectType);
    //void put_initial_infrastructure();
    void make_dice();
    void drop_resource(Player&);
    void move_robber(coord_t pos);
    void rob(int player_num);
    void exchange_with_field(Resource player_resourse,
                             Resource bank_resource);
    void exchange_players_request(int player_to, int player_from,
                                  int resource_from[5], int resource_to[5]);
    void exchange_players_accept(std::multimap<int, Request>::iterator pos);
    void end_exchanges();
    void build_road(road_pos);
    void build_town(cross_pos);
    void build_city(cross_pos);
    void next_player();
public:
    Engine();
    void game();
    field_ptr get_field();
    std::vector<Player> get_players();
};

#endif // DRIVER_H
