#ifndef ERRORS_H
#define ERRORS_H
#include <string>

enum class Error_code
{PLAYER_EXISTS, TOO_MANY_PLAYERS, NO_PLAYERS, PROHIBITED_COMMAND,
    OBJ_EXISTS, CONDITIONS_ERROR, NOT_ENOUGH_OBJECTS, NOT_ENOUGH_PLAYER_RESOURCE,
    CANT_ROB, NOT_ENOUGH_BANK_RESOURCE, MUST_MOVE            };

class Error {
    Error_code code;
public:
    Error(Error_code);
    std::string what();
};
#endif // ERRORS_H
