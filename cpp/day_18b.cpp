#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

std::deque<std::string> convertStringToDeque(const std::string& line) {
  std::deque<std::string> cur_expr;
  for (int i = 0; i < line.size(); i++) {
    if (line[i] != ' ') {
      cur_expr.push_back(std::string{line[i]});
    }
  }
  return cur_expr;
}

std::deque<std::string> getSubExpr(std::deque<std::string>& expr) {
  int brack_count = 0;
  expr.pop_front();
  std::deque<std::string> new_expr;
  while (brack_count > 0 || expr.front() != ")") {
    if (expr.front() == "(") {
      brack_count++;
    }
    if (expr.front() == ")") {
      brack_count--;
    }
    new_expr.push_back(expr.front());
    expr.pop_front();
  }
  expr.pop_front();
  return new_expr;
}

long long calcExpressionValue(std::deque<std::string>& expr) {
  while (expr.size() > 1) {
    long long operand1;
    long long operand2;
    std::string op;

    if (expr.front() != "(") {
      operand1 = std::stol(expr.front());
      expr.pop_front();
    } else {
      auto new_expr = getSubExpr(expr);
      operand1 = calcExpressionValue(new_expr);
    }

    op = expr.front();
    expr.pop_front();

    if (expr.front() != "(") {
      operand2 = std::stol(expr.front());
      expr.pop_front();
    } else {
      auto new_expr = getSubExpr(expr);
      operand2 = calcExpressionValue(new_expr);
    }
    if (op == "+") expr.push_front(std::to_string(operand1 + operand2));
    if (op == "*") expr.push_front(std::to_string(operand1 * operand2));
  }
  return std::stol(expr.front());
}

long long calcExpressionValueAdvanced(std::deque<std::string>& expr) {
  std::deque<std::string> simplifiedEquation;
  while (expr.size() > 1) {
    long long operand1;
    long long operand2;
    std::string op;

    if (expr.front() != "(") {
      operand1 = std::stol(expr.front());
      expr.pop_front();
    } else {
      auto new_expr = getSubExpr(expr);
      operand1 = calcExpressionValueAdvanced(new_expr);
    }

    op = expr.front();
    expr.pop_front();

    if (expr.front() != "(") {
      operand2 = std::stol(expr.front());
      expr.pop_front();
    } else {
      auto new_expr = getSubExpr(expr);
      operand2 = calcExpressionValueAdvanced(new_expr);
    }
    if (op == "+") expr.push_front(std::to_string(operand1 + operand2));
    // If the operation is multiplcation, add the second operand back to the
    // front of the original deque and add the first operand the the operator to
    // the simplified equation to build up an equation that consists of only
    // multiplication
    if (op == "*") {
      expr.push_front(std::to_string(operand2));
      simplifiedEquation.push_back(std::to_string(operand1));
      simplifiedEquation.push_back(op);
    }
  }
  // Get the final operand from the equation into simplified equation
  std::string finalOperand = expr.front();
  simplifiedEquation.push_back(finalOperand);
  expr.pop_front();
  std::cout << "Hello" << '\n';
  // Finally solve the remaining multiplication using our original
  // solveEquation()
  return calcExpressionValue(simplifiedEquation);
}

int main() {
  std::fstream file{"../input/day_18_input"};
  std::string line;
  long long sum = 0;
  while (std::getline(file, line)) {
    line.erase(std::remove_if(std::begin(line), std::end(line),
                              [](auto c) { return !isprint(c) || c == ' '; }),
               std::end(line));
    std::cout << line << '\n';
    auto d_line = convertStringToDeque(line);
    sum += calcExpressionValueAdvanced(d_line);
  }

  std::cout << sum << '\n';
  return sum;
}
