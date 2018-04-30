#ifndef ENGINE_H
#define ENGINE_H
#include "gamefield.h"
#include <vector>
#include <map>
using std::string;

const Pack<Resource> road_request = Pack<Resource>(0, 1, 0, 0, 1);
const Pack<Resource> town_request = Pack<Resource>(1, 1, 0, 1, 1);
const Pack<Resource> city_request = Pack<Resource>(2, 0, 3, 0, 0);
enum class GameState{FINAL, PLAYERS_REGISTRATION, PUT_INITIAL_INFRASTRUCTURES_DIRECT, PUT_INITIAL_INFRASTRUCTURE_REVERCE,
                     STAGE1_DICE, STAGE1_MOVE_ROBBER, STAGE1_ROBBING, STAGE2, STAGE3};
class Engine
{
    field_ptr field = nullptr;
    GameState state = GameState::PLAYERS_REGISTRATION;
    long player_num = -1;
    std::vector<Player> players;
    void undo_obj_registration(road_pos);
    void get_init_resource(cross_pos);
    void get_resources();
    void register_road(const road_pos&);
    void register_town(const cross_pos&);
    bool drop_resource(Player&);
public:
    void join_player(const string &player_name);
    void start_game();
    void put_initial_infrastructure(cross_pos, road_pos);
    void make_dice();
    void move_robber(const coord_t &pos);
    bool rob(const string &name);
    void exchange_with_field(Resource player_resourse,
                             Resource bank_resource);
    void end_exchanges();
    void build_road(road_pos);
    void build_town(cross_pos);
    void build_city(cross_pos);
    bool next_player();
    Engine();
    field_ptr get_field() const;
    std::vector<Player> get_players() const;
    int get_cur_player() const;
    GameState get_cur_state() const;
    void set_state(GameState x) {
        state = x;
    }
    void insert_player() {
        players.push_back(Player());
    }
};

#endif // DRIVER_H
