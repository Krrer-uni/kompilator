//
// Created by krrer on 09.01.23.
//
#include <map>
#include <iostream>
#include "compiler.h"

Compiler::Compiler(int debug_mode) {
  _variable_map = std::map<std::string, variable>();
  _debug_mode = debug_mode;
}
void Compiler::add_variable(const std::string& name, variable_type type) {
  _variable_map[name] = variable{name, type};
}
void Compiler::handle_declaration (std::string name, variable_type type) {
  add_variable(name, type);
  if(_debug_mode > 0){
    std::cout << "created var " + name + " with type " + std::to_string(type) + "\n";
  }
}
std::shared_ptr<VAR_BLOCK> Compiler::handle_variable(const std::string& name) {
  if(_variable_map.find(name) == _variable_map.end()){
    std::cout << "variable " + name + " was not declared in the scope";
    return nullptr;
  }
  auto type = _variable_map[name].get_type();
  auto block = std::make_shared<VAR_BLOCK>(name,type);
  if(_debug_mode > 0){
    std::cout << "added block for var " + name + "\n";
  }
  return block;
}
std::shared_ptr<NUM_BLOCK> Compiler::handle_literal(unsigned long long int value) {
  auto block = std::make_shared<NUM_BLOCK>(value);
  if(_debug_mode > 0){
    std::cout << "added block for literal " + std::to_string(value) + "\n";
  }
  return block;
}
