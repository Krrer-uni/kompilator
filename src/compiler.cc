//
// Created by krrer on 09.01.23.
//
#include <map>
#include <iostream>
#include "compiler.h"

int Compiler::COMPILER_DEBUG_MODE = 100;

Compiler::Compiler() {
  _variable_map = std::map<std::string, variable>();
  _memory_count = 1;
  _tag_count = 1;
  _output_file = nullptr;
  _curr_proc_prefix = "";
  init_operations();
}

std::string Compiler::compiler_log(std::string msg, int log_level) {
  if (COMPILER_DEBUG_MODE >= log_level) {
    return msg;
  } else {
    return "";
  }
}

void Compiler::handle_program() {
  main_code = _main->translate_block();
  for (auto proc : _proc_map) {
    if (proc.second->_is_used) {
      auto proc_code = proc.second->translate_block();
//      proc_code[0] += compiler_log(" [ start of " + proc.first.substr(1) + "] ",5);
//      proc_code[proc_code.size()-1] += compiler_log(" [ end of " + proc.first.substr(1) + "] ",5);
      main_code.insert(main_code.end(), proc_code.begin(), proc_code.end());

    }
  }
  std::vector<std::string> allocate_const_code;
  for (const auto &var : _const_map) {
    allocate_const_code.push_back(
        "SET " + std::to_string(var.first) + Compiler::compiler_log(" [allocate const] ", 2));
    allocate_const_code.push_back("STORE " + std::to_string(var.second.memory_index)
                                      + Compiler::compiler_log(" [allocate const] ", 2));
  }
  main_code.insert(main_code.begin(), allocate_const_code.begin(), allocate_const_code.end());
  translate_tags();
  if (_output_file == nullptr) {
    std::cout << "NO OUTPUT FILE FOUND \n";
    for (const auto &line : main_code) {
      std::cout << line << std::endl;
    }
  } else {
    for (const auto &line : main_code) {
      fprintf(_output_file, "%s", (line + "\n").c_str());
    }
  }

}

void Compiler::add_variable(const std::string &name, variable_type type) {
  if (_variable_map.find(name) == _variable_map.end()) {
    _variable_map[name] = variable{name, type, _memory_count};
    _memory_count++;
  } else {
    std::cout << "Redeclaration of variable " + name + "\n";
  }
}

void Compiler::handle_declaration(std::string name, variable_type type) {
  if (_proc_declaration_flag) {
    name = _curr_proc_prefix + name;
  }
  add_variable(name, type);
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "created var " + name + " with type " + std::to_string(type) + "\n";
  }
}

VALUE_BLOCK *Compiler::handle_variable(std::string &name) {
  if (_proc_declaration_flag) {
    name = _curr_proc_prefix + name;
  }
  if (_variable_map.find(name) == _variable_map.end()) {
    std::cout << "variable " + name + " was not declared in the scope \n";
    return nullptr;
  }
  auto var = _variable_map[name];
  auto var_block = new VAR_BLOCK(name, var.type, var.memory_index);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_VAR);
  value_block->_var_block = var_block;
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added block for var " + name + "\n";
  }
  return value_block;
}
VALUE_BLOCK *Compiler::handle_literal(unsigned long long int value) {
  auto num_block = new NUM_BLOCK(value);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_LIT);
  value_block->_num_block = num_block;
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added block for literal " + std::to_string(value) + "\n";
  }
  return value_block;
}
COMMAND_BLOCK *Compiler::handle_write(VALUE_BLOCK *output) {
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added WRITE block \n";
  }
  auto write_block = new WRITE_BLOCK(output);
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_WRITE);
  command_block->_write_block = write_block;
  return command_block;
}

COMMANDS_BLOCK *Compiler::handle_command(COMMANDS_BLOCK *commands_block, COMMAND_BLOCK *command) {
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added COMMAND block to COMMANDS \n";
  }
  if (commands_block == nullptr) {
    commands_block = new COMMANDS_BLOCK();
  }
  commands_block->add_command(command);
  return commands_block;
}
void Compiler::handle_main(COMMANDS_BLOCK *commands_block) {
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "created MAIN block \n";
  }

  _main = new MAIN_BLOCK(commands_block);
}

COMMAND_BLOCK *Compiler::handle_read(std::string input_name) {
  if (_proc_declaration_flag) {
    input_name = _curr_proc_prefix + input_name;
  }
  if (_variable_map.find(input_name) == _variable_map.end()) {
    std::cout << "variable " + input_name + " was not declared in the scope \n";
    return nullptr;
  }
  auto var = _variable_map[input_name];
  auto var_block = new VAR_BLOCK(input_name, var.type, var.memory_index);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_VAR);
  value_block->_var_block = var_block;
  auto read_block = new READ_BLOCK(value_block);
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_READ);
  command_block->_read_block = read_block;

  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added READ block \n";
  }
  return command_block;
}
variable *Compiler::add_const_variable(const unsigned long long int value) {
  std::string name = std::to_string(value);
  variable *new_var;
  if (_const_map.find(value) == _const_map.end()) {
    new_var = new variable{name, const_var, _memory_count};
    _const_map[value] = *new_var;
    _memory_count++;
  } else {
    std::cout << "Redeclaration of const " + std::to_string(value) + "\n";
    new_var = nullptr;
  }
  return new_var;
}

variable *Compiler::find_const(unsigned long long int value) {
  variable *map_val;
  if (_const_map.find(value) == _const_map.end()) {
    map_val = nullptr;
  } else {
    map_val = &_const_map[value];
  }
  return map_val;
}

variable *Compiler::get_const(unsigned long long int value) {
  variable *map_val;
  if (_const_map.find(value) == _const_map.end()) {
    map_val = add_const_variable(value);
  } else {
    map_val = &_const_map[value];
  }
  return map_val;
}

VALUE_BLOCK *Compiler::get_block_const(unsigned long long int value) {
  variable *map_val;
  if (_const_map.find(value) == _const_map.end()) {
    map_val = add_const_variable(value);
  } else {
    map_val = &_const_map[value];
  }
  auto var_block =
      new VAR_BLOCK("const " + std::to_string(value), map_val->type, map_val->memory_index);
  auto value_block = new VALUE_BLOCK(basic_blocks_types::VAL_VAR);
  value_block->_var_block = var_block;
  return value_block;
}

EXPRESSION_BLOCK *Compiler::handle_expression(basic_blocks_types::expression_type expression_type,
                                              VALUE_BLOCK *lhs,
                                              VALUE_BLOCK *rhs) {
  auto block = new EXPRESSION_BLOCK(expression_type, lhs, rhs, this);
  switch (expression_type) {
    case basic_blocks_types::EXP_MUL:
    block->_mult_proc = _mult_proc;
    _proc_map["&$MULT"]->_is_used = true;
      break;
    case basic_blocks_types::EXP_MOD:
      block->_mod_proc = _mod_proc;
      break;
    case basic_blocks_types::EXP_DIV:
      block->_div_proc = _div_proc;
      break;
    default:
      break;
  }
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added EXPRESSION block \n";
  }
  return block;
}
COMMAND_BLOCK *Compiler::handle_assign(std::string identifier, EXPRESSION_BLOCK *expression_block) {
  if (_proc_declaration_flag) {
    identifier = _curr_proc_prefix + identifier;
  }
  if (_variable_map.find(identifier) == _variable_map.end()) {
    std::cout << "variable " + identifier + " was not declared in the scope \n";
    return nullptr;
  }
  auto var = _variable_map[identifier];
  auto var_block = new VAR_BLOCK(identifier, var.type, var.memory_index);
  auto cmd_block = new COMMAND_BLOCK(basic_blocks_types::CMD_ASIGN);
  cmd_block->_assign_block = new ASSIGN_BLOCK(var_block, expression_block);
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added ASSIGN block \n";
  }
  return cmd_block;
}

void Compiler::set_output(_IO_FILE *output_file) {
  _output_file = output_file;
}
void Compiler::set_log_level(int log_level) {
  COMPILER_DEBUG_MODE = log_level;
}
CONDITION_BLOCK *Compiler::handle_condition(basic_blocks_types::condition_type condition_type,
                                            VALUE_BLOCK *lhs,
                                            VALUE_BLOCK *rhs) {
  std::string tag = "&" + std::to_string(_tag_count++);
  auto block = new CONDITION_BLOCK(condition_type, lhs, rhs, tag, this);
  if (COMPILER_DEBUG_MODE > 0) {
    std::cout << "added CONDITION block \n";
  }
  return block;
}

void Compiler::translate_tags() {
  int i = 0;
  for (auto line = main_code.begin(); line != main_code.end(); line++, i++) {
    std::string tag_msg;
    while ((*line)[0] == '&') {
      _tag_map[*line] = i;
      std::string tag = (*line).substr(1);
      tag_msg += " [ ^" + tag + " ] ";
      line = main_code.erase(line);
    }
    line->append(compiler_log(tag_msg, 4));
  }
  for (auto line = main_code.begin(); line != main_code.end(); line++, i++) {
    auto index_start = (*line).find("&");
    if (index_start != std::string::npos) {
      auto line_from_tag = (*line).substr(index_start);
      auto tag = line_from_tag.substr(0, line_from_tag.find(" "));
      auto tag_line = _tag_map[tag];
      line->erase(index_start);
      line->append(std::to_string(tag_line) + compiler_log(" [ JUMP ^" + tag.substr(1) + " ] ", 4));
    }
  }
}

COMMAND_BLOCK *Compiler::handle_if(CONDITION_BLOCK *condition_block,
                                   COMMANDS_BLOCK *commands_block) {
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_IF);
  command_block->_if_block = new IF_BLOCK(condition_block, commands_block);

  std::cout << compiler_log("added IF block", 1) + "\n";
  return command_block;
}

COMMAND_BLOCK *Compiler::handle_if_else(CONDITION_BLOCK *condition_block,
                                        COMMANDS_BLOCK *if_commands_block,
                                        COMMANDS_BLOCK *else_commands_block) {
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_IFELS);
  command_block->_if_else_block =
      new IF_ELSE_BLOCK(condition_block, if_commands_block, else_commands_block);

  std::cout << compiler_log("added IF ELSE block", 1) + "\n";
  return command_block;
}
COMMAND_BLOCK *Compiler::handle_while(CONDITION_BLOCK *condition_block,
                                      COMMANDS_BLOCK *commands_block) {
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_WHILE);
  command_block->_while_block = new WHILE_BLOCK(condition_block, commands_block);
  std::cout << compiler_log("added WHILE block", 1) + "\n";
  return command_block;

}
COMMAND_BLOCK *Compiler::handle_repeat(CONDITION_BLOCK *condition_block,
                                       COMMANDS_BLOCK *commands_block) {
  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_REPEAT);
  command_block->_repeat_block = new REPEAT_BLOCK(condition_block, commands_block);

  std::cout << compiler_log("added REPEAT block", 1) + "\n";
  return command_block;
}
std::vector<std::string> *Compiler::handle_proc_declaration(std::string name,
                                                            std::vector<std::string> *declarations) {
  if (declarations == nullptr) {
    declarations = new std::vector<std::string>;
  }

  declarations->push_back(name);
  return declarations;
}
std::vector<variable> *Compiler::handle_proc_head(std::string proc_name,
                                                  std::vector<std::string> *proc_declarations) {
  auto *declared_vars = new std::vector<variable>;
  auto number_of_variables = proc_declarations->size();
  std::string var_prefix = proc_name + std::to_string(number_of_variables) + "$";
  for (auto &var_name : *proc_declarations) {
    var_name = var_prefix + var_name;
    if (_variable_map.find(var_name) != _variable_map.end()) {
      std::cout << "Redeclaration of variable " + var_name + " in procedure\n";
    }
    auto new_var = variable(var_name, pointer_var, _memory_count++);
    _variable_map[var_name] = new_var;
    declared_vars->push_back(new_var);
  }
  _curr_proc_prefix = var_prefix;
  _proc_declaration_flag = true;
  return declared_vars;
}
void Compiler::handle_procedure_definition(COMMANDS_BLOCK *commands_block,
                                           std::vector<variable> *proc_head_vars) {
  std::string return_address_name = _curr_proc_prefix + "$return";
  add_variable(return_address_name, pointer_var);
  auto return_addres = _variable_map[return_address_name];
  uint8_t number_of_vars = proc_head_vars->size();
  PROCEDURE_BLOCK* procedure =
      new PROCEDURE_BLOCK(number_of_vars, proc_head_vars, return_addres.memory_index, commands_block,
                      false, _curr_proc_prefix);
  _proc_map[_curr_proc_prefix] = procedure;
  _proc_declaration_flag = false;
}

COMMAND_BLOCK *Compiler::handle_proc_use(std::string proc_name,
                                         std::vector<std::string> *proc_declarations) {
  std::string proc_id = proc_name + std::to_string(proc_declarations->size()) + "$";
  PROCEDURE_BLOCK* procedure;
  if (_proc_map.find(proc_id) == _proc_map.end()) {
    std::cout << "INCORRECT PROCEDURE CALL: there is no procedure " + proc_name + " with "
        + std::to_string(proc_declarations->size()) + " arguments\n";
    return nullptr;
  } else{
    procedure = _proc_map[proc_id];
    _proc_map[proc_id]->_is_used = true;
  }
  std::vector<variable>* call_variables = new std::vector<variable>;
  for(auto var_name : *proc_declarations){
    if (_proc_declaration_flag) {
      var_name = _curr_proc_prefix + var_name;
    }
    if(_variable_map.find(var_name) == _variable_map.end()){
      std::cout << "In function call " + proc_name + " with "
          + std::to_string(proc_declarations->size()) + "arguments. Use of undeclared variable " + var_name + ".\n";
      return nullptr;
    } else{
      call_variables->push_back(_variable_map[var_name]);
    }
  }

  auto command_block = new COMMAND_BLOCK(basic_blocks_types::CMD_PROC);
  command_block->_procedure_call_block = new PROCEDURE_CALL_BLOCK(call_variables, proc_id, procedure);

  std::cout << compiler_log("added PROCEDURE CALL block", 1) + "\n";
  return command_block;
}
void Compiler::init_operations() {
  std::string lhs_adr = std::to_string(_memory_count++);
  std::string rhs_adr = std::to_string(_memory_count++);
  uint16_t ret_adr = _memory_count++;
  std::string acc_adr = std::to_string(_memory_count++);
  auto one_const = add_const_variable(1);
  auto* mul_proc = new MULT_PROC(lhs_adr, rhs_adr, acc_adr, std::to_string(one_const->memory_index), ret_adr);
  _mult_proc = mul_proc;
  _proc_map["&$MULT"] = mul_proc;
}
