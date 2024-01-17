// scanner.h
#ifndef scanner_h
#define scanner_h

typedef enum {
    // 单字符Token
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,		// '(', ')'
    TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,	// '[', ']'
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,  		// '{', '}'
    TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,	// ',', '.', ';'
    TOKEN_TILDE,  // '~'
    // 可能是单字符 也可能是双字符的Token
    TOKEN_PLUS, TOKEN_PLUS_PLUS, TOKEN_PLUS_EQUAL, // '+', '++', '+='
    // '-', '--', '-=', '->'
    TOKEN_MINUS, TOKEN_MINUS_MINUS, TOKEN_MINUS_EQUAL, TOKEN_MINUS_GREATER,
    TOKEN_STAR, TOKEN_STAR_EQUAL,			// '*', '*='
    TOKEN_SLASH, TOKEN_SLASH_EQUAL, 		// '/', '/=', 
    TOKEN_PERCENT, TOKEN_PERCENT_EQUAL, 	// '%', '%='
    TOKEN_AMPER, TOKEN_AMPER_EQUAL, TOKEN_AMPER_AMPER, 	// '&', '&=', '&&'
    TOKEN_PIPE, TOKEN_PIPE_EQUAL, TOKEN_PIPE_PIPE,		// '|', '|=', '||'
    TOKEN_HAT, TOKEN_HAT_EQUAL, 		// '^', '^='
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, 	// '=', '=='
    TOKEN_BANG, TOKEN_BANG_EQUAL,	  	// '!', '!='
    TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_LESS_LESS, 				// '<', '<=', '<<'
    TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_GREAER_GREATER, 	// '>', '>=', '>>'
    
    // >>= <<= 三字符Token都去掉了 你要愿意可以加上

    // 多字符Token
    // 字面值: 标识符, 字符字面值, 字符串字面值, 数字字面值
    TOKEN_IDENTIFIER, TOKEN_CHARACTER, TOKEN_STRING, TOKEN_NUMBER,
    // 关键字
    TOKEN_SIGNED, TOKEN_UNSIGNED,
    TOKEN_CHAR, TOKEN_SHORT, TOKEN_INT, TOKEN_LONG,
    TOKEN_FLOAT, TOKEN_DOUBLE,
    TOKEN_STRUCT, TOKEN_UNION, TOKEN_ENUM, TOKEN_VOID,
    TOKEN_IF, TOKEN_ELSE, TOKEN_SWITCH, TOKEN_CASE, TOKEN_DEFAULT,
    TOKEN_WHILE, TOKEN_DO, TOKEN_FOR,
    TOKEN_BREAK, TOKEN_CONTINUE, TOKEN_RETURN, TOKEN_GOTO,
    TOKEN_CONST, TOKEN_SIZEOF, TOKEN_TYPEDEF,
    // 辅助Token

    // TOKEN_ERROR 如果读到的Token不是上述任何一个类型的,那么就返回TOKEN_ERROR
    // TOKEN_ERROR只是用于报错信息的 并不会处理报错

    // TOKEN_EOF表示分析源代码到了末尾 结束标记
    TOKEN_ERROR, TOKEN_EOF  
} TokenType;

// 它是词法分析器生成的结果
typedef struct {
    TokenType type;     // Token的类型
    // start指向源代码中Token的起点字符
    const char* start;	// start指向source中的字符，source为读入的源代码。
    int length;		    // length表示这个Token的长度
    int line;		    // line表示这个Token在源代码的哪一行, 方便后面打印以及报错
} Token;

// 对 Scanner 进行初始化 
void initScanner(const char* source);

// 调用scanToken(), 返回下一个Token.
Token scanToken();

#endif