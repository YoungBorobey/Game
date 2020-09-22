#ifndef PROJECT_INCLUDE_ITEM_H_
#define PROJECT_INCLUDE_ITEM_H_

#include <string>

class C_ITEM {
 private:
  std::string name;
  int weight;

 public:
  C_ITEM(const std::string& name, int weight);
  void set_weight(int new_weight);
  std::string get_name() const;
};

class C_EQUIPMENT : public C_ITEM {
 private:
  int armor;

 public:
  explicit C_EQUIPMENT(const std::string& str);

  int get_armor() const;
};
#endif  // PROJECT_INCLUDE_ITEM_H_
