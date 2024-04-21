#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>
#include <cctype>

// Разделение на токены

struct OpeningBracket {};
struct ClosingBracket {};
struct Comma {};
struct Number {
	int value;
};

struct UnknownToken {
	std::string value;
};

struct MinToken {};
struct AbsToken {};
struct Plus {};
struct Minus {};
struct Multiply {};
struct Modulo {};
struct Divide {};
struct MaxToken {};
struct SqrToken {};

using Token = std::variant<OpeningBracket, ClosingBracket, Comma, Number, UnknownToken,
	Multiply, Modulo, MinToken, AbsToken, Plus, Minus, Divide, MaxToken, SqrToken>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
	{',', Comma {}}, { '(', OpeningBracket{} }, {')', ClosingBracket{}}, {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'%', Modulo{}}, {'/', Divide{}} };

const std::unordered_map<std::string, Token> kName2Token{
		{"min", MinToken{}}, {"max", MaxToken{}}, {"abs", AbsToken{}}, {"sqr", SqrToken{}} };


int ToDigit(unsigned char symbol) {
	return symbol - '0';
}

Number ParseNumber(const std::string &input, size_t &pos) {
	int value = 0;
	auto symbol = static_cast<unsigned char>(input[pos]);
	while (std::isdigit(symbol)) {
		value = value * 10 + ToDigit(symbol);
		if (pos == input.size() - 1) {
			break;
		}
		symbol = static_cast<unsigned char>(input[++pos]);
	}
	return Number{ value };
}

Token ParseName(const std::string input, size_t &pos) {
	size_t start = pos;
	while (std::isalnum(input[pos])) {
		++pos;
	}
	std::string name = input.substr(start, pos - start);
	auto it = kName2Token.find(name);
	if (it != kName2Token.end()) {
		return it->second;
	}
	else {
		return UnknownToken{ name };
	}
}

std::vector<Token> Tokenize(const std::string &input) {
	std::vector<Token> tokens;
	const size_t size = input.size();
	size_t pos = 0;
	while (pos < size) {
		const auto symbol = static_cast<unsigned char>(input[pos]);
		if (std::isspace(symbol)) {
			++pos;
		}
		else if (std::isdigit(symbol)) {
			tokens.emplace_back(ParseNumber(input, pos));
		}
		else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
			tokens.emplace_back(it->second);
			++pos;
		}
		else {
			auto name_token = ParseName(input, pos);
			tokens.push_back(name_token);
		}
	}
	return tokens;
}