//
// Created by krrer on 09.01.23.
//

#ifndef KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define literal unsigned long long

#define MAIN_BLOCK_TYPE 10

#define VALUE_BLOCK_TYPE 40
#define NUM_BLOCK_TYPE 41
#define VARIABLE_BLOCK_TYPE 42

#define COMMANDS_BLOCK_TYPE 50
#define WRITE_BLOCK_TYPE 51
#define ADDITION_BLOCK_TYPE 52

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



class NUM_BLOCK{
 public:
  explicit NUM_BLOCK(literal value);
  std::vector<std::string> translate_block();
  literal _number_literal;
 private:

};

class VAR_BLOCK{
 public:
  VAR_BLOCK(std::string name, variable_type variable_type, unsigned int memory_index);
  std::vector<std::string> translate_block();
  std::string _var_name;
  uint64_t _memory_block{};
  variable_type _var_type;
};

class VALUE_BLOCK{
 public:
  enum value_type{var, number};
  VALUE_BLOCK(value_type value_type);
  std::vector<std::string> translate_block();
  value_type _value_type;
  VAR_BLOCK* _var_block;
  NUM_BLOCK* _num_block;
};

//class ADDITION_BLOCK : public EXPRESSION_BLOCK {
// public:
//  ADDITION_BLOCK(VALUE_BLOCK *lhs, VALUE_BLOCK *rhs);
//  std::vector<std::string> translate_block() override;
// private:
//  VALUE_BLOCK *_lhs;
//  VALUE_BLOCK *_rhs;
//};

/**
 * COMMANDS
 */


class WRITE_BLOCK{
 public:
  explicit WRITE_BLOCK(VALUE_BLOCK* output);
  std::vector<std::string> translate_block();
 private:
  VALUE_BLOCK* _output;
};

class READ_BLOCK{
 public:
  explicit READ_BLOCK(VALUE_BLOCK* input);
  std::vector<std::string> translate_block();
 private:
  VALUE_BLOCK* _input;
};

class ASSIGN_BLOCK{
 public:
  ASSIGN_BLOCK(VAR_BLOCK* lhs, EXPRESSION_BLOCK* rhs);
  std::vector<std::string> translate_block();
 private:
  VAR_BLOCK* _lhs;
  EXPRESSION_BLOCK* _rhs;
};

class COMMAND_BLOCK{
 public:
  enum command_type{write,assign,read,iff,iffels,whilee,proc};
  COMMAND_BLOCK(command_type command_type);
  std::vector<std::string> translate_block();
  command_type _commnad_type;
  WRITE_BLOCK* _write_block;
  ASSIGN_BLOCK* _assign_block;
  READ_BLOCK* _read_block;
};


class VARIABLE_DECLARATION_BLOCK : public GENERIC_BLOCK {
  std::vector<VAR_BLOCK *> _vars;
};

class COMMANDS_BLOCK : public GENERIC_BLOCK {
 public:
  COMMANDS_BLOCK();
  std::vector<std::string> translate_block() override;
  void add_command(COMMAND_BLOCK* block);
 private:
  std::vector<COMMAND_BLOCK*> _commands;
};

class MAIN_BLOCK : GENERIC_BLOCK {
 public:
  MAIN_BLOCK(COMMANDS_BLOCK *commands_block);
  std::vector<std::string> translate_block() override;
 private:
  COMMANDS_BLOCK *_commands_block;
};

#endif //KOMPILATOR_SRC_BASIC_BLOCKS_H_
