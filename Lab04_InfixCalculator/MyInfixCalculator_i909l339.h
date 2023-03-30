#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "./330/Lab03_StackAndQueue/MyStack_Template.h"
#include "./330/Lab03_StackAndQueue/MyQueue_Template.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator() {}

    ~MyInfixCalculator() {}

    double calculate(const std::string& s)
    {
        // code begins
        MyVector<std::string> token_vec;
        MyVector<std::string> postfix_vec;
        std::cout << "Vectors created" << std::endl;
        tokenize(s, token_vec);
        std::cout << "tokenization success" << std::endl;
        for (size_t i = 0; i < token_vec.size(); i++)
            std::cout << token_vec[i] << std::endl;
        infixToPostfix(token_vec, postfix_vec);
        std::cout << "conversion success" << std::endl;
        std::cout << postfix_vec.size() << std::endl;
        return calPostfix(postfix_vec);
        // code ends
    }

  private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens) {
        int i = 0;
        while (i < s.size()) {
            // WARNING
            // my hope is that checking for negative index first will prevent errors but come back to this

            if (isDigit(s[i]) 
            || (s[i] == '-' && i-1 < 0) 
            || (s[i] == '-' && !isDigit(s[i-1]) && s[i-1] != ')' && isDigit(s[i+1]))
            || s[i] == '.') {

                int start_index = i;
                int len = 1;

                while (isDigit(s[++i]) || s[i] == '.')
                    len++;

                tokens.push_back(s.substr(start_index, len));
            } else {
                //tokens.push_back(std::to_string(s[i]));
                tokens.push_back(s.substr(i, 1));
                ++i;
            }
        }


    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens) {
        MyStack<std::string> operations;

        for (size_t index = 0; index < infix_tokens.size(); index++) {
            if (infix_tokens[index] == "(") {
                // open paren should be pushed no matter what
                operations.push(infix_tokens[index]);
            } else if (infix_tokens[index] == ")") {
                // we will assume parens are balanced
                while (operations.top() != "(") {
                    postfix_tokens.push_back(operations.top());
                    operations.pop();
                }
                // pop open paren without adding to tokens
                operations.pop();
            } else if (infix_tokens[index] == "+" || infix_tokens[index] == "-" || infix_tokens[index] == "*" || infix_tokens[index] == "/") {
                if (operations.empty() || operations.top() == "(") {
                    operations.push(infix_tokens[index]);
                } else {
                    while (!operations.empty() && operations.top() != "(" && operatorPrec(operations.top()[0]) < operatorPrec(infix_tokens[index][0])) {
                        postfix_tokens.push_back(operations.top());
                        operations.pop();
                    }
                    operations.push(infix_tokens[index]);
                }
            } else {
                postfix_tokens.push_back(infix_tokens[index]);
            }
        }


        // add operator to stack
        // if higher precende, add to stack, otherwise
        // pop top and add to postfix token
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
        // use new stack
        // push number to stack
        // on encountering an operator, pops twice and applies operand; push result to stack
        
        MyStack<std::string> num_stack;

        for (MyVector<std::string>::const_iterator itr = postfix_tokens.begin(); itr != postfix_tokens.end(); ++itr) {
            if (*itr == "+" || *itr == "-" || *itr == "*" || *itr == "/") {
                std::string num1 = num_stack.top();
                num_stack.pop();
                std::string num2 = num_stack.top();
                num_stack.pop(); 

                double d = computeBinaryOperation(num1, num2, *itr);
                num_stack.push(std::to_string(d));

            } else { 
                num_stack.push(*itr);
            }
        }

        return std::stod(num_stack.top());

    }
};

#endif // __MYINFIXCALCULATOR_H__