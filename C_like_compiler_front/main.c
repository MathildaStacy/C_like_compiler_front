// main.c
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>

#define REPL_INITIAL_CAP 10

// 类似测试用例,测试词法分析器

// 核心逻辑,打印效果都依赖于该函数
static void run(const char* source) {
	// 初始化词法分析器
	initScanner(source);
	int line = -1;  // 用来记录当前处理的代码行数,现在还没处理就是-1
	// 打印Token, 遇到TOKEN_EOF为止
	for (;;) {
		Token token = scanToken();  // 拍一下按钮,出一个Token
		if (token.line != line) {
			printf("%4d ", token.line);
			line = token.line;
		}
		else {
			printf("   | ");
		}
		/*
		* %.*s 转换说明
		* 不能写%s 打印字符串 那它就会打印到空字符
		* 如果用它 它会一直打印到文件末尾
		* %.2f 打印float保留2位小数
		* %.10s 这就表示只打印10个字符长度的字符串
		* %.*s  其中的*就是占位符,它表示取值对应参数
		*
		* 效果是:
		* 打印从token.start开始,长度为token.length的字符串 这个字符就是Token的具体字符串
		*/
		printf("%2d '%.*s'\n", token.type, token.length, token.start);

		if (token.type == TOKEN_EOF) break;     // 读完了 读到了末尾 词法分析结束了
	}
}

static void repl() {
	// 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
	// repl是"read evaluate print loop"的缩写
	// 1.需要死循环
	// 2.需要能一次性读一整行 fgets(, stdin) 该函数读到文件末尾EOF返回NULL
	// 3.最终需要调用run函数(run函数main.c中的核心逻辑) 它需要传入源代码字符串

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
	// 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
	// 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
	// 动态内存分配一个字符数组,长度 = 文件大小 + 1
	// 文件大小 : 文件指针移动最后,然后ftell返回的就是文件大小
	// 将文件中的字符数据一次性全部读到字符数组中
	// 最后不要忘记手动加上一个空字符
}

static void runFile(const char* path) {
	// 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
	char* source = readFile(path);  // 读取源文件,将整个源文件中的字符数据转换成一个字符串
	run(source);
	free(source);
}

int main(int argc, const char* argv[]) {
	if (argc == 1) {
		// ./scanner 没有参数,则进入交互式界面
		repl();
	}
	else if (argc == 2) {
		// ./scanner file 后面跟一个参数,则分析整个文件
		runFile(argv[1]);
	}
	else {
		fprintf(stderr, "Usage: scanner [path]\n");
		exit(1);
	}
	return 0;
}