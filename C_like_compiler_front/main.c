// main.c
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>

#define REPL_INITIAL_CAP 10

// ���Ʋ�������,���Դʷ�������

// �����߼�,��ӡЧ���������ڸú���
static void run(const char* source) {
	// ��ʼ���ʷ�������
	initScanner(source);
	int line = -1;  // ������¼��ǰ����Ĵ�������,���ڻ�û�������-1
	// ��ӡToken, ����TOKEN_EOFΪֹ
	for (;;) {
		Token token = scanToken();  // ��һ�°�ť,��һ��Token
		if (token.line != line) {
			printf("%4d ", token.line);
			line = token.line;
		}
		else {
			printf("   | ");
		}
		/*
		* %.*s ת��˵��
		* ����д%s ��ӡ�ַ��� �����ͻ��ӡ�����ַ�
		* ������� ����һֱ��ӡ���ļ�ĩβ
		* %.2f ��ӡfloat����2λС��
		* %.10s ��ͱ�ʾֻ��ӡ10���ַ����ȵ��ַ���
		* %.*s  ���е�*����ռλ��,����ʾȡֵ��Ӧ����
		*
		* Ч����:
		* ��ӡ��token.start��ʼ,����Ϊtoken.length���ַ��� ����ַ�����Token�ľ����ַ���
		*/
		printf("%2d '%.*s'\n", token.type, token.length, token.start);

		if (token.type == TOKEN_EOF) break;     // ������ ������ĩβ �ʷ�����������
	}
}

static void repl() {
	// ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
	// repl��"read evaluate print loop"����д
	// 1.��Ҫ��ѭ��
	// 2.��Ҫ��һ���Զ�һ���� fgets(, stdin) �ú��������ļ�ĩβEOF����NULL
	// 3.������Ҫ����run����(run����main.c�еĺ����߼�) ����Ҫ����Դ�����ַ���

	int length = 0;
	int ch = 0;
	int capacity = REPL_INITIAL_CAP;

	char* input = calloc(sizeof(char), REPL_INITIAL_CAP);
	char* new_input = NULL;
	if(input == NULL)
	{
		fprintf(stderr, "memory allocation failed in func calloc of function repl.");
		exit(-1);
	}

	while (1)
	{
		printf("C-like repl>");

		int capacity = REPL_INITIAL_CAP;

		char* input = calloc(sizeof(char), REPL_INITIAL_CAP);
		char* new_input = NULL;
		if (input == NULL)
		{
			fprintf(stderr, "memory allocation failed in func calloc of function repl.");
			exit(-1);
		}


		


		while ((ch = getchar()) != '\n' && ch != EOF)
		{
			if(length == capacity - 1)
			{
				capacity *= 2;

				new_input = realloc(input, capacity * sizeof(char));

				if(input == NULL)
				{
					free(input);
					fprintf(stderr, "memory allocation failed in func realloc of function repl.");
					exit(-1);
				}

				input = new_input;
			}

			input[length++] = ch;
		}

		if(ch == EOF)
		{
			exit(1);
		}

		input[length] = '\0';

		run(input);

		free(input);

	}
}

static char* readFile(const char* path) {
	// �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
	// ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
	// ��̬�ڴ����һ���ַ�����,���� = �ļ���С + 1
	// �ļ���С : �ļ�ָ���ƶ����,Ȼ��ftell���صľ����ļ���С
	// ���ļ��е��ַ�����һ����ȫ�������ַ�������
	// ���Ҫ�����ֶ�����һ�����ַ�
}

static void runFile(const char* path) {
	// ����'.c'�ļ�:�û������ļ��������������ļ�������������
	char* source = readFile(path);  // ��ȡԴ�ļ�,������Դ�ļ��е��ַ�����ת����һ���ַ���
	run(source);
	free(source);
}

int main(int argc, const char* argv[]) {
	if (argc == 1) {
		// ./scanner û�в���,����뽻��ʽ����
		repl();
	}
	else if (argc == 2) {
		// ./scanner file �����һ������,����������ļ�
		runFile(argv[1]);
	}
	else {
		fprintf(stderr, "Usage: scanner [path]\n");
		exit(1);
	}
	return 0;
}