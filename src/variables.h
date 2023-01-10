//
// Created by krrer on 09.01.23.
//
#ifndef KOMPILATOR_SRC_VARIABLES_H_
#define KOMPILATOR_SRC_VARIABLES_H_

#include <string>

enum variable_type{body, header};
class variable{


 private:
  std::string name;
  variable_type type;
  uint64_t memory_index;
  int usage_number = 0;
};

#endif //KOMPILATOR_SRC_VARIABLES_H_
