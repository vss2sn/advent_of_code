#!/usr/bin/python3
from collections import deque

def getExpr(line):
    expr = deque()
    for char in line:
        if char != ' ':
            expr.append(char)
    return expr

def getSubExpr(expr):
    bracket_count = 0
    expr.popleft()
    new_expr = deque()
    while bracket_count > 0 or expr[0] != ')':
        if expr[0] == '(':
            bracket_count += 1
        elif expr[0] == ')':
            bracket_count -= 1
        new_expr.append(expr[0])
        expr.popleft()
    expr.popleft()
    return new_expr

def calcExprValue(expr):
    while len(expr) > 1:
        operand1 = int()
        operand2 = int()
        if expr[0] != '(':
            operand1 = int(expr.popleft())
        else:
            operand1 = calcExprValue(getSubExpr(expr))
        operator = expr.popleft()
        if expr[0] != '(':
            operand2 = int(expr.popleft())
        else:
            operand2 = calcExprValue(getSubExpr(expr))
        if operator == '+':
            expr.appendleft(str(operand1 + operand2))
        elif operator == '*':
            expr.appendleft(str(operand1 * operand2))
    return int(expr[0])

def calcExprValueAdv(expr):
    simplifiedEquation = deque()
    while len(expr) > 1:
        operand1 = int()
        operand2 = int()
        if expr[0] != '(':
            operand1 = int(expr.popleft())
        else:
            operand1 = calcExprValueAdv(getSubExpr(expr))
        operator = expr.popleft()
        if expr[0] != '(':
            operand2 = int(expr.popleft())
        else:
            operand2 = calcExprValueAdv(getSubExpr(expr))

        if operator == '+':
            expr.appendleft(str(operand1 + operand2))
        elif operator == '*':
            expr.appendleft(str(operand2))
            simplifiedEquation.append(str(operand1))
            simplifiedEquation.append(operator)
    simplifiedEquation.append(expr.popleft())
    return calcExprValue(simplifiedEquation)


def main():
    f = open("../cpp/day_18_data.txt")
    sum = 0
    for line in f:
        line = line.strip('\n')
        expr = getExpr(line)
        sum += calcExprValueAdv(expr)
    print(sum)
    return sum

if __name__ == "__main__":
    main()
