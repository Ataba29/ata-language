#ifndef __LEXER_H
#define __LEXER_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

enum type
{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTE,
    TOKEN_EOF
};

struct Token
{
    enum type TYPE;
    std::string VALUE;
};
// convert the enum to its string value
std::string typeToString(enum type Type)
{
    switch (Type)
    {
    case TOKEN_ID:
        return "TOKEN_ID";
        break;
    case TOKEN_INT:
        return "TOKEN_INT";
        break;
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
        break;
    case TOKEN_LEFT_PAREN:
        return "TOKEN_LEFT_PAREN";
        break;
    case TOKEN_RIGHT_PAREN:
        return "TOKEN_RIGHT_PAREN";
        break;
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
        break;
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
        break;
    case TOKEN_STRING:
        return "TOKEN_STRING";
        break;
    case TOKEN_QUOTE:
        return "TOKEN_QUOTE";
        break;
    case TOKEN_EOF:
        return "TOKEN_EOF";
        break;
    default:
        return "Unknown";
        break;
    }
}

class Lexer
{
public:
    // contructor
    Lexer(std::string sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        curr = sourceCode.at(cursor);
        lineNumber = 1;
        charInLineNumber = 1;
    }

    // return char at curr positions and advance by 1
    char advance()
    {
        // update lineNumber, charInLineNumber
        if (curr == '\n')
        {
            lineNumber++;
            charInLineNumber = 1;
        }
        else
            charInLineNumber++;

        // advance
        if (cursor < size)
        {
            char temp = curr;
            cursor++;
            curr = (cursor < size) ? source[cursor] : '\0';
            return temp;
        }
        return '\0';
    }

    // check what is the char at an offset
    char peek(int offset)
    {
        if (cursor + offset < size)
            return source[cursor + offset];
        return '\0';
    }

    // converts the source into a vector of tokens
    std::vector<Token *> tokonize()
    {
        std::vector<Token *> tokens;
        Token *temp;
        bool reachedEnd = false;
        while (cursor < size && !reachedEnd)
        {
            skipWhiteSpace();
            // token is an ID, example "num1"
            if (isalpha(curr) || curr == '_')
            {
                tokens.push_back(tokonizeID_KEYWORD());
                continue;
            }
            // token is an int, example "123"
            if (isdigit(curr))
            {
                tokens.push_back(tokonizeINT());
                continue;
            }
            // switch for all the special tokens
            switch (curr)
            {
            case '"':
            {
                tokens.push_back(tokenizeSpecial(TOKEN_QUOTE));
                tokens.push_back(tokenizeString());
                tokens.push_back(tokenizeSpecial(TOKEN_QUOTE));
                break;
            }
            case '=':
            {
                tokens.push_back(tokenizeSpecial(TOKEN_EQUALS));
                break;
            }
            case '(':
            {
                tokens.push_back(tokenizeSpecial(TOKEN_LEFT_PAREN));
                break;
            }
            case ')':
            {
                tokens.push_back(tokenizeSpecial(TOKEN_RIGHT_PAREN));
                break;
            }
            case ';':
            {
                tokens.push_back(tokenizeSpecial(TOKEN_SEMICOLON));
                break;
            }
            default:
            {
                std::cout << std::endl
                          << std::endl
                          << "[!] LEXER ERROR: unidentified char: " << curr << std::endl
                          << "[!] at line: " << lineNumber << ", at char: " << charInLineNumber;
                exit(1);
            }
            }
        }

        // add EOF
        Token *t = new Token();
        t->TYPE = TOKEN_EOF;
        t->VALUE = '\0';
        tokens.push_back(t);

        return tokens;
    }

private:
    std::string source;   // the code
    int cursor;           // the location that we are at
    int size;             // the size of the code string
    char curr;            // the current char we are at
    int lineNumber;       // number of line that we are on start at 1
    int charInLineNumber; // the nmber of chars we have crossed in the line start at 1

    // skip all white spaces until we reach chars
    void skipWhiteSpace()
    {
        while (isWhiteSpace())
        {
            advance();
        }
    }
    // return true if curr is pointing at white space false if else
    bool isWhiteSpace()
    {
        return (curr == '\t' || curr == ' ' || curr == '\n' || curr == '\r');
    }

    // create single id token
    std::vector<std::string> keywords = {"return", "print"};
    Token *tokonizeID_KEYWORD()
    {
        std::stringstream buff;
        while (isalnum(curr) || curr == '_')
        {
            buff << advance();
        }
        Token *t = new Token();
        t->TYPE = (std::find(keywords.begin(), keywords.end(), buff.str()) != keywords.end())
                      ? TOKEN_KEYWORD
                      : TOKEN_ID;
        t->VALUE = buff.str();

        return t;
    }
    // create single int token
    Token *tokonizeINT()
    {
        std::stringstream buff;
        while (isdigit(curr))
        {
            buff << advance();
        }
        Token *t = new Token();
        t->TYPE = TOKEN_INT;
        t->VALUE = buff.str();

        return t;
    }
    // token is a special char
    Token *tokenizeSpecial(enum type tokenType)
    {
        std::stringstream buff;
        buff << advance();
        Token *t = new Token();
        t->TYPE = tokenType;
        t->VALUE = buff.str();

        return t;
    }

    // create a string token
    Token *tokenizeString()
    {
        std::stringstream buff;
        while (curr != '"')
        {
            if (curr == '\0')
            {
                std::cout << "[!] LEXER ERROR: missing quotes";
                exit(1);
            }

            buff << advance();
        }

        Token *newToken = new Token();
        newToken->TYPE = TOKEN_STRING;
        newToken->VALUE = buff.str();
        return newToken;
    }
};
#endif