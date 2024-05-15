#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_ID,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
    NODE_VARIABLE,
    NODE_STRING
};

struct AST_NODE
{
    enum NODE_TYPE TYPE;
    std::string *VALUE;
    AST_NODE *CHILD;
    std::vector<AST_NODE *> SUB_STATEMENTS; // maybe change later
};

class Parser
{
private:
    int size;                          // size of tokens
    int index;                         // index of current position
    std::vector<Token *> parserTokens; // the tokens in the parser
    Token *curr;                       // current token

public:
    Parser(std::vector<Token *> tokens)
    {
        parserTokens = tokens;
        index = 0;
        size = parserTokens.size();
        curr = parserTokens.at(index);
    }
    // main function of class that parses the tokens
    AST_NODE *parse()
    {
        AST_NODE *root = new AST_NODE();
        root->TYPE = NODE_ROOT;

        while (curr->TYPE != TOKEN_EOF)
        {
            switch (curr->TYPE)
            {
            case TOKEN_ID:
                root->SUB_STATEMENTS.push_back(parseID());
                break;
            case TOKEN_KEYWORD:
                root->SUB_STATEMENTS.push_back(parseKeyword());
                break;
            default:
                std::cout << "[!] Parser ERROR: found wrong token" << std::endl;
                exit(1);
            }
            proceed(TOKEN_SEMICOLON);
        }

        return root;
    }

    // parse the token of type ID
    AST_NODE *parseID()
    {
        std::string *buff = &curr->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUALS);

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_VARIABLE;
        newNode->CHILD = praseINT();

        return newNode;
    }

    // parse the token of type Keyword
    AST_NODE *parseKeyword()
    {
        if (curr->VALUE == "return")
            return parseReturn();
        else if (curr->VALUE == "print")
            return parsePrint();
        else
        {
            std::cout << "[!] Parser ERROR: found wrong token" << std::endl;
            exit(1);
        }
    }

    // parse the token of type return
    AST_NODE *parseReturn()
    {
        proceed(TOKEN_KEYWORD);
        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_RETURN;
        newNode->CHILD = praseINT();
        return newNode;
    }

    // parse the token of type print
    AST_NODE *parsePrint()
    {
        proceed(TOKEN_KEYWORD);
        proceed(TOKEN_LEFT_PAREN);
        proceed(TOKEN_QUOTE);

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_PRINT;
        newNode->CHILD = praseString();

        proceed(TOKEN_QUOTE);
        proceed(TOKEN_RIGHT_PAREN);

        return newNode;
    }

    // parse the token of type string
    AST_NODE *praseString()
    {
        if (curr->TYPE != TOKEN_STRING)
        {
            std::cout << "[!] Parser ERROR: Token should be string" << std::endl
                      << std::endl;
            exit(1);
        }
        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_STRING;
        newNode->VALUE = &curr->VALUE;
        proceed(TOKEN_STRING);
        return newNode;
    }

    // parse the token of type int
    AST_NODE *praseINT()
    {
        if (curr->TYPE != TOKEN_INT)
        {
            std::cout << "[!] Parser ERROR: Token should be int" << std::endl
                      << std::endl;
            exit(1);
        }

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_INT;
        newNode->VALUE = &curr->VALUE;
        proceed(TOKEN_INT);
        return newNode;
    }

    // advance to the next token
    Token *proceed(enum type Type)
    {
        if (curr->TYPE != Type)
        {
            std::cout << "[!] Parser ERROR: Token type is not type" << std::endl
                      << std::endl;
            exit(1);
        }
        index++;
        curr = parserTokens.at(index);
        return curr;
    }
};

#endif