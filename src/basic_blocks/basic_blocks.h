//
// Created by krrer on 09.01.23.
//

#ifndef KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define KOMPILATOR_SRC_BASIC_BLOCKS_H_
#define literal unsigned long long

#define MAIN_BLOCK_TYPE 10

#define COMMANDS_BLOCK_TYPE 50

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "../variables.h"

class MULT_PROC;
class div_proc;
class mod_proc;
class Compiler;
class PROCEDURE_CALL_BLOCK;
namespace basic_blocks_types {
enum expression_type { EXP_VAL, EXP_ADD, EXP_SUB, EXP_MUL, EXP_DIV, EXP_MOD };
enum command_type {
  CMD_WRITE,
  CMD_ASIGN,
  CMD_READ,
  CMD_IF,
  CMD_IFELS,
  CMD_WHILE,
  CMD_REPEAT,
  CMD_PROC
};
enum condition_type { CON_EQ, CON_NEQ, CON_GR, CON_GRE, CON_LS, CON_LSE };
enum value_type { VAL_VAR, VAL_LIT };
}

class GENERIC_BLOCK {
 protected:
  int _type = 0;
 public:
  virtual int get_type();
  virtual std::vector<std::string> translate_block() = 0;
};

/**
 * VALUE
 */
class NUM_BLOCK {
 public:
  explicit NUM_BLOCK(literal value);
  std::vector<std::string> translate_block();
  literal _number_literal;
 private:

};

class VAR_BLOCK {
 public:
  VAR_BLOCK(std::string name, variable_type variable_type, unsigned int memory_index);
  std::string get_memory_adress();
  std::string _var_name;
  uint64_t _memory_block{};
  variable_type _var_type;
};

class VALUE_BLOCK {
 public:
  VALUE_BLOCK(basic_blocks_types::value_type value_type);
  std::string to_acc();
  basic_blocks_types::value_type _value_type;
  VAR_BLOCK *_var_block;
  NUM_BLOCK *_num_block;
};

/**
 * EXPRESSIONS
 */
class EXPRESSION_BLOCK : public GENERIC_BLOCK {
 public:

  EXPRESSION_BLOCK(basic_blocks_types::expression_type type,
                   VALUE_BLOCK *lhs,
                   VALUE_BLOCK *rhs,
                   Compiler *compiler);
  Compiler *_compiler;
  std::vector<std::string> translate_block() override;
  basic_blocks_types::expression_type _type;
  VALUE_BLOCK *_lhs;
  VALUE_BLOCK *_rhs;
  MULT_PROC* _mult_proc;
  div_proc* _div_proc;
  mod_proc* _mod_proc;
};

/**
 * CONDITION BLOCKS
 */
class CONDITION_BLOCK : public GENERIC_BLOCK {
 public:
  CONDITION_BLOCK(basic_blocks_types::condition_type type,
                  VALUE_BLOCK *lhs,
                  VALUE_BLOCK *rhs,
                  std::string tag,
                  Compiler *compiler);
  Compiler *_compiler;
  std::vector<std::string> translate_block();
  std::string _tag;
  VALUE_BLOCK *_lhs;
  VALUE_BLOCK *_rhs;
  basic_blocks_types::condition_type _condition_type;
 private:
  std::vector<std::string> translate_eq();
  std::vector<std::string> translate_neq();
  std::vector<std::string> translate_gr();
  std::vector<std::string> translate_gre();
};

/**
 * COMMANDS
 */
class COMMANDS_BLOCK;

class IF_BLOCK {
 public:
  IF_BLOCK(CONDITION_BLOCK *condition_block, COMMANDS_BLOCK *commands_block);
  CONDITION_BLOCK *_condition_block;
  COMMANDS_BLOCK *_commands_block;
  std::vector<std::string> translate_block();
};

class IF_ELSE_BLOCK {
 public:
  IF_ELSE_BLOCK(CONDITION_BLOCK *condition_block,
                COMMANDS_BLOCK *if_commands_block,
                COMMANDS_BLOCK *else_commands_block);
  CONDITION_BLOCK *_condition_block;
  COMMANDS_BLOCK *_if_commands_block;
  COMMANDS_BLOCK *_else_commands_block;
  std::vector<std::string> translate_block();
};

class WRITE_BLOCK {
 public:
  explicit WRITE_BLOCK(VALUE_BLOCK *output);
  std::vector<std::string> translate_block();
 private:
  VALUE_BLOCK *_output;
};

class WHILE_BLOCK {
 public:
  WHILE_BLOCK(CONDITION_BLOCK *condition_block, COMMANDS_BLOCK *commands_block);
  CONDITION_BLOCK *_condition_block;
  COMMANDS_BLOCK *_commands_block;
  std::vector<std::string> translate_block();
};

class REPEAT_BLOCK {
 public:
  REPEAT_BLOCK(CONDITION_BLOCK *condition_block, COMMANDS_BLOCK *commands_block);
  CONDITION_BLOCK *_condition_block;
  COMMANDS_BLOCK *_commands_block;
  std::vector<std::string> translate_block();
};

class READ_BLOCK {
 public:
  explicit READ_BLOCK(VALUE_BLOCK *input);
  std::vector<std::string> translate_block();
 private:
  VALUE_BLOCK *_input;
};

class ASSIGN_BLOCK {
 public:
  ASSIGN_BLOCK(VAR_BLOCK *lhs, EXPRESSION_BLOCK *rhs);
  std::vector<std::string> translate_block();
 private:
  VAR_BLOCK *_lhs;
  EXPRESSION_BLOCK *_rhs;
};

class COMMAND_BLOCK {
 public:
  COMMAND_BLOCK(basic_blocks_types::command_type command_type);
  std::vector<std::string> translate_block();
  basic_blocks_types::command_type _commnad_type;
  WRITE_BLOCK *_write_block;
  ASSIGN_BLOCK *_assign_block;
  READ_BLOCK *_read_block;
  IF_BLOCK *_if_block;
  IF_ELSE_BLOCK *_if_else_block;
  WHILE_BLOCK *_while_block;
  REPEAT_BLOCK *_repeat_block;
  PROCEDURE_CALL_BLOCK *_procedure_call_block;
};

class COMMANDS_BLOCK : public GENERIC_BLOCK {
 public:
  COMMANDS_BLOCK();
  std::vector<std::string> translate_block() override;
  void add_command(COMMAND_BLOCK *block);
 private:
  std::vector<COMMAND_BLOCK *> _commands;
};

/**
 * PROCEDURE
 */
class PROCEDURE_BLOCK : public GENERIC_BLOCK {

 public:
  PROCEDURE_BLOCK();
  PROCEDURE_BLOCK(uint16_t number_of_vars,
                  std::vector<variable>* &variables,
                  uint16_t return_adress,
                  COMMANDS_BLOCK *commands_block,
                  bool is_used, std::string proc_name) : _number_of_vars(number_of_vars),
                                                         _variables(variables),
                                                         _return_adress(return_adress),
                                                         _commands_block(commands_block),
                                                         _is_used(is_used),
                                                         _proc_name(std::move(proc_name)) {}
  std::vector<std::string> translate_block() override;
  bool _is_used{};
  std::vector<variable>* _variables{};
  uint16_t _return_adress{};
  uint64_t _no_uses{};
 private:
  std::string _proc_name{};
  uint16_t _number_of_vars{};
  COMMANDS_BLOCK *_commands_block{};
};

class PROCEDURE_CALL_BLOCK : public GENERIC_BLOCK {
 public:
  std::vector<std::string> translate_block() override;
  PROCEDURE_CALL_BLOCK(std::vector<variable> *variables,
                       std::string proc_name,
                       PROCEDURE_BLOCK *_procedure_block);
  std::vector<variable> *_variables{};
  std::string _proc_name{};
  PROCEDURE_BLOCK *_procedure_block;
  uint64_t _return_address{};

};



/**
 * MAIN BLOCK
 */
class MAIN_BLOCK : GENERIC_BLOCK {
 public:
  explicit MAIN_BLOCK(COMMANDS_BLOCK *commands_block);
  std::vector<std::string> translate_block() override;
 private:
  COMMANDS_BLOCK *_commands_block;
};

#endif //KOMPILATOR_SRC_BASIC_BLOCKS_H_
