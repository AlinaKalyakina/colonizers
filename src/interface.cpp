#include "interface.h"
#include <iostream>


std::istream& operator>> (std::istream& cin, coord_t &pos) {
    int x, y;
    std::cin >> x >> y;
    pos = coord_t(x, y);
    return cin;
}

std::istream& operator>> (std::istream& cin, cross_pos &pos) {
    coord_t hex;
    std::cin >> hex;
    string specific;
    std::cin >> specific;
    auto it = cross_specific.find(specific);
    if (it == cross_specific.end()) {
        throw ;
    }
    pos = cross_pos(hex, it->second);
    return cin;
}

std::istream& operator>> (std::istream& cin, road_pos &pos) {
    coord_t hex;
    std::cin >> hex;
    string specific;
    std::cin >> specific;
    auto it = road_specific.find(specific);
    if (it == road_specific.end()) {
        throw ;
    }
    pos = road_pos(hex, it->second);
    return cin;
}

Interface::Interface() : field(engine.get_field()) {}

User_cmd Interface::get_cmd() {
    string cmd;
    std::cin >> cmd;
    auto it = commands.find(cmd);
    if (it == commands.end()) {
        throw "!";
    }
    return it->second;
}

void Interface::play() {
    while (!end_of_game) {
        std::cout << introduction << std::endl;
        //CHANGES
        try {
            User_cmd cmd;
            cmd = get_cmd();
            choose_function(cmd);
        }
        catch (...) {

        };
        //CHANGES
        show_field();
    }
}

void Interface::choose_function(User_cmd cmd) {
    switch (cmd) {
    case User_cmd::REG_PLAYER: {
        string name;
        std::cin >> name;
        engine.join_player(name);
        }
        break;
    case User_cmd::START:
        engine.start_game();
        break;
    case User_cmd::PUT_INFRASTR: {
        road_pos road;
        cross_pos cross;
        std::cin >> road >> cross;
        engine.put_initial_infrastructure(cross, road);
        }
        break;
    case User_cmd::MAKE_DICE:
        engine.make_dice();
        break;
    case User_cmd::MOVE_ROBBER: {
        coord_t pos;
        std::cin >> pos;
        engine.move_robber(pos);
        }
        break;
    case User_cmd::ROB: {
        string name;
        std::cin>> name;
        engine.rob(name);
        }
        break;
    case User_cmd::BUILD_ROAD: {
        road_pos pos;
        std::cin >> pos;
        engine.build_road(pos);
        }
        break;
    case User_cmd::BUILD_TOWN: {
        cross_pos pos;
        std::cin >> pos;
        engine.build_town(pos);
        }
        break;
    case User_cmd::BUILD_CITY: {
        cross_pos pos;
        std::cin >> pos;
        engine.build_city(pos);
        }
        break;
    case User_cmd::EXCHANGE_RES: {
        string res1, res2;
        std::cin >> res1 >> res2;
        auto it1 = resource_names.find(res1);
        auto it2 = resource_names.find(res2);
        if (it1 == resource_names.end() || it2 == resource_names.end()) {
            throw;
        }
        engine.exchange_with_field(it1->second, it2->second);
        break;
    }
    case User_cmd::END_EXCHANGE:
        engine.end_exchanges();
        break;
    case User_cmd::NEXT_PLAYER:
        end_of_game = !engine.next_player();
    }
}

std::ostream& operator<< (std::ostream& cout, coord_t x) {
    std::cout << "(" << x.first << "," << x.second << ")";
    return cout;
}

std::ostream& operator<< (std::ostream& cout, road_pos x) {
    string str;
    for (auto it : road_specific) {
        if (x.second == it.second) {
            str = it.first;
            break;
        }
    }
    std::cout << x.first << " " + str;
    return cout;
}

std::ostream& operator<< (std::ostream& cout, cross_pos x) {
    string str;
    for (auto it : cross_specific) {
        if (x.second == it.second) {
            str = it.first;
            break;
        }
    }
    std::cout << x.first << " " + str;
    return cout;
}

std::ostream& operator<< (std::ostream& cout, std::pair<int, Hex> x) {
    string res;
    for (auto it : resource_names) {
        if (x.second.type == it.second) {
            res = it.first;
            break;
        }
    }
    std::cout << "Hex #" << x.first << " at pos " << x.second.pos << " with resource: " << res;
    return cout;
}

std::ostream& operator<< (std::ostream& cout, std::pair<road_pos, Object> x) {
    std::cout << "At pos " << x.first << ", owner: " << x.second.owner;
    return cout;
}

std::ostream& operator<< (std::ostream& cout, std::pair<cross_pos, Object> x) {
    std::cout << "At pos " << x.first << ", owner: " << x.second.owner << ", type: ";
    if (x.second.type == ObjectType::CITY) {
        std::cout << "city";
    } else {
        std::cout << "town";
    }
    return cout;
}

std::ostream& operator<< (std::ostream& cout, Pack<Resource> x) {
    for (int i = 0; i < int(Resource::NUMBER); i++) {
        string str;
        for (auto it : resource_names) {
            if (it.second == Resource(i)) {
                str = it.first;
                break;
            }
        }
        std::cout << str << ":" << x[Resource(i)] << "     ";
    }
    return cout;
}

std::ostream& operator<< (std::ostream& cout, Pack<ObjectType> x) {
    for (int i = 0; i < int(ObjectType::NUMBER); i++) {
        string str;
        for (auto it : object_names) {
            if (it.second == ObjectType(i)) {
                str = it.first;
                break;
            }
        }
        std::cout << str << ": " << x[ObjectType(i)] << "     ";
    }
    return cout;
}

std::ostream& operator<< (std::ostream& cout, Player x) {
    std::cout << "Player #" << x.id << ", name: " << x.name << ", score: " <<
                 x.score << std::endl << "Resourses:" << std::endl << x.resource_cards << std::endl <<
                 "Rest object cards:" << std::endl << x.object_cards;
    return cout;
}

void Interface::show_field() {
    std::cout << "**********FIELD**********" << std::endl << "Hexes:\n";
    for (auto x : field->hexes) {
        std::cout << x << std::endl;
    }
    std::cout << "Roads:\n";
    for (auto x : field->roads) {
        std::cout << x << std::endl;
    }
    std::cout << "Settlements:\n";
    for (auto x : field->settlements) {
        std::cout << x << std::endl;
    }
    std::cout << "Bank:" << std::endl << field->bank << std::endl;
    std::cout << "Dice score:" << std::endl << field->dice_score << std::endl;
    std::cout << "Robber_pos:" << std::endl << field->robber_pos << std::endl;
    std::cout << "**********PLAYERS**********" << std::endl;
    auto players = engine.get_players();
    for (auto x : players) {
        std::cout << x << std::endl;
    }
}
