#include "validator.h"

#include <variant>
#include <vector>
#include <iostream>
#include <ostream>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
  /// TODO: Implement this
  return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(Token token) {
  /// TODO: Implement this
  State nextState = std::visit([&token](auto& currentState){return transition(currentState, token); }, SqlValidator::state_);
  SqlValidator::state_ = nextState;
}

bool is_valid_sql_query(std::vector<Token> tokens){
  SqlValidator validator;
  for ( auto token : tokens){
    validator.handle(token);
  }

  return validator.is_valid();
}

struct TransitionFromStartVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Select) const { return state::SelectStmt{}; }
  
  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}


/// 2222
struct TrnasitionSelectStmtVisitor{
  State operator()(token::Asterisks) const { return state::AllColumns{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
  
  // Invalid State 
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
}; 

State transition(state::SelectStmt, Token token){
  return std::visit(TrnasitionSelectStmtVisitor{}, token.value());
}


struct TrnasitionAllColumnsVisitor{
  // token : FROM -> state :: From Clause 
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};
State transition(state::AllColumns, Token token){
  return std::visit(TrnasitionAllColumnsVisitor{}, token.value());
}


struct TrnasitionNamedColumnVisitor{
  // token : Comma -> state :: morecolumns 
  // token : From -> state :: from 
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::MoreColumns{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};
State transition(state::NamedColumn, Token token){
  return std::visit(TrnasitionNamedColumnVisitor{}, token.value());
}


struct TrnasitionMoreColumnsVisitor{
  // token : Identifier -> state :: named column 
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
};
State transition(state::MoreColumns, Token token){
    return std::visit(TrnasitionMoreColumnsVisitor{}, token.value());
}


struct TransitionFromClause{
  // token : Identifier -> state:tablename 
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::TableName{}; }
};
State transition(state::FromClause, Token token){
  return std::visit(TransitionFromClause{}, token.value());
}


struct TransitionTableNameVisitor{
  // token : semicolon -> state : valid
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Valid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};
State transition(state::TableName, Token token){
    return std::visit(TransitionTableNameVisitor{}, token.value());
}



State transition(state::Valid, Token token) {
  auto t_ = token.value();
  return state::Valid{};
  // return std::visit(TransitionEndVisitor{}, token.value());
}

State transition(state::Invalid, Token token) {
  auto t_ = token.value();
  return state::Invalid{};
  // return std::visit(TransitionEndVisitor{}, token.value());
}

} // namespace sql
