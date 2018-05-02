#include "errors.h"

Error::Error(Error_code x) : code(x) {}

std::string Error::what() {
    switch (code) {
    case Error_code::CANT_ROB:
        return "You can't rob this player";
    case Error_code::CONDITIONS_ERROR:
        return "You can't built this object there";
    case Error_code::NOT_ENOUGH_BANK_RESOURCE:
        return "There's no enough resources in bank to make exchange";
    case Error_code::NOT_ENOUGH_OBJECTS:
        return "You don't have any object cards of this type";
    case Error_code::NOT_ENOUGH_PLAYER_RESOURCE:
        return "You don't have enough resources to build this object";
    case Error_code::NO_PLAYERS:
        return "Register any player before starting game";
    case Error_code::OBJ_EXISTS:
        return "Object in this position already exists";
    case Error_code::PLAYER_EXISTS:
        return "Player with such name exists";
    case Error_code::PROHIBITED_COMMAND:
        return "You can't do this at the moment";
    case Error_code::TOO_MANY_PLAYERS:
        return "Attempt to exceed the maximum number of players";
    case Error_code::MUST_MOVE:
        return "You must move robber somewhere";
    }
}
