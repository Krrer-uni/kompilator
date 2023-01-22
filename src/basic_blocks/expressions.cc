//
// Created by krrer on 12.01.23.
//

#include <iostream>
#include <utility>
#include "basic_blocks.h"
#include "../compiler.h"

EXPRESSION_BLOCK::EXPRESSION_BLOCK(basic_blocks_types::expression_type type,
                                   VALUE_BLOCK *lhs,
                                   VALUE_BLOCK *rhs,
                                   Compiler *compiler) :
    _type(type),
    _lhs(lhs),
    _rhs(rhs),
    _compiler(compiler),
    _div_proc(nullptr),
    _mod_proc(nullptr),
    _mult_proc(
        nullptr) {
}

std::vector<std::string> EXPRESSION_BLOCK::translate_block() {
  std::vector<std::string> code;
  switch (_type) {
    case basic_blocks_types::EXP_ADD: /* ADDITION */
      if (_lhs->_value_type == basic_blocks_types::VAL_VAR) {
        if (_rhs->_value_type == basic_blocks_types::VAL_VAR) {  // both vars
          code.push_back("LOAD" + _rhs->_var_block->get_memory_adress());
          code.push_back("ADD" + _lhs->_var_block->get_memory_adress());
        } else {  // lhs var rhs literal
          code.push_back("SET " + std::to_string(_rhs->_num_block->_number_literal));
          code.push_back("ADD" + _lhs->_var_block->get_memory_adress());
        }
      } else {
        if (_rhs->_value_type == basic_blocks_types::VAL_VAR) {   // lhs literal rhs var
          code.push_back("SET " + std::to_string(_lhs->_num_block->_number_literal));
          code.push_back("ADD" + _rhs->_var_block->get_memory_adress());
        } else {   // both literals
          auto lhs_val = _lhs->_num_block->_number_literal;
          auto rhs_val = _rhs->_num_block->_number_literal;
          if (_compiler->find_const(rhs_val) == nullptr) {
            if (_compiler->find_const(lhs_val) == nullptr) {
              auto new_const = _compiler->add_const_variable(rhs_val);
              code.push_back("SET " + std::to_string(lhs_val));
              code.push_back("ADD " + std::to_string(new_const->memory_index));
            } else {
              auto rhs_var = _compiler->find_const(rhs_val);
              code.push_back("SET " + std::to_string(lhs_val));
              code.push_back("ADD " + std::to_string(rhs_var->memory_index));
            }
          } else {
            auto lhs_var = _compiler->find_const(lhs_val);
            code.push_back("SET " + std::to_string(rhs_val));
            code.push_back("ADD " + std::to_string(lhs_var->memory_index));
          }
        }
      }
      code[0] = code[0] + " [ ADDITION ] ";
      break;
    case basic_blocks_types::EXP_VAL:
      if (_lhs->_value_type == basic_blocks_types::VAL_LIT) {
        code.push_back("SET " + std::to_string(_lhs->_num_block->_number_literal));
      } else {
        code.push_back("LOAD" + _lhs->_var_block->get_memory_adress());
      }
      break;
    case basic_blocks_types::EXP_SUB:  /* SUBTRACTION */
      if (_lhs->_value_type == basic_blocks_types::VAL_VAR) {
        if (_rhs->_value_type == basic_blocks_types::VAL_VAR) {  // both vars
          code.push_back("LOAD" + _lhs->_var_block->get_memory_adress());
          code.push_back("SUB" + _rhs->_var_block->get_memory_adress());
        } else {  // lhs var rhs literal
          auto rhs_val = _rhs->_num_block->_number_literal;
          variable *literal_var = _compiler->find_const(rhs_val);
          if (literal_var == nullptr) {
            literal_var = _compiler->add_const_variable(rhs_val);
          }
          code.push_back("LOAD" + _lhs->_var_block->get_memory_adress());
          code.push_back("SUB " + std::to_string(literal_var->memory_index));
        }
      } else {
        if (_rhs->_value_type == basic_blocks_types::VAL_VAR) {   // lhs literal rhs var
          code.push_back("SET " + std::to_string(_lhs->_num_block->_number_literal));
          code.push_back("SUB" + _rhs->_var_block->get_memory_adress());
        } else {   // both literals
          if (_lhs->_num_block->_number_literal <= _rhs->_num_block->_number_literal) {
            code.emplace_back("SET 0");
          } else {
            code.emplace_back("SET " + std::to_string(
                _lhs->_num_block->_number_literal - _rhs->_num_block->_number_literal));
          }
        }
      }
      code[0] = code[0] + Compiler::compiler_log(" [ SUBTRACTION ] ", 2);
      break;
    case basic_blocks_types::EXP_MUL: {
      if (_lhs->_value_type == basic_blocks_types::VAL_LIT
          && _rhs->_value_type == basic_blocks_types::VAL_LIT
          && _lhs->_num_block->_number_literal < _rhs->_num_block->_number_literal) {
        std::swap(_lhs, _rhs);
      }
      code.push_back(_lhs->to_acc());
      code.emplace_back("STORE " + _mult_proc->lhs);
      code.push_back(_rhs->to_acc());
      code.emplace_back("STORE " + _mult_proc->rhs);
      code.emplace_back("SET &$MULT" + _mult_proc->get_no_uses() + "return");
      code.emplace_back("STORE " + _mult_proc->get_ret_adr());
      code.emplace_back("JUMP &$MULT");
      code.emplace_back("&$MULT" + _mult_proc->get_no_uses()  + "return");
      _mult_proc->inc_no_uses();
      break;
    }

    case basic_blocks_types::EXP_DIV:
      code.push_back(_lhs->to_acc());
      code.emplace_back("STORE " + _div_proc->lhs);
      code.push_back(_rhs->to_acc());
      code.emplace_back("STORE " + _div_proc->rhs);
      code.emplace_back("SET &$DIV" + _div_proc->get_no_uses() + "return");
      code.emplace_back("STORE " + _div_proc->get_ret_adr());
      code.emplace_back("JUMP &$DIV");
      code.emplace_back("&$DIV" + _div_proc->get_no_uses()  + "return");
      _div_proc->inc_no_uses();
      break;
    case basic_blocks_types::EXP_MOD:
      code.push_back(_lhs->to_acc());
      code.emplace_back("STORE " + _mod_proc->lhs);
      code.push_back(_rhs->to_acc());
      code.emplace_back("STORE " + _mod_proc->rhs);
      code.emplace_back("SET &$MOD" + _mod_proc->get_no_uses() + "return");
      code.emplace_back("STORE " + _mod_proc->get_ret_adr());
      code.emplace_back("JUMP &$MOD");
      code.emplace_back("&$MOD" + _mod_proc->get_no_uses()  + "return");
      _mod_proc->inc_no_uses();
      break;
  }

  return code;
}


