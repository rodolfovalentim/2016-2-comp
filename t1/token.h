#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
	ELSE,
	IF,	
	INPUT,
	INT,
	OUTPUT,
	RETURN,
	VOID,
	WHILE,
	WRITE,
	PLUS,
	MINUS,
	TIMES,
	OVER,
	LT,
	LE,
	GT,
	GE,
	EQ,
	NEQ,
	ASSIGN,
	SEMI,
	COMMA,
	LPAREN,
	RPAREN,
	LBRACK,
	RBRACK,
	LBRACE,
	RBRACE,
	NUM,
	ID,
	STRING
} TokenType;

static const char *TOKEN_TYPE_STRING[] = {
	"ELSE",
	"IF",
	"INPUT",
	"INT",
	"OUTPUT",
	"RETURN",
	"VOID",
	"WHILE",
	"WRITE",
	"PLUS",
	"MINUS",
	"TIMES",
	"OVER",
	"LT",
	"LE",
	"GT",
	"GE",
	"EQ",
	"NEQ",
	"ASSIGN",
	"SEMI",
	"COMMA",
	"LPAREN",
	"RPAREN",
	"LBRACK",
	"RBRACK",
	"LBRACE",
	"RBRACE",
	"NUM",
	"ID",
	"STRING"
};

#endif // TOKEN_H
