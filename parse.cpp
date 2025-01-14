#include "parse.h"
#include "lexer.h"
#include "grammar.tab.hh"

#include <cassert>
#include <memory>

static std::vector<Token> tokens;
static int tokenIndex;

extern std::shared_ptr<ast::Node> bisonProgramResult;

int yylex(yy::parser::semantic_type *un) {
    if (tokenIndex >= tokens.size()) {
        return yy::parser::token::YYEOF;
    }

    auto token = tokens[tokenIndex++];

    if (std::holds_alternative<TokenInt>(token)) {
        *un = std::make_shared<ast::Integer>(std::get<TokenInt>(token));
        return yy::parser::token::INTEGER;
    }

    if (std::holds_alternative<TokenFloat>(token)) {
        *un = std::make_shared<ast::Floating>(std::get<TokenFloat>(token));
        return yy::parser::token::FLOATING;
    }

    if (std::holds_alternative<TokenSymbol>(token)) {
        return std::get<TokenSymbol>(token);
    }

    if (std::holds_alternative<TokenIdent>(token)) {
        *un = std::make_shared<ast::Ident>(std::get<TokenIdent>(token).str);
        return yy::parser::token::ident;
    }

    if (std::holds_alternative<TokenKeyword>(token)) {
        auto keyword = std::string(std::get<TokenKeyword>(token).str);
        if (keyword == "fn") {
            return yy::parser::token::fn;
        }
        if (keyword == "if") {
            return yy::parser::token::If;
        }
        if (keyword == "else") {
            return yy::parser::token::Else;
        }
        if (keyword == "return") {
            return yy::parser::token::Return;
        }

        assert(false);
    }

    if (std::holds_alternative<TokenSpace>(token)) {
        switch (std::get<TokenSpace>(token)) {
        case SPACE_NEWLINE: return yy::parser::token::NEWLINE;
        case SPACE_INDENT: return yy::parser::token::INDENT;
        case SPACE_DEDENT: return yy::parser::token::DEDENT;
        default: assert(false); break;
        }
    }

    assert(false);
}

std::shared_ptr<ast::Node> parse(std::string& text) {
    tokens = lexTokens(text);
    tokenIndex = 0;

    yy::parser parser;
    parser.parse();

    if (bisonProgramResult) {
        std::shared_ptr<ast::Node> expr = bisonProgramResult;
        bisonProgramResult.reset();
        return expr;
    } else {
        assert(false);
    }
}
