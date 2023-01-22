//
// Created by krrer on 21.01.23.
//

#include "mult_div.h"

#include <utility>

std::vector<std::string> MULT_PROC::translate_block() {
  std::vector<std::string> code;
  code.emplace_back("&$MULT");
  code.emplace_back("LOAD " + acc);
  code.emplace_back("SUB 0");
  code.emplace_back("STORE " + acc);
  code.emplace_back("LOAD " + rhs);
  code.emplace_back("&$MULT_START");
  code.emplace_back("HALF");
  code.emplace_back("ADD 0");
  code.emplace_back("ADD " + one_const);
  code.emplace_back("SUB " + rhs);
  code.emplace_back("JPOS &$MULT_SKIP");
  code.emplace_back("LOAD " + acc);
  code.emplace_back("ADD " + lhs);
  code.emplace_back("STORE " + acc);
  code.emplace_back("&$MULT_SKIP");
  code.emplace_back("LOAD " + lhs);
  code.emplace_back("ADD 0");
  code.emplace_back("STORE " + lhs);
  code.emplace_back("LOAD " + rhs);
  code.emplace_back("HALF ");
  code.emplace_back("STORE " + rhs);
  code.emplace_back("JPOS &$MULT_START");
  code.emplace_back("LOAD " + acc);
  code.emplace_back("JUMPI " + std::to_string(_return_adress));
  return code;
}

MULT_PROC::MULT_PROC(std::string lhs,
                     std::string rhs,
                     std::string acc,
                     std::string one_const,
                     uint16_t return_address)
    : lhs(std::move(lhs)),
      rhs(std::move(rhs)),
      acc(std::move(acc)),
      one_const(std::move(one_const)) {
  _return_adress = return_address;
  _no_uses = 0;
}
std::string MULT_PROC::get_ret_adr() {
  return std::to_string(_return_adress);
}
std::string MULT_PROC::get_no_uses() {
  return std::to_string(_no_uses);
}
void MULT_PROC::inc_no_uses() {
  _no_uses++;
}

