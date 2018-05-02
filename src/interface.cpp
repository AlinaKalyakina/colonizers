#include "interface.h"
#include <iostream>
#include "errors.h"
#include <sstream>

template<class T>
T& operator>> (T& cin, coord_t &pos) {
    int x, y;
    cin >> x >> y;
    pos = coord_t(x, y);
    return cin;
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

void Interface::show_hexes() {
    std::cout << "Hexes generated:" << std::endl;
    for (auto x : field->hexes) {
        std::cout << x << std::endl;
    }
}

std::istream& operator>> (std::istream& cin, cross_pos &pos) {
    coord_t hex;
    cin >> hex;
    string specific;
    cin >> specific;
    auto it = cross_specific.find(specific);
    if (it == cross_specific.end()) {
         "Nonexistent specificator " + specific;
    }
    pos = cross_pos(hex, it->second);
    return cin;
}

std::istream& operator>> (std::istream& cin, road_pos &pos) {
    coord_t hex;
    cin >> hex;
    string specific;
    cin >> specific;
    auto it = road_specific.find(specific);
    if (it == road_specific.end()) { 
         "Nonexistent specificator " + specific;
    }
    pos = road_pos(hex, it->second);
    return cin;
}



field_ptr create_field() {
    field_ptr res = std::make_shared<GameField>(GameField());
    std::istringstream stream(field_code);
    while (res->hexes.size() != 19) {
        int num;
        string str;
        coord_t coord;
        stream >> num;
        stream >> str;
        stream >> coord;
        res->hexes.insert(std::make_pair(num, Hex(coord, resource_names.find(str)->second)));
        if (num == 7) {
            res->robber_pos = coord;
        }
    }
    return res;
}

Interface::Interface() : field(engine.get_field()) {}

User_cmd Interface::get_cmd() {
    string cmd;
    std::cin >> cmd;
    auto it = commands.find(cmd);
    if (it == commands.end()) {
        throw "Nonexistent command " + cmd;
    }
    return it->second;
}

void Interface::play() {
    while (!end_of_game) {
        std::cout << introduction;
        if (curplayer.id != -1) {
            std::cout << ", " + curplayer.name << std::endl;
        } else {
            std::cout << std::endl;
        }
        //CHANGES
        try {
            User_cmd cmd;
            cmd = get_cmd();
            choose_function(cmd);
        }
        catch (string x) {
            std::cout << x << std::endl;
            continue;
        }
        catch (Error x) {
            std::cout << x.what() << std::endl;
        }
        //CHANGES
        curplayer = engine.get_cur_player();
        show_field();
    }
    std::cout << curplayer.name << " won!!!" << std::endl;
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
        show_hexes();
        break;
    case User_cmd::SET_FIELD:
        engine.start_game(create_field());
        field = engine.get_field();
        show_hexes();
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
    case User_cmd::SET_DICE: {
        int n;
        std::cin >> n;
        if (n < 2 || n > 12) {
            throw "Choose between 2 and 12";
        }
        engine.make_dice(n);
        }
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
    case User_cmd::SET_ROB: {
        string name, res;
        std::cin >> name;
        std::cin >> res;
        auto it = resource_names.find(res);
        if (it == resource_names.end()) {
            throw "Bad command!";
        }
        engine.rob(name, it->second);
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
            throw "Nonexistent specificator ";
        }
        engine.exchange_with_field(it1->second, it2->second);
        break;
    }
    case User_cmd::END_EXCHANGE:
        engine.end_exchanges();
        break;
    case User_cmd::NEXT_PLAYER:
        end_of_game = !engine.next_player();
        break;
    case User_cmd::EXIT:
        exit(0);
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
                 x.score << std::endl << "Resourses: " << x.resource_cards << std::endl <<
                 "Rest object cards: "<< x.object_cards;
    return cout;
}

void Interface::show_field() {
    std::cout << "**********FIELD**********" << std::endl;
    std::cout << "Roads:\n";
    for (auto x : field->roads) {
        std::cout << x << std::endl;
    }
    std::cout << "Settlements:\n";
    for (auto x : field->settlements) {
        std::cout << x << std::endl;
    }
    std::cout << "Bank: " << field->bank << std::endl;
    std::cout << "Dice score: " << field->dice_score << std::endl;
    std::cout << "Robber_pos: " << field->robber_pos << std::endl;
    std::cout << "**********PLAYERS**********" << std::endl;
    auto players = engine.get_players();
    for (auto x : players) {
        std::cout << x << std::endl;
    }
}
