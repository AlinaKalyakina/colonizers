#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include <memory>
#include "engine.h"
#include <map>

using std::string;
enum class User_cmd{REG_PLAYER, START, PUT_INFRASTR, MAKE_DICE, MOVE_ROBBER, ROB,
                    BUILD_ROAD, BUILD_TOWN, BUILD_CITY, EXCHANGE_RES, END_EXCHANGE,
                   NEXT_PLAYER};
const std::map<string, User_cmd> commands =
{{"reg_player", User_cmd::REG_PLAYER}, {"start", User_cmd::START},
 {"put_inf", User_cmd::PUT_INFRASTR}, {"dice", User_cmd::MAKE_DICE},
 {"mode_robber", User_cmd::MOVE_ROBBER}, {"rob", User_cmd::ROB}, {"build_road", User_cmd::BUILD_ROAD},
 {"build_city", User_cmd::BUILD_ROAD}, {"build_town", User_cmd::BUILD_TOWN}, {"exchange", User_cmd::EXCHANGE_RES},
 {"end_exchange", User_cmd::END_EXCHANGE}, {"next", User_cmd::NEXT_PLAYER}};

const std::map<string, CrossCoord> cross_specific =
{{"t",CrossCoord::TOP}, {"b", CrossCoord::BOTTOM}};

const std::map<string, RoadCoord> road_specific =
{{"u", RoadCoord::UP}, {"r", RoadCoord::RIGHT}, {"d", RoadCoord::DOWN}};

const std::map<string, Resource> resource_names =
{{"grain", Resource::GRAIN}, {"claim", Resource::CLAIM}, {"wood", Resource::WOOD},
 {"ore", Resource::ORE}, {"wool", Resource::WOOL}, {"desert", Resource::NO_RESOURCE}};

const std::map<string, ObjectType> object_names =
{{"city", ObjectType::CITY}, {"town", ObjectType::TOWN}, {"road", ObjectType::ROAD}};

const string introduction = "write cmd";

class Interface
{
    Engine engine;
    const field_ptr field = nullptr;
    bool end_of_game = false;
    void show_field();
    User_cmd get_cmd();
    void choose_function(User_cmd);
public:
    void play();
    Interface();
};

#endif // INTERFACE_H
