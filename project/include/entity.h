#ifndef PROJECT_INCLUDE_ENTITY_H_
#define PROJECT_INCLUDE_ENTITY_H_

#include <map>
#include <string>

#include "item.h"
class C_ENTITY {
 protected:
  int hp, dmg, base_dmg, armor;
  bool dead;

 public:
  C_ENTITY();

  C_ENTITY(int hp, int dmg, bool is_dead, int base_dmg = 1, int armor = 0);

  int get_hp() const;

  int get_dmg() const;

  virtual int get_armor() const;

  bool is_dead() const;

  void hit(C_ENTITY& someone);

  void decrease_hp(int dmg);
};

class C_ENEMY : public C_ENTITY {
 private:
  std::string enemy_name;
  int max_armor;

 public:
  C_ENEMY(int hp = 0, int dmg = 0, const std::string& name = "unknown", bool is_dead = true,
          int base_dmg = 1, int armor = 0, int max_armor = 20);

  explicit C_ENEMY(const std::string& name);

  std::string get_name() const;

  int get_armor() const override;
};

class C_PLAYER : public C_ENTITY {
 private:
  std::map<std::string, C_EQUIPMENT> Inventory;
  size_t x, y;
  int max_armor;
  bool battle_check;

 public:
  explicit C_PLAYER(int hp = 100, int dmg = 0, int armor = 0, int max_armor = 20);

  //  increase coordinates, depending on direction
  int step(const std::string& direction);

  void pick_item(C_EQUIPMENT& item);

  void throw_item(const std::string& item);

  //  inventory contains return TRUE if contains item, either FALSE
  bool inv_cont(const std::string& item) const;

  size_t get_x() const;

  size_t get_y() const;

  int get_armor() const override;

  const std::map<std::string, C_EQUIPMENT>& get_inventory() const;

  bool& battle_status();
  const bool& battle_status() const;
};

#endif  // PROJECT_INCLUDE_ENTITY_H_
