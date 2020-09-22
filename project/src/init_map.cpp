#include <fstream>
#include <iostream>
#include <set>
#include <ctime>
#include <random>
//#include <filesystem>

#include "game.h"

T_MAP create_map_from_file(const char* filepath) {
  std::ifstream file;
  file.open(filepath, std::fstream::basic_ios::in);
  try {
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file");
    }
    size_t rows = 0, cols = 0;
    file >> cols >> rows;

    T_MAP map(rows, std::vector<C_CELL>(cols));

    std::set<std::string> Enemy_set(
        {"wolf", "dog", "rat"});
    std::set<std::string> Armor_set(
        {"armor", "helmet", "shield", "pants", "t-shirt"});
    int x, y;
    std::string name;
    while (file >> x >> y >> name) {
      if (Enemy_set.find(name) != Enemy_set.end()) {
        map[y][x].set_enemy(C_ENEMY(name));
      } else if (Armor_set.find(name) != Armor_set.end()) {
        map[y][x].set_equipment(C_EQUIPMENT(name));
      }
    }
    file.close();
    return map;
  } catch (std::runtime_error& e) {
    set_random_map(filepath);
    return create_map_from_file(filepath);
  }
}

void set_random_map(const char* filepath) {
    //std::filesystem::create_directory("res");
    std::ofstream out(filepath, std::ios::out | std::ios::trunc);
    std::random_device rd;
    std::optional<std::mt19937> init_gen;
    if (rd.entropy() == 0) {
        init_gen.emplace(std::mt19937(time(0)));
    } else {
        init_gen.emplace(std::mt19937(rd()));
    }
    std::mt19937 gen = init_gen.value();
    std::uniform_int_distribution<> range_map(5, 10);    // init map range
    size_t cols = range_map(gen), rows = range_map(gen);
    out << cols  << " " << rows << "\n";
    std::vector<std::string> enemies({"wolf", "dog", "rat"});    // possible enemies
    std::vector<std::string> items({"armor", "helmet", "shield", "pants", "t-shirt"});   // possible items
    std::set<std::pair<size_t, size_t>> used_nodes;     // to create valid map
    std::uniform_int_distribution<> range_things((cols + rows) / 4, (cols + rows) / 2);
    size_t enemy_quant = range_things(gen);
    size_t item_quant = range_things(gen);
    for (size_t i = 0; i < enemy_quant; ++i) {
        size_t pos_x = gen() % cols, pos_y = gen() % rows;
        while (used_nodes.find(std::make_pair(pos_x, pos_y)) != used_nodes.end()) {
            pos_x = gen() % cols;
            pos_y = gen() % rows;
        }
        used_nodes.insert(std::make_pair(pos_x, pos_y));
        out << pos_x << " " << pos_y << " " << enemies[gen() % enemies.size()] << "\n";
    }
    for (size_t i = 0; i < item_quant; ++i) {
        size_t pos_x = gen() % cols, pos_y = gen() % rows;
        while (used_nodes.find(std::make_pair(pos_x, pos_y)) != used_nodes.end()) {
            pos_x = gen() % cols;
            pos_y = gen() % rows;
        }
        used_nodes.insert(std::make_pair(pos_x, pos_y));
        out << pos_x << " " << pos_y << " " << items[gen() % items.size()] << "\n";
    }
    out.close();
}


void view_description() {
    std::cout << "Starting Game...\nWelcome to the Game!\nDefeat all enemies to pass the level\nGood luck!\n\n";
}
