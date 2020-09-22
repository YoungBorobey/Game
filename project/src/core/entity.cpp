#include <algorithm>

#include "game.h"
C_ENTITY::C_ENTITY(int hp, int dmg, bool is_dead, int base_dmg, int armor)
    : hp(hp),
      dmg(dmg + base_dmg),
      base_dmg(base_dmg),
      armor(armor),
      dead(is_dead) {}

C_ENTITY::C_ENTITY() : hp(0), dmg(0), base_dmg(0), armor(0), dead(true) {}

int C_ENTITY::get_hp() const { return this->hp; }

int C_ENTITY::get_dmg() const { return this->dmg; }

int C_ENTITY::get_armor() const { return this->armor; }

bool C_ENTITY::is_dead() const { return this->dead; }

void C_ENTITY::hit(C_ENTITY& someone) {
  if (!someone.dead) {
    someone.decrease_hp(
        std::max((this->dmg + this->base_dmg - someone.get_armor()), 1));
    if (someone.hp == 0) {
      someone.dead = true;
    }
  }
}

void C_ENTITY::decrease_hp(int dmg) {
  this->hp = std::max(0, (this->hp) - dmg);
  if (hp == 0) {
    dead = true;
  }
}
