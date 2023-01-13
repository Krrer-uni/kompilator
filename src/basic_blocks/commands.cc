#include <iostream>
#include "basic_blocks.h"
//
// Created by krrer on 12.01.23.
//

COMMANDS_BLOCK::COMMANDS_BLOCK() {
  _type = COMMANDS_BLOCK_TYPE;
}

std::vector<std::string> COMMANDS_BLOCK::translate_block() {
  std::vector<std::string> code;
  std::vector<std::string> command_code;
  for (COMMAND_BLOCK *command : _commands) {
    command_code = command->translate_block();
    code.insert(code.end(), command_code.begin(), command_code.end());
  }
  return code;
}
void COMMANDS_BLOCK::add_command(COMMAND_BLOCK *command) {
  _commands.push_back(command);
}

COMMAND_BLOCK::COMMAND_BLOCK(basic_blocks_types::command_type command_type)
    : _commnad_type(command_type),
      _write_block(nullptr),
      _assign_block(nullptr),
      _read_block(nullptr) {

}

std::vector<std::string> COMMAND_BLOCK::translate_block() {
  std::vector<std::string> code;

  switch (_commnad_type) {
    case basic_blocks_types::CMD_WRITE:code = _write_block->translate_block();
      break;
    case basic_blocks_types::CMD_ASIGN:code = _assign_block->translate_block();
      break;
    case basic_blocks_types::CMD_READ:code = _read_block->translate_block();
      break;
    case basic_blocks_types::CMD_IF:code = _if_block->translate_block();
      break;
    default:std::cout << "jeszcze nie zaimplementowałeś tej komendy \n";
      break;
  }

  return code;
}

READ_BLOCK::READ_BLOCK(VALUE_BLOCK *input) {
  _input = input;
}

std::vector<std::string> READ_BLOCK::translate_block() {
  std::vector<std::string> code;
  code.emplace_back("GET" + _input->_var_block->get_memory_adress());
  return code;
}

ASSIGN_BLOCK::ASSIGN_BLOCK(VAR_BLOCK *lhs, EXPRESSION_BLOCK *rhs)
    : _rhs(rhs), _lhs(lhs) {
}

std::vector<std::string> ASSIGN_BLOCK::translate_block() {
  std::vector<std::string> code = _rhs->translate_block();
  code.push_back("STORE" + _lhs->get_memory_adress());
  return code;
}

WRITE_BLOCK::WRITE_BLOCK(VALUE_BLOCK *output) {
  _output = output;
}

std::vector<std::string> WRITE_BLOCK::translate_block() {
  std::vector<std::string> code;
  if (_output->_value_type == basic_blocks_types::VAL_VAR) {
    code.emplace_back("PUT" + _output->_var_block->get_memory_adress() );
  } else {
    code.push_back("SET " + std::to_string(_output->_num_block->_number_literal));
    code.emplace_back("PUT 0");
  }

  return code;
}

IF_BLOCK::IF_BLOCK(CONDITION_BLOCK *condition_block, COMMANDS_BLOCK *commands_block)
    : _commands_block(commands_block), _condition_block(condition_block) {

}
std::vector<std::string> IF_BLOCK::translate_block() {
  std::vector<std::string> code;
  code = _condition_block->translate_block();
  auto commands_code = _commands_block->translate_block();
  code.insert(code.end(),commands_code.begin(),commands_code.end());
  code.push_back(_condition_block->_tag);
  return code;
}