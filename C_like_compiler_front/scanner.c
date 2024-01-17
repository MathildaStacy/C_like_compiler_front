// scanner.c
#include "scanner.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	const char* start;  // 词法分析器在分析某个Token时,start始终指向Token的第一个字符
	const char* current;    // 词法分析器分析的Token中, 接下来要读取的一个字符, 不是它当前正在读取的字符
	int line;   // 当前正在分析的Token的行数
} Scanner;

// 全局变量
Scanner scanner;

void initScanner(const char* source) {
	// 初始化scanner全局变量
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

/***************************************************************************************
 *                                   辅助方法									       *
 ***************************************************************************************/

// 判断字符c是不是字母或者下划线
static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

// 判断字符c是不是数字
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

// 判断下一个要处理的字符是不是源代码末尾
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// 先返回下一个字符,然后curr前进一个字节
static char advance() {
	return *scanner.current++;
}

// 瞥一眼下一个字符,返回它
static char peek() {
	return *scanner.current;
}

// 如果下一个字符不是源代码末尾,那就去瞥一眼下下一个字符,返回它
static char peekNext() {
	if (isAtEnd()) return '\0';
	return *(scanner.current + 1);
}

// 如果下一个字符不是源代码末尾,那就瞥一眼它,判断一下它是不是预期的字符
static bool match(char expected) {
	if (isAtEnd()) return false;
	if (peek() != expected) return false;
	scanner.current++;
	return true;
}

// 传入TokenType, 创建对应类型的Token，并返回。
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;	// 把词法分析器正在处理的Token的首字符赋值给token.start
	token.length = (int)(scanner.current - scanner.start);	// 指针减法 计算Token的长度
	token.line = scanner.line;
	return token;
}

// 遇到不能解析的情况时，
// 我们创建一个ERROR Token. 比如：遇到@，$等符号时，比如字符串，字符没有对应的右引号时。
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}
static void skipWhitespace() {
	// 跳过空白字符: ' ', '\r', '\t', '\n'和注释
	// 注释以'//'开头, 一直到行尾  (多行注释不做)
	// 注意更新scanner.line！

	/*
		思路:
		1.需要死循环
		2.瞥一眼下一个字符,根据它是什么,做出不同的处理,switch
		3.跳过怎么跳? advance
		4.注释: 假如下一个字符是/,继续再瞥下下一个如果还是/ 那么就是处理注释 循环跳过 没有换行就全部跳过
	*/

	if((*(scanner.current)) == '\n')
	{
		scanner.current++;
		scanner.line++;
	}
	else if(isspace(*(scanner.current)))
	{
		scanner.current++;
	}
	else if(*(scanner.current) == '/' && *(scanner.current + 1) == '/')
	{
		while(*(scanner.current) != '\n')
		{
			scanner.current++;
		}
	}
}
// 参数说明：
// start: 待检查的字符序列的起点
// length: 待检查的字符序列的长度
// rest: 待检查的Token的剩余部分(因为开头已经用switch判断过了)
// type: 如果完全匹配，则说明是type类型的Token
// 用于检查当前处理的Token是不是传入的type类型
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
	int len = (int)(scanner.current - scanner.start);	// Token的长度
	// 先比较长度是否相等 然后再判断内容是否一致
	// 检查从start开始, 长度为length的字符序列是否与给定的rest字符串一致
	if (start + length == len && memcmp(scanner.start + start, rest, length) == 0) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}

// 判断当前Token到底是标识符还是关键字
static TokenType identifierType() {
	char c = scanner.start[0];	// 获取当前Token的首字符
	// 用switch语句实现Trie树
	switch (c) {
	case 'b': return checkKeyword(1, 4, "reak", TOKEN_BREAK);
		... // TODO
	}

	// identifier
	return TOKEN_IDENTIFIER;
}
static Token identifier() {
	// IDENTIFIER包含: 字母，数字和下划线
	while (isAlpha(peek()) || isDigit(peek())) {
		advance();	// 只要下一个字符还是字母/下划线/数字,就继续跳过
	}	// 说明下一个字符已经是非数字/字母/下划线了 curr指针指向Token的下一个字符
	// 这样的Token可能是标识符, 也可能是关键字, identifierType()是用来确定Token类型的
	// break
	return makeToken(identifierType());
}

static Token number() {
	// 简单起见，我们将NUMBER的规则定义如下:
	// 1. NUMBER可以包含数字和最多一个'.'号
	// 2. '.'号前面要有数字(这和C语言不同)
	// 3. '.'号后面也要有数字
	// 这些都是合法的NUMBER: 123, 3.14
	// 这些都是不合法的NUMBER: 123., .14
	// 比如123.456 sc.start指向1, curr指针要挪到6后面的第一个非数字字符
}

static Token string() {
	// 字符串以"开头，以"结尾，而且不能跨行
	// 如果下一个字符不是末尾也不是双引号,那就一直跳过
}

static Token character() {
	// 字符'开头，以'结尾，而且不能跨行   
}

/***************************************************************************************
 *                                   	分词											  *
 ***************************************************************************************/
Token scanToken() {
	// 跳过前置空白字符和注释
	skipWhitespace();// 在skipWhitespace函数当中,如果发现下一个字符不是空白也不是注释这些字符,就直接返回了
	// 记录下一个Token的起始位置
	scanner.start = scanner.current;	// curr指向的是Token的第一个字符	
	
	// 如果下一个字符是源文件末尾, 那就返回结束Token
	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();	// c是当前解析的Token的第一个字符, curr已经指向了第二个字符
	if (isAlpha(c)) return identifier();
	if (isDigit(c)) return number();

	switch (c) {
		// single-character tokens
	case '(': return makeToken(TOKEN_LEFT_PAREN);
		...	// TODO

		// one or two characters tokens
	case '+':
		if (match('+')) return makeToken(TOKEN_PLUS_PLUS);
		else if (match('=')) return makeToken(TOKEN_PLUS_EQUAL);
		else return makeToken(TOKEN_PLUS);
		... // TODO

		// various-character tokens
	case '"': return string();	// 字符串字面值
	case '\'': return character();	// 字符字面值
	}

	return errorToken("Unexpected character.");
}