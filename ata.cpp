#include "headers/lexer.hpp"
#include "headers/parser.hpp"

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        std::cout << "Please provide a file";
        exit(1);
    }

    std::cout << std::endl
              << "Reading from: " << argv[1] << std::endl;
    std::ifstream mainFile(argv[1]);
    std::stringstream buf;

    char t;

    while (mainFile.get(t))
        buf << t;

    std::string orgCode = buf.str();
    std::cout << "The code is:" << std::endl
              << std::endl
              << orgCode << std::endl;

    // create the lexer
    Lexer lexer(orgCode);
    std::vector<Token *> tokens = lexer.tokonize(); // create Tokens
    std::cout << std::endl
              << "[*] Tokenize success!" << std::endl;
    // print Tokens
    // std::cout << std::endl
    //           << std::endl;
    // int counter = 0;
    // for (Token *t : tokens)
    // {
    //     counter++;
    //     std::cout << counter << ". " << typeToString(t->TYPE) << ", " << t->VALUE << std::endl;
    // }

    // create the parser
    Parser parser(tokens);
    AST_NODE *root = parser.parse();
    std::cout
        << "[*] Parsing success!, found: " << root->SUB_STATEMENTS.size() << " statements" << std::endl;
    // end of code
    std::cout << std::endl
              << "Compiling Success!!!" << std::endl;

    return 0;
}