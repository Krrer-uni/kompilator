//
// Created by krrer on 09.01.23.
//

#ifndef KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define literal unsigned long long

#define PROGRAM_BLOCK_TYPE 10

#define NUM_BLOCK_TYPE 40
#define VARIABLE_BLOCK_TYPE 41

#define ADDITION_BLOCK_TYPE 50

#define ASSIGN_BLOCK_TYPE 50
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "variables.h"
class GENERIC_BLOCK {
 protected:
  int _type = 0;
 public:
  virtual int get_type();
  virtual std::vector<std::string> translate_block() = 0;
};

/**
 * EXPRESSIONS
 */
class EXPRESSION_BLOCK : public GENERIC_BLOCK {
};

class VALUE_BLOCK : public EXPRESSION_BLOCK {
 public:
  virtual std::string get_content() = 0;
};

class NUM_BLOCK : public VALUE_BLOCK {
 public:
  explicit NUM_BLOCK(literal value);
  std::string get_content() override;
  std::vector<std::string> translate_block() override;
 private:
  literal _number_literal;

};

class VAR_BLOCK : public VALUE_BLOCK {
 public:
  VAR_BLOCK(std::string name, variable_type variable_type);
  std::string get_content() override;
  std::vector<std::string> translate_block() override;
 private:
  std::string _var_name;
  uint64_t _memory_block{};
  variable_type _var_type;
};

class ADDITION_BLOCK : public EXPRESSION_BLOCK {
 public:
  ADDITION_BLOCK(VALUE_BLOCK *lhs, VALUE_BLOCK *rhs);
  std::vector<std::string> translate_block() override;
 private:
  VALUE_BLOCK *_lhs;
  VALUE_BLOCK *_rhs;
};

/**
 * COMMANDS
 */
class COMMAND_BLOCK : public GENERIC_BLOCK {
};



class ASSIGN_BLOCK : public COMMAND_BLOCK {
 public:
  ASSIGN_BLOCK(std::shared_ptr<VAR_BLOCK> lhs, std::shared_ptr<EXPRESSION_BLOCK> rhs);
  std::vector<std::string> translate_block() override;
 private:
  std::shared_ptr<VAR_BLOCK> _lhs;
  std::shared_ptr<EXPRESSION_BLOCK> _rhs;
};

class VARIABLE_DECLARATION_BLOCK : public GENERIC_BLOCK {
  std::vector<VAR_BLOCK *> _vars;
};

class COMMANDS_BLOCK : public GENERIC_BLOCK {
  std::vector<COMMAND_BLOCK *> _commands;
};

class PROGRAM_BLOCK : GENERIC_BLOCK {
 public:
  PROGRAM_BLOCK(VARIABLE_DECLARATION_BLOCK *declaration_block, COMMANDS_BLOCK *_commands_block);
 private:
  VARIABLE_DECLARATION_BLOCK *_declaration_block;
  COMMANDS_BLOCK *_commands_block;
  std::map<std::string, variable> _variable_list;
};

#endif //KOMPILATOR_SRC_BASIC_BLOCKS_H_
