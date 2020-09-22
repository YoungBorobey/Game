#include <iostream>
#include <sstream>
#include <cmath>
#include "game.h"

void fix_string(std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 'a' - 'A';
        }
    }
}

void action_smart_travel(T_MAP& map, C_PLAYER& player, size_t pos_x, size_t pos_y,
            std::vector<std::vector<std::pair<int, int>>>& graph) {
    try {
        auto result = try_fast_travel(map, player, graph, std::make_pair(pos_x, pos_y));
        do_actions(result.value(), player, map, graph);
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }
}

void do_actions(const std::vector<std::string>& actions, C_PLAYER& player, T_MAP& map,
                std::vector<std::vector<std::pair<int, int>>>& graph) {
    bool check = true;
    for (const auto& i : actions) {
        Run_action(i, player, map, graph, check);
    }
}
//

bool is_action_correct(const std::string& action, const C_PLAYER& player, const T_MAP& map) {
    std::vector<std::string> possible_actions = poss_act(player, map);
    size_t i = 0;
    while (i < possible_actions.size() && possible_actions[i] != action) {
        ++i;
    }
    if (i != possible_actions.size() || action == "map size") {
        return true;
    }
    if (action.substr(0, 12) == "smart travel") {
        std::istringstream iss(action);
        std::string str;
        size_t pos_x, pos_y;
        if (iss >> str >> str >> pos_x >> pos_y) {
            return pos_x < map[0].size() && pos_y < map.size();
        }
    }
    return false;
}

void Output(const std::string& action, C_PLAYER& player, T_MAP& map,
                const std::vector<std::vector<std::pair<int, int>>>& graph, bool is_action_ok) {
    if (is_action_ok) {
        if (action.find("move") != std::string::npos) {
            std::cout << "moved\n";
            size_t pl_x = player.get_x(), pl_y = player.get_y();
            if (map[pl_y][pl_x].is_enemy_here()) {
                std::cout << map[pl_y][pl_x].get_enemy().get_name() << " is found\n";
                std::cout << "Prepare for a fight\n\n";
            }
            if (map[pl_y][pl_x].is_item_here()) {
                std::cout << map[pl_y][pl_x].get_item().get_name() << " is found\n\n";
            }
        }
        if (action.find("throw") != std::string::npos) {
            std::cout << "the " << action.substr(6) << " is thrown out\n\n";
        }
        if (action.find("pick") != std::string::npos) {
            std::cout << "the " << action.substr(5) << " is picked\n\n";
        }
        if (action.find("kick") != std::string::npos) {
            std::cout << "Enemy kicked. " << player.get_dmg() << " damage dealt\n";
            if (!map[player.get_y()][player.get_x()].is_enemy_here()) {
                std::cout << "Enemy died\n\n";
            } else {
                C_ENEMY& enemy = map[player.get_y()][player.get_x()].get_enemy();
                std::cout << "Enemy hp: " << enemy.get_hp() << '\n';
                std::cout << "Player kicked. " << std::max(enemy.get_dmg() - player.get_armor(), 1) << " damage dealt\n";
                if (player.is_dead()) {
                    std::cout << "Player died\n\n";
                    exit(1);
                }
            }
        }
        if (action.find("smart travel") != std::string::npos) {
            std::cout << "success!\n";
        }
    } else {
        std::cout << "Invalid action!\n";
        if (player.battle_status()) {
            std::cout << "You are in battle! You should hit the enemy!\n\n";
        }
    }
    if (action == "map size") {
        std::cout << "Map size: " << map[0].size() << " " << map.size() << '\n';
    }
    if (!player.battle_status()) {   // correct output enemy quant
        if (C_CELL::enemy_quant == 0) {
            std::cout << "All enemies defeated\nYou won!\n";
            std::string _;
            try {
                std::getline(std::cin, _);
                exit(0);
            } catch(...) {
                exit(-1);
            }
        }
        std::cout << C_CELL::enemy_quant << " enem";
        if (C_CELL::enemy_quant == 1) {
            std::cout << "y";
        } else {
            std::cout << "ies";
        }
        std::cout << " left\n";
    }
    std::vector<std::string> possible_actions = poss_act(player, map);
    if (!player.battle_status()) {
        std::pair<size_t, size_t> closest_enemy_pos = get_closest_enemy(player, graph, map);
        std::cout << "The nearest enemy position is: " << closest_enemy_pos.first << " " << closest_enemy_pos.second << '\n';
        std::cout << "Enemy type: unknown\n\n";
    }
    std::cout << "Supported actions:" << '\n';
    if (possible_actions.size() == 0) {
      std::cout << '\n';
    } else {
      for (const auto& i : possible_actions) {
        std::cout << " * " << i << '\n';
      }
    }
    std::cout << player.get_x() << " x " << player.get_y() << ", hp: " << player.get_hp()
              << ", armor: " << player.get_armor() << " > \n";
    std::cout << "**************************************\n";
}

std::string Player_Input() {
    try {
        std::string action;
        std::getline(std::cin, action);
        std::istringstream iss(action);
        std::string ans, curr;
        while (iss >> curr) {
        if (ans.size() != 0) {
            ans += " ";
        }
        ans += curr;
        }
        fix_string(ans);
        return ans;
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
        exit(-1);
    }
}

void Run_action(const std::string& action, C_PLAYER& player, T_MAP& map,
        std::vector<std::vector<std::pair<int, int>>>& graph, bool& is_correct) {
    is_correct = is_action_correct(action, player, map);
    if (!is_correct) {
        return;
    }
    if (player.battle_status()) {
        int result = battle(player, map[player.get_y()][player.get_x()].get_enemy());
        if (result == 1) {
            fix_graph(graph, player.get_x(), player.get_y(), map);
            map[player.get_y()][player.get_x()].enemy_dies();
            player.battle_status() = false;
        }
        return;
    }
    if (action.substr(0, 4) == "move") {
        player.step(action);
        if (map[player.get_y()][player.get_x()].is_enemy_here()) {
            player.battle_status() = true;
        }
        return;
    }
    if (action.substr(0, 4) == "pick") {
        player.pick_item(map[player.get_y()][player.get_x()].get_item());
        map[player.get_y()][player.get_x()].item_left();
        return;
    }
    if (action.substr(0, 5) == "throw") {
        player.throw_item(action.substr(6));
        map[player.get_y()][player.get_x()].set_equipment(C_EQUIPMENT(action.substr(6)));
        return;
    }
    if (action == "map size") {
        return;
    }
    std::istringstream iss(action.substr(12));
    size_t pos_x, pos_y;
    iss >> pos_x >> pos_y;
    action_smart_travel(map, player, pos_x, pos_y, graph);
}



