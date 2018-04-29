#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include <memory>
#include "gamefield.h"

using std::string;

enum class Cmd_from_user{REGISTER_PLAYER, START, REGISTER_OBJ, MOVE_ROBBER,
                         ROB, MAKE_DICE, EXCHANGE_RES, END_EXCHANGE};

struct Command {
   Cmd_from_user type;
   string name;
   coord_t hex;
   ObjectType obj_type;
   road_pos road;
   cross_pos cross;
};


typedef std::shared_ptr<Command> Command_ptr;

class Interface
{
    const field_ptr field;
public:
    void show_introduction();
    Command_ptr get_user_cmd();
    void show_error();
    void show_field();
    void show_player_info(const Player&);
    void show_robbed(const Player&);
    Interface();

};

#endif // INTERFACE_H
