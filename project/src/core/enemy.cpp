#include "game.h"

C_ENEMY::C_ENEMY(int hp, int dmg, const std::string& name, bool is_dead, int base_dmg,
                 int armor, int max_armor)
    : C_ENTITY(hp, dmg, is_dead, base_dmg, armor),
      enemy_name(name),
      max_armor(max_armor) {}

// create enemy by it's name
C_ENEMY::C_ENEMY(const std::string& name) : enemy_name(name), max_armor(20) {
  int curr_dmg = 0, curr_hp = 0, curr_bd = 0, curr_armor = 0;

  if (name == "wolf") {
    curr_hp = 6;
    curr_dmg = 10;
  }
  if (name == "dog") {
    curr_hp = 3;
    curr_dmg = 5;
  }
  if (name == "rat") {
    curr_hp = 2;
    curr_dmg = 3;
  }

  dmg = curr_dmg;
  hp = curr_hp;
  dead = false;
  base_dmg = curr_bd;
  armor = curr_armor;
}

std::string C_ENEMY::get_name() const { return this->enemy_name; }

int C_ENEMY::get_armor() const { return std::min(max_armor, armor); }
