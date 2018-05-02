#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include <memory>
#include "engine.h"
#include <map>

using std::string;
enum class User_cmd{REG_PLAYER, START, PUT_INFRASTR, MAKE_DICE, MOVE_ROBBER, ROB,
                    BUILD_ROAD, BUILD_TOWN, BUILD_CITY, EXCHANGE_RES, END_EXCHANGE,
                   NEXT_PLAYER, SET_FIELD, SET_DICE, SET_ROB, EXIT};
const std::map<string, User_cmd> commands =
{{"reg", User_cmd::REG_PLAYER}, {"start", User_cmd::START}, {"set_field", User_cmd::SET_FIELD},
 {"put_inf", User_cmd::PUT_INFRASTR}, {"dice", User_cmd::MAKE_DICE}, {"set_dice", User_cmd::SET_DICE},
 {"move_robber", User_cmd::MOVE_ROBBER}, {"rob", User_cmd::ROB}, {"build_road", User_cmd::BUILD_ROAD},
 {"build_city", User_cmd::BUILD_CITY}, {"build_town", User_cmd::BUILD_TOWN}, {"exchange", User_cmd::EXCHANGE_RES},
 {"end_exchange", User_cmd::END_EXCHANGE}, {"next", User_cmd::NEXT_PLAYER}, {"set_rob", User_cmd::SET_ROB}, {"exit", User_cmd::EXIT}};

const std::map<string, CrossCoord> cross_specific =
{{"t",CrossCoord::TOP}, {"b", CrossCoord::BOTTOM}};

const std::map<string, RoadCoord> road_specific =
{{"u", RoadCoord::UP}, {"r", RoadCoord::RIGHT}, {"d", RoadCoord::DOWN}};

const std::map<string, Resource> resource_names =
{{"grain", Resource::GRAIN}, {"claim", Resource::CLAIM}, {"wood", Resource::WOOD},
 {"ore", Resource::ORE}, {"wool", Resource::WOOL}, {"desert", Resource::NO_RESOURCE}};

const std::map<string, ObjectType> object_names =
{{"city", ObjectType::CITY}, {"town", ObjectType::TOWN}, {"road", ObjectType::ROAD}};

const string introduction = "WRITE COMMAND";

const string field_code = "2 claim -2 1 3 wood -1 -1 3 grain 0 -2 4 grain -2 2 4 wool 1 1 "
                          "5 wood -1 1 5 ore 0 -1 6 wood -1 2 6 grain 1 -2 7 desert 0 0 "
                          "8 ore 1 0 8 claim -1 0 9 grain 0 2 9 wool -2 0 10 wool 2 0 "
                          "10 claim 1 -1 11 wood 2 -1 11 ore 2 -2 12 wool 0 1";

class Interface
{
    Player curplayer;
    Engine engine;
    field_ptr field = nullptr;
    bool end_of_game = false;
    void show_field();
    void show_hexes();
    User_cmd get_cmd();
    void choose_function(User_cmd);
public:
    void play();
    Interface();
};

std::ostream& operator<< (std::ostream& cout, cross_pos x);
std::ostream& operator<< (std::ostream& cout, coord_t x);
std::ostream& operator<< (std::ostream& cout, road_pos x);
field_ptr create_field();

#endif // INTERFACE_H
