//
// Created by krrer on 12.01.23.
//

#include "basic_blocks.h"
#include "../compiler.h"
CONDITION_BLOCK::CONDITION_BLOCK(basic_blocks_types::condition_type type,
                                 VALUE_BLOCK *lhs,
                                 VALUE_BLOCK *rhs,
                                 std::string tag, Compiler *compiler) :
    _lhs(lhs), _rhs(rhs), _tag(std::move(tag)), _condition_type(type), _compiler(compiler) {
}

std::vector<std::string> CONDITION_BLOCK::translate_block() {
  std::vector<std::string> code;
  switch (_condition_type) {
    case basic_blocks_types::CON_EQ:code = translate_eq();
      break;
    case basic_blocks_types::CON_GR:code = translate_gr();
      break;
    case basic_blocks_types::CON_GRE:code = translate_gre();
      break;
    case basic_blocks_types::CON_NEQ:code = translate_neq();
      break;
    case basic_blocks_types::CON_LS:std::swap(_lhs, _rhs);
      code = translate_gr();
      break;
    case basic_blocks_types::CON_LSE:std::swap(_lhs, _rhs);
      code = translate_gre();
      break;
  }
  return code;
}

std::vector<std::string> CONDITION_BLOCK::translate_gr() {
  std::vector<std::string> code;
  if (_rhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto rhs_val = _rhs->_num_block->_number_literal;
    auto new_const = _compiler->get_const(rhs_val);
    code.emplace_back(_lhs->to_acc());
    code.emplace_back("SUB " + std::to_string(new_const->memory_index));
  } else {
    code.emplace_back(_lhs->to_acc());
    code.emplace_back("SUB" + _rhs->_var_block->get_memory_adress());
  }
  code.emplace_back("JZERO " + _tag);
  return code;
}

std::vector<std::string> CONDITION_BLOCK::translate_gre() {
  std::vector<std::string> code;
  if (_lhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto lhs_val = _lhs->_num_block->_number_literal;
    auto new_const = _compiler->get_const(lhs_val);
    code.emplace_back(_rhs->to_acc());
    code.emplace_back("SUB" + std::to_string(new_const->memory_index));
  } else {
    code.emplace_back(_rhs->to_acc());
    code.emplace_back("SUB" + _lhs->_var_block->get_memory_adress());
  }
  code.emplace_back("JPOS " + _tag);
  return code;
}

std::vector<std::string> CONDITION_BLOCK::translate_eq() {
  std::vector<std::string> code;
  if (_lhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto lhs_val = _lhs->_num_block->_number_literal;
    _lhs = _compiler->get_block_const(lhs_val);
  }
  if (_rhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto rhs_val = _rhs->_num_block->_number_literal;
    _rhs = _compiler->get_block_const(rhs_val);
  }
  code.emplace_back(_lhs->to_acc());
  code.emplace_back("SUB" + _rhs->_var_block->get_memory_adress());
  code.emplace_back("JPOS " + _tag);
  code.emplace_back(_rhs->to_acc());
  code.emplace_back("SUB" + _lhs->_var_block->get_memory_adress());
  code.emplace_back("JPOS " + _tag);

  return code;
}
std::vector<std::string> CONDITION_BLOCK::translate_neq() {
  std::vector<std::string> code;
  if (_lhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto lhs_val = _lhs->_num_block->_number_literal;
    _lhs = _compiler->get_block_const(lhs_val);
  }
  if (_rhs->_value_type == basic_blocks_types::VAL_LIT) {
    auto rhs_val = _rhs->_num_block->_number_literal;
    _rhs = _compiler->get_block_const(rhs_val);
  }
  code.emplace_back(_lhs->to_acc());
  code.emplace_back("SUB" + _rhs->_var_block->get_memory_adress());
  code.emplace_back("JZERO " + _tag + "_1");
  code.emplace_back("JUMP " + _tag + "_2");
  code.emplace_back(_tag + "_1");
  code.emplace_back(_rhs->to_acc());
  code.emplace_back("SUB" + _lhs->_var_block->get_memory_adress());
  code.emplace_back("JZERO " + _tag);
  code.emplace_back(_tag + "_2");

  return code;
}
