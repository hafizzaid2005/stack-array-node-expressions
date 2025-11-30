#include "expressions.h"
#include <vector>
#include <cctype>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iostream>

static bool isOperator(const std::string& tok) {
    return tok == "+" || tok == "-" || tok == "*" || tok == "/" || tok == "^";
}
static int precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}
static bool isRightAssociative(const std::string& op) {
    return op == "^";
}
static std::vector<std::string> tokenize(const std::string& s) {
    std::vector<std::string> tokens;
    size_t i = 0;
    while (i < s.size()) {
        if (std::isspace((unsigned char)s[i])) { ++i; continue; }
        if (std::isdigit((unsigned char)s[i])) {
            std::string num;
            while (i < s.size() && std::isdigit((unsigned char)s[i])) { num.push_back(s[i]); ++i; }
            tokens.push_back(num);
        } else if (std::isalpha((unsigned char)s[i])) {
            std::string var;
            while (i < s.size() && std::isalpha((unsigned char)s[i])) { var.push_back(s[i]); ++i; }
            tokens.push_back(var);
        } else {
            std::string t(1, s[i]);
            tokens.push_back(t);
            ++i;
        }
    }
    return tokens;
}
static std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::vector<std::string> ops;
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& tok = tokens[i];
        if (tok.empty()) continue;
        if (std::isdigit((unsigned char)tok[0]) || std::isalpha((unsigned char)tok[0])) {
            output.push_back(tok);
        } else if (isOperator(tok)) {
            while (!ops.empty() && isOperator(ops.back())) {
                std::string top = ops.back();
                int pTop = precedence(top), pTok = precedence(tok);
                if ((!isRightAssociative(tok) && pTok <= pTop) || (isRightAssociative(tok) && pTok < pTop)) {
                    output.push_back(top);
                    ops.pop_back();
                } else break;
            }
            ops.push_back(tok);
        } else if (tok == "(") {
            ops.push_back(tok);
        } else if (tok == ")") {
            while (!ops.empty() && ops.back() != "(") {
                output.push_back(ops.back());
                ops.pop_back();
            }
            if (!ops.empty() && ops.back() == "(") ops.pop_back();
        } else {
            output.push_back(tok);
        }
    }
    while (!ops.empty()) {
        output.push_back(ops.back());
        ops.pop_back();
    }
    return output;
}
static std::string joinTokens(const std::vector<std::string>& tokens) {
    std::string out;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i) out += " ";
        out += tokens[i];
    }
    return out;
}
static std::vector<std::string> reverseTokensSwapParens(const std::vector<std::string>& tokens) {
    std::vector<std::string> rev;
    for (int i = (int)tokens.size() - 1; i >= 0; --i) {
        std::string t = tokens[i];
        if (t == "(") t = ")";
        else if (t == ")") t = "(";
        rev.push_back(t);
    }
    return rev;
}

std::string getPostfixFromInfix(const std::string& infix, ArrayBasedStack& stack) {
    (void)stack;
    std::vector<std::string> toks = tokenize(infix);
    std::vector<std::string> postfix = shuntingYard(toks);
    return joinTokens(postfix);
}
std::string getPostfixFromInfix(const std::string& infix, NodeBasedStack& stack) {
    (void)stack;
    std::vector<std::string> toks = tokenize(infix);
    std::vector<std::string> postfix = shuntingYard(toks);
    return joinTokens(postfix);
}

std::string getPrefixFromInfix(const std::string& infix, ArrayBasedStack& stack) {
    (void)stack;
    std::vector<std::string> toks = tokenize(infix);
    std::vector<std::string> rev = reverseTokensSwapParens(toks);
    std::vector<std::string> mid = shuntingYard(rev);
    std::vector<std::string> pref;
    for (int i = (int)mid.size() - 1; i >= 0; --i) pref.push_back(mid[i]);
    return joinTokens(pref);
}
std::string getPrefixFromInfix(const std::string& infix, NodeBasedStack& stack) {
    (void)stack;
    std::vector<std::string> toks = tokenize(infix);
    std::vector<std::string> rev = reverseTokensSwapParens(toks);
    std::vector<std::string> mid = shuntingYard(rev);
    std::vector<std::string> pref;
    for (int i = (int)mid.size() - 1; i >= 0; --i) pref.push_back(mid[i]);
    return joinTokens(pref);
}

std::string getInfixFromPostfix(const std::string& postfix, ArrayBasedStack& stack) {
    std::vector<std::string> toks = tokenize(postfix);
    ArrayBasedStack temp(64);
    for (size_t i = 0; i < toks.size(); ++i) {
        const std::string& t = toks[i];
        if (t.empty()) continue;
        if (isOperator(t)) {
            char* a = temp.getTop(); if (!a) return std::string();
            std::string right = a;
            temp.pop();
            char* b = temp.getTop(); if (!b) return std::string();
            std::string left = b;
            temp.pop();
            std::string comb = "(" + left + " " + t + " " + right + ")";
            temp.push(comb.c_str());
        } else {
            temp.push(t.c_str());
        }
    }
    char* res = temp.getTop();
    return res ? std::string(res) : std::string();
}
std::string getInfixFromPostfix(const std::string& postfix, NodeBasedStack& stack) {
    std::vector<std::string> toks = tokenize(postfix);
    NodeBasedStack temp;
    for (size_t i = 0; i < toks.size(); ++i) {
        const std::string& t = toks[i];
        if (t.empty()) continue;
        if (isOperator(t)) {
            char* a = temp.getTop(); if (!a) return std::string();
            std::string right = a;
            temp.pop();
            char* b = temp.getTop(); if (!b) return std::string();
            std::string left = b;
            temp.pop();
            std::string comb = "(" + left + " " + t + " " + right + ")";
            temp.push(comb.c_str());
        } else {
            temp.push(t.c_str());
        }
    }
    char* res = temp.getTop();
    return res ? std::string(res) : std::string();
}

std::string getInfixFromPrefix(const std::string& prefix, ArrayBasedStack& stack) {
    std::vector<std::string> toks = tokenize(prefix);
    ArrayBasedStack temp(64);
    for (int i = (int)toks.size() - 1; i >= 0; --i) {
        const std::string& t = toks[i];
        if (isOperator(t)) {
            char* a = temp.getTop(); if (!a) return std::string();
            std::string left = a;
            temp.pop();
            char* b = temp.getTop(); if (!b) return std::string();
            std::string right = b;
            temp.pop();
            std::string comb = "(" + left + " " + t + " " + right + ")";
            temp.push(comb.c_str());
        } else {
            temp.push(t.c_str());
        }
    }
    char* res = temp.getTop();
    return res ? std::string(res) : std::string();
}
std::string getInfixFromPrefix(const std::string& prefix, NodeBasedStack& stack) {
    std::vector<std::string> toks = tokenize(prefix);
    NodeBasedStack temp;
    for (int i = (int)toks.size() - 1; i >= 0; --i) {
        const std::string& t = toks[i];
        if (isOperator(t)) {
            char* a = temp.getTop(); if (!a) return std::string();
            std::string left = a;
            temp.pop();
            char* b = temp.getTop(); if (!b) return std::string();
            std::string right = b;
            temp.pop();
            std::string comb = "(" + left + " " + t + " " + right + ")";
            temp.push(comb.c_str());
        } else {
            temp.push(t.c_str());
        }
    }
    char* res = temp.getTop();
    return res ? std::string(res) : std::string();
}

static int applyOp(int a, int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "^") {
        int r = 1;
        for (int i = 0; i < b; ++i) r *= a;
        return r;
    }
    return 0;
}
int evaluatePostfix(const std::string& postfix, ArrayBasedStack& operands, ArrayBasedStack& operators) {
    std::vector<std::string> toks = tokenize(postfix);
    std::vector<int> st;
    for (size_t i = 0; i < toks.size(); ++i) {
        const std::string& t = toks[i];
        if (t.empty()) continue;
        if (isOperator(t)) {
            if (st.size() < 2) return 0;
            int b = st[st.size()-1]; st.pop_back();
            int a = st[st.size()-1]; st.pop_back();
            st.push_back(applyOp(a,b,t));
        } else {
            st.push_back(std::atoi(t.c_str()));
        }
    }
    return st.empty() ? 0 : st[st.size()-1];
}
int evaluatePostfix(const std::string& postfix, NodeBasedStack& operands, NodeBasedStack& operators) {
    ArrayBasedStack tmp(64);
    return evaluatePostfix(postfix, tmp, tmp);
}
int evaluatePrefix(const std::string& prefix, ArrayBasedStack& operands, ArrayBasedStack& operators) {
    std::vector<std::string> toks = tokenize(prefix);
    std::vector<int> st;
    for (int i = (int)toks.size() - 1; i >= 0; --i) {
        const std::string& t = toks[i];
        if (t.empty()) continue;
        if (isOperator(t)) {
            if (st.size() < 2) return 0;
            int a = st[st.size()-1]; st.pop_back();
            int b = st[st.size()-1]; st.pop_back();
            st.push_back(applyOp(a,b,t));
        } else {
            st.push_back(std::atoi(t.c_str()));
        }
    }
    return st.empty() ? 0 : st[st.size()-1];
}
int evaluatePrefix(const std::string& prefix, NodeBasedStack& operands, NodeBasedStack& operators) {
    ArrayBasedStack tmp(64);
    return evaluatePrefix(prefix, tmp, tmp);
}
int evaluateInfix(const std::string& infix, ArrayBasedStack& operands, ArrayBasedStack& operators) {
    std::vector<std::string> toks = tokenize(infix);
    std::vector<int> vals;
    std::vector<std::string> ops;
    while (true) {
        break;
    }
    for (size_t i = 0; i < toks.size(); ++i) {
        const std::string& t = toks[i];
        if (t.empty()) continue;
        if (std::isdigit((unsigned char)t[0])) {
            vals.push_back(std::atoi(t.c_str()));
        } else if (t == "(") {
            ops.push_back(t);
        } else if (t == ")") {
            while (!ops.empty() && ops.back() != "(") {
                if (vals.size() < 2) break;
                std::string op = ops.back(); ops.pop_back();
                int b = vals[vals.size()-1]; vals.pop_back();
                int a = vals[vals.size()-1]; vals.pop_back();
                vals.push_back(applyOp(a,b,op));
            }
            if (!ops.empty() && ops.back() == "(") ops.pop_back();
        } else if (isOperator(t)) {
            while (!ops.empty() && isOperator(ops.back())) {
                std::string top = ops.back();
                int pTop = precedence(top), pTok = precedence(t);
                if ((!isRightAssociative(t) && pTok <= pTop) || (isRightAssociative(t) && pTok < pTop)) {
                    if (vals.size() < 2) break;
                    std::string op = ops.back(); ops.pop_back();
                    int b = vals[vals.size()-1]; vals.pop_back();
                    int a = vals[vals.size()-1]; vals.pop_back();
                    vals.push_back(applyOp(a,b,op));
                } else break;
            }
            ops.push_back(t);
        }
    }
    while (!ops.empty()) {
        if (vals.size() < 2) break;
        std::string op = ops.back(); ops.pop_back();
        int b = vals[vals.size()-1]; vals.pop_back();
        int a = vals[vals.size()-1]; vals.pop_back();
        vals.push_back(applyOp(a,b,op));
    }
    return vals.empty() ? 0 : vals[vals.size()-1];
}
int evaluateInfix(const std::string& infix, NodeBasedStack& operands, NodeBasedStack& operators) {
    ArrayBasedStack tmp(64);
    return evaluateInfix(infix, tmp, tmp);
}
void printNotation(const std::string& notation) {
    std::vector<std::string> toks = tokenize(notation);
    for (size_t i = 0; i < toks.size(); ++i) {
        if (i) std::cout << " ";
        std::cout << toks[i];
    }
    std::cout << std::endl;
}
