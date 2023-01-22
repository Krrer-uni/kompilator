//
// Created by krrer on 21.01.23.
//

#ifndef KOMPILATOR_SRC_MULT_DIV_H_
#define KOMPILATOR_SRC_MULT_DIV_H_

#include "basic_blocks/basic_blocks.h"

class PROCEDURE_BLOCK;
class MULT_PROC : public PROCEDURE_BLOCK {

 public:
  std::vector<std::string> translate_block() override;
  MULT_PROC(std::string lhs,
            std::string rhs,
            std::string acc,
            std::string one_const,
            uint16_t return_address);
  std::string lhs;
  std::string rhs;
  std::string acc;
  std::string one_const;
  std::string get_ret_adr();
  std::string get_no_uses();
  void inc_no_uses();
};

class DIV_PROC : public PROCEDURE_BLOCK{

  std::vector<std::string> translate_block() override;
 public:
  DIV_PROC(std::string lhs,
           std::string rhs,
           std::string acc,
           std::string k,
           uint16_t return_address);
  std::string k;
  std::string lhs;
  std::string rhs;
  std::string acc;
  std::string get_ret_adr();
  std::string get_no_uses();
  void inc_no_uses();
};

class MOD_PROC : public PROCEDURE_BLOCK{

  std::vector<std::string> translate_block() override;
 public:
  MOD_PROC(std::string lhs,
           std::string rhs,
           std::string acc,
           std::string k,
           uint16_t return_address);
  std::string k;
  std::string lhs;
  std::string rhs;
  std::string acc;
  std::string one_const;
  std::string get_ret_adr();
  std::string get_no_uses();
  void inc_no_uses();
};





#endif //KOMPILATOR_SRC_MULT_DIV_H_
