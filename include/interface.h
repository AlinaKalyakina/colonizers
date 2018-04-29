#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include <memory>
#include "engine.h"

using std::string;


class Interface
{
    const Engine engine;
    const field_ptr field = nullptr;
    Command cmd;
    void show_field();
    void get_cmd();
public:
    void play();
    Interface(Engine);
};

#endif // INTERFACE_H
