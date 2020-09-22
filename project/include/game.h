#ifndef PROJECT_INCLUDE_GAME_H_
#define PROJECT_INCLUDE_GAME_H_
#include <optional>
#include <string>
#include <utility>

#include "entity.h"
#include "game.h"
#include "item.h"
#include "vector"

class C_CELL {
 public:
  std::optional<C_EQUIPMENT> equipment;

  std::optional<C_ENEMY> enemy;

  static int enemy_quant;

  explicit C_CELL(const C_ENEMY& enemy_here);

  explicit C_CELL(const C_EQUIPMENT& eq);

  C_CELL();

  void set_enemy(const C_ENEMY& an_enemy);

  void set_equipment(const C_EQUIPMENT& item);

  bool is_enemy_here() const;

  bool is_item_here() const;

  void enemy_dies();

  void item_left();

  C_ENEMY& get_enemy();

  const C_ENEMY& get_enemy() const;

  C_EQUIPMENT& get_item();

  const C_EQUIPMENT& get_item() const;
};

typedef std::vector<std::vector<C_CELL>> T_MAP;

T_MAP create_map_from_file(const char* filepath);

int battle(C_PLAYER& p, C_ENEMY& e);

std::vector<std::string> poss_act(const C_PLAYER& player,
                                  const std::vector<std::vector<C_CELL>>& Map);

std::pair<size_t, size_t> get_pos(int node_numb, const T_MAP& DATA);

int get_node_numb(const T_MAP& DATA, size_t pos_x, size_t pos_y);

std::vector<std::vector<std::pair<int, int>>> get_graph(const T_MAP& DATA, const C_PLAYER& player);

std::vector<std::pair<int, int>> find_distance(const std::vector<std::vector<std::pair<int, int>>>& graph, int start, int finish = -1);

std::optional<std::vector<std::pair<int, int>>> nodes_way(const std::vector<std::vector<std::pair<int, int>>>& graph, int start, int finish);

std::string get_move(const std::pair<size_t, size_t>& start_pos, const std::pair<size_t, size_t>& finish_pos);

std::vector<std::string> action_list(const std::vector<std::pair<int, int>>& node_sequence, const T_MAP& DATA);

std::optional<std::vector<std::string>> try_fast_travel(const T_MAP& DATA, const C_PLAYER& player,
    const std::vector<std::vector<std::pair<int, int>>>& graph, const std::pair<int, int>& destination);

void fix_graph(std::vector<std::vector<std::pair<int, int>>>& graph, size_t x, size_t y, const T_MAP& DATA);

std::string get_action(const C_PLAYER& player, const T_MAP& map);

void action_smart_travel(T_MAP& map, C_PLAYER& player, std::vector<std::vector<std::pair<int, int>>>& graph);


void do_actions(const std::vector<std::string>& actions, C_PLAYER& player, T_MAP& map,
                std::vector<std::vector<std::pair<int, int>>>& graph);

bool is_action_correct(const std::string& action, const C_PLAYER& player, const T_MAP& map);

void Output(const std::string& action, C_PLAYER& player, T_MAP& map,
    const std::vector<std::vector<std::pair<int, int>>>& graph, bool is_action_ok);

std::string Player_Input();

void Run_action(const std::string& action, C_PLAYER& player, T_MAP& map,
        std::vector<std::vector<std::pair<int, int>>>& graph, bool& is_correct);

void fix_string(std::string& str);

void set_random_map(const char* filepath);

std::pair<size_t, size_t> get_closest_enemy(const C_PLAYER& player, const std::vector<std::vector<std::pair<int, int>>>& graph, const T_MAP& map);


void view_description();
#endif  //  PROJECT_INCLUDE_GAME_H_
