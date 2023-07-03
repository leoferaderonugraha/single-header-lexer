#ifndef _LEXER_H
#define _LEXER_H

#include <string>
#include <cctype>
#include <vector>
#include <variant>
#include <iostream>
#include <array>

enum class TokenType {
    IDENTIFIER,
    DIGIT,
    PUNCTUATION,
    END,
};

typedef struct {
    TokenType type;
    std::string literal;
    std::string type_literal;  // for debugging purposes
    size_t line;
} Token;

class Lexer {
public:
    Lexer(std::string source) {
        this->source = source;
        this->source_len = source.length();
        this->start = 0;
        this->current = 0;
    };

    std::vector<Token> analyze() {
        while (!this->is_eof()) {
            this->start = this->current;
            this->scan_token();
        }

        this->tokens.push_back(Token{
            .type = TokenType::END,
            .literal = "\0",
            .type_literal = "TokenType::END",
        });

        return this->tokens;
    }

    
private:
    std::string source;
    size_t start;
    size_t current;
    size_t source_len;
    std::vector<Token> tokens;

    void scan_token() {
        char ch = this->advance();

        if (std::isalpha(ch)) {
            this->token_identifier();
        } else if (std::isdigit(ch)) {
            this->token_digit();
        } else if (this->is_punc(ch)) {
            this->token_punctuation();
        } else if(ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t') {
            /* consume */
        } else {
            std::cout << "Unexpected token: " << '"' << ch << '"' << std::endl;
        }
    }

    char peek() {
        if (this->current >= this->source_len) {
            return '\0';
        }

        return this->source[this->current];
    }

    char advance() {
        return this->source[this->current++];
    }

    void add_token(TokenType type, std::string literal) {
        Token token = {
            .type = type,
            .literal = literal,
        };

        switch (type) {
            case TokenType::IDENTIFIER:
                token.type_literal = "TokenType::IDENTIFIER";
                break;
            case TokenType::DIGIT:
                token.type_literal = "TokenType::DIGIT";
                break;
            case TokenType::PUNCTUATION:
                token.type_literal = "TokenType::PUNCTUATION";
                break;
            case TokenType::END:
                token.type_literal = "TokenType::END";
                break;
        }

        this->tokens.push_back(token);
    }

    void token_identifier() {
        while (std::isalnum(this->peek())) {
            this->advance();
        }

        size_t len = this->current - this->start;
        std::string literal = this->source.substr(this->start, len);
        TokenType type = TokenType::IDENTIFIER;

        this->add_token(type, literal);
    }

    void token_digit() {
        while (std::isdigit(this->peek())) {
            this->advance();
        }

        size_t len = this->current - this->start;
        std::string literal = this->source.substr(this->start, len);
        TokenType type = TokenType::DIGIT;

        this->add_token(type, literal);
    }

    void token_punctuation() {
        size_t len = this->current - this->start;
        std::string literal = this->source.substr(this->start, len);
        TokenType type = TokenType::PUNCTUATION;
        this->add_token(type, literal);
    }

    bool is_punc(char ch) {
        std::string puncs = ".,'\"?!";
        bool result = false;

        for (char c : puncs) {
            if (ch == c) {
                result = true;
                break;
            }
        }

        return result;
    }

    bool is_eof() {
        return this->current >= this->source_len;
    }
};

#endif // _LEXER_H
