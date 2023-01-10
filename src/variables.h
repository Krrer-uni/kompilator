//
// Created by krrer on 09.01.23.
//
#ifndef KOMPILATOR_SRC_VARIABLES_H_
#define KOMPILATOR_SRC_VARIABLES_H_

#include <string>
#include <utility>

enum variable_type { value_var, pointer_var };
class variable {
 public:
  variable();
  variable(std::string name, variable_type type) : name(std::move(name)), type(type), usage_number(0),
                                                   memory_index(0) {};
  variable_type get_type();
 private:
  std::string name;
  variable_type type;
  uint64_t memory_index;
  int usage_number = 0;
};

#endif //KOMPILATOR_SRC_VARIABLES_H_
