#ifndef ENGINE_H
#define ENGINE_H
#include "gamefield.h"
#include <vector>
#include <map>
using std::string;

const Pack<Resource> road_request = Pack<Resource>(0, 1, 0, 0, 1);
const Pack<Resource> town_request = Pack<Resource>(1, 1, 0, 1, 1);
const Pack<Resource> city_request = Pack<Resource>(2, 0, 3, 0, 0);

enum class Cmd_from_user{REGISTER_PLAYER, START, REGISTER_OBJ, MOVE_ROBBER,
                         ROB, MAKE_DICE, EXCHANGE_RES, END_EXCHANGE,
                        BUILD, END_BUILDING};

struct Command {
   Cmd_from_user type;
   string name;
   coord_t hex;
   ObjectType obj_type;
   road_pos road;
   cross_pos cross;
   Resource player_res;
   Resource bank_res;
};

class Engine
{
    field_ptr field = nullptr;
    enum class GameState{FINAL, PLAYERS_REGISTRATION, PUT_INITIAL_INFRASTRUCTURES_DIRECT, PUT_INITIAL_INFRASTRUCTURE_REVERCE,
                         STAGE1_DICE, STAGE1_DROP_RECOURCES, STAGE1_MOVE_ROBBER, STAGE1_ROBBING, STAGE2, STAGE3};

    GameState state = GameState::PLAYERS_REGISTRATION;
    long player_num = -1;
    std::vector<Player> players;
    void undo_obj_registration(road_pos);
    void get_init_resource(cross_pos);
    void get_resources();
    void join_player(const string &player_name);
    void start_game();
    void register_road(const road_pos&);
    void register_town(const cross_pos&);
    void put_initial_infrastructure(cross_pos, road_pos);
    void make_dice();
    bool drop_resource(Player&);
    void move_robber(coord_t pos);
    bool rob(Player& robbed);
    void exchange_with_field(Resource player_resourse,
                             Resource bank_resource);
    void build_road(road_pos);
    void build_town(cross_pos);
    void build_city(cross_pos);
    bool next_player();
public:
    Engine();
    void game(const Command&);
    field_ptr get_field();
    std::vector<Player> get_players();
};

#endif // DRIVER_H
