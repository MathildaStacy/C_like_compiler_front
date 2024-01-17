// scanner.c
#include "scanner.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	const char* start;  // �ʷ��������ڷ���ĳ��Tokenʱ,startʼ��ָ��Token�ĵ�һ���ַ�
	const char* current;    // �ʷ�������������Token��, ������Ҫ��ȡ��һ���ַ�, ��������ǰ���ڶ�ȡ���ַ�
	int line;   // ��ǰ���ڷ�����Token������
} Scanner;

// ȫ�ֱ���
Scanner scanner;

void initScanner(const char* source) {
	// ��ʼ��scannerȫ�ֱ���
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

/***************************************************************************************
 *                                   ��������									       *
 ***************************************************************************************/

// �ж��ַ�c�ǲ�����ĸ�����»���
static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

// �ж��ַ�c�ǲ�������
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

// �ж���һ��Ҫ������ַ��ǲ���Դ����ĩβ
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// �ȷ�����һ���ַ�,Ȼ��currǰ��һ���ֽ�
static char advance() {
	return *scanner.current++;
}

// Ƴһ����һ���ַ�,������
static char peek() {
	return *scanner.current;
}

// �����һ���ַ�����Դ����ĩβ,�Ǿ�ȥƳһ������һ���ַ�,������
static char peekNext() {
	if (isAtEnd()) return '\0';
	return *(scanner.current + 1);
}

// �����һ���ַ�����Դ����ĩβ,�Ǿ�Ƴһ����,�ж�һ�����ǲ���Ԥ�ڵ��ַ�
static bool match(char expected) {
	if (isAtEnd()) return false;
	if (peek() != expected) return false;
	scanner.current++;
	return true;
}

// ����TokenType, ������Ӧ���͵�Token�������ء�
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;	// �Ѵʷ����������ڴ����Token�����ַ���ֵ��token.start
	token.length = (int)(scanner.current - scanner.start);	// ָ����� ����Token�ĳ���
	token.line = scanner.line;
	return token;
}

// �������ܽ��������ʱ��
// ���Ǵ���һ��ERROR Token. ���磺����@��$�ȷ���ʱ�������ַ������ַ�û�ж�Ӧ��������ʱ��
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}
static void skipWhitespace() {
	// �����հ��ַ�: ' ', '\r', '\t', '\n'��ע��
	// ע����'//'��ͷ, һֱ����β  (����ע�Ͳ���)
	// ע�����scanner.line��

	/*
		˼·:
		1.��Ҫ��ѭ��
		2.Ƴһ����һ���ַ�,��������ʲô,������ͬ�Ĵ���,switch
		3.������ô��? advance
		4.ע��: ������һ���ַ���/,������Ƴ����һ���������/ ��ô���Ǵ���ע�� ѭ������ û�л��о�ȫ������
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
// ����˵����
// start: �������ַ����е����
// length: �������ַ����еĳ���
// rest: ������Token��ʣ�ಿ��(��Ϊ��ͷ�Ѿ���switch�жϹ���)
// type: �����ȫƥ�䣬��˵����type���͵�Token
// ���ڼ�鵱ǰ�����Token�ǲ��Ǵ����type����
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
	int len = (int)(scanner.current - scanner.start);	// Token�ĳ���
	// �ȱȽϳ����Ƿ���� Ȼ�����ж������Ƿ�һ��
	// ����start��ʼ, ����Ϊlength���ַ������Ƿ��������rest�ַ���һ��
	if (start + length == len && memcmp(scanner.start + start, rest, length) == 0) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}

// �жϵ�ǰToken�����Ǳ�ʶ�����ǹؼ���
static TokenType identifierType() {
	char c = scanner.start[0];	// ��ȡ��ǰToken�����ַ�
	// ��switch���ʵ��Trie��
	switch (c) {
	case 'b': return checkKeyword(1, 4, "reak", TOKEN_BREAK);
		... // TODO
	}

	// identifier
	return TOKEN_IDENTIFIER;
}
static Token identifier() {
	// IDENTIFIER����: ��ĸ�����ֺ��»���
	while (isAlpha(peek()) || isDigit(peek())) {
		advance();	// ֻҪ��һ���ַ�������ĸ/�»���/����,�ͼ�������
	}	// ˵����һ���ַ��Ѿ��Ƿ�����/��ĸ/�»����� currָ��ָ��Token����һ���ַ�
	// ������Token�����Ǳ�ʶ��, Ҳ�����ǹؼ���, identifierType()������ȷ��Token���͵�
	// break
	return makeToken(identifierType());
}

static Token number() {
	// ����������ǽ�NUMBER�Ĺ���������:
	// 1. NUMBER���԰������ֺ����һ��'.'��
	// 2. '.'��ǰ��Ҫ������(���C���Բ�ͬ)
	// 3. '.'�ź���ҲҪ������
	// ��Щ���ǺϷ���NUMBER: 123, 3.14
	// ��Щ���ǲ��Ϸ���NUMBER: 123., .14
	// ����123.456 sc.startָ��1, currָ��ҪŲ��6����ĵ�һ���������ַ�
}

static Token string() {
	// �ַ�����"��ͷ����"��β�����Ҳ��ܿ���
	// �����һ���ַ�����ĩβҲ����˫����,�Ǿ�һֱ����
}

static Token character() {
	// �ַ�'��ͷ����'��β�����Ҳ��ܿ���   
}

/***************************************************************************************
 *                                   	�ִ�											  *
 ***************************************************************************************/
Token scanToken() {
	// ����ǰ�ÿհ��ַ���ע��
	skipWhitespace();// ��skipWhitespace��������,���������һ���ַ����ǿհ�Ҳ����ע����Щ�ַ�,��ֱ�ӷ�����
	// ��¼��һ��Token����ʼλ��
	scanner.start = scanner.current;	// currָ�����Token�ĵ�һ���ַ�	
	
	// �����һ���ַ���Դ�ļ�ĩβ, �Ǿͷ��ؽ���Token
	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();	// c�ǵ�ǰ������Token�ĵ�һ���ַ�, curr�Ѿ�ָ���˵ڶ����ַ�
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
	case '"': return string();	// �ַ�������ֵ
	case '\'': return character();	// �ַ�����ֵ
	}

	return errorToken("Unexpected character.");
}