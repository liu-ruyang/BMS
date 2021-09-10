#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

#define bookstore_initial_size  200
#define SIZE 20
#define bookstore_size_increasement 50

//每一个ISBN对应的图书信息
typedef struct {
	int ISBN;  //书的ISBN编号
	char* bookname;  //书名
	char* writer;  //书的作者
	char* publisher;  //图书出版商
	int price;  //图书的价格
	int stock;  //该编号的图书的库存量
}BOOK;

typedef struct {
	BOOK* books;
	int types;  //当前书店的ISBN的种类
	int number;  //当前书店书的总量
	int bookstore_capacity;  //书店容量
}Bookstore;

//书店初始化
void initial(Bookstore& bookstore) {
	bookstore.books = (BOOK*)malloc(bookstore_initial_size * sizeof(BOOK));
	if (!bookstore.books) {
		printf("初始化内存分配失败！\n");
		exit(-1);
	}
	bookstore.number = bookstore.types = 0;
	bookstore.bookstore_capacity = bookstore_initial_size;
}

//根据书的ISBN编号查找图书
int book_query_ISBN(Bookstore bookstore) {
	int ISBN;
	printf("请输入ISBN编号：");
	scanf("%d", &ISBN);
	for (int i = 0; i < bookstore.types; i++) {
		if (bookstore.books[i].ISBN == ISBN) {
			return i;
		}
	}
	return -1;
}

//根据书名查找图书
int book_query_bookname(Bookstore bookstore, char* s, int& i) {
	//若查找到，返回该书的下标；若未查找到，返回-1；
	for (; i < bookstore.types; i++) {
		if (!strcmp(s, bookstore.books[i].bookname)) {  //查到图书，并返回
			i++;
			return i - 1;
		}
	}
	return -1;  //最后一本不是，但是也需要返回
}

//添加图书
void book_add(Bookstore& bookstore) {
	if (bookstore.number >= bookstore.bookstore_capacity) {
		if ((bookstore.books = (BOOK*)realloc(bookstore.books, (bookstore.bookstore_capacity + bookstore_size_increasement) * sizeof(BOOK)))) {
			printf("书店扩容失败！\n");
			exit(-1);
		}
	}
	BOOK book;
	book.bookname = (char*)malloc(SIZE * sizeof(char));
	book.writer = (char*)malloc(SIZE * sizeof(char));
	book.publisher = (char*)malloc(SIZE * sizeof(char));
	printf("依次输入添加图书的ISBN、书名、作者、出版商以及价格（以空格隔开）：");
	scanf("%d%s%s%s%d", &book.ISBN, book.bookname, book.writer, book.publisher, &book.price);
	int i;
	for (i = 0; i < bookstore.types; i++) {
		if (bookstore.books[i].ISBN == book.ISBN) {  //原先已存在该编号的书
			bookstore.books[i].stock++;
			bookstore.number++;
			break;
		}
	}
	if (i >= bookstore.types) {  //原先没有该编号的书，需要新添加
		bookstore.books[bookstore.types] = book;
		bookstore.books[bookstore.types].stock = 1;
		bookstore.types++;
		bookstore.number++;
	}
	printf("图书添加成功。\n");
}

//删除图书
void book_delete(Bookstore& bookstore) {
	printf("删除方式：\n1.按编号删除\n2.按书名删除\n****选择删除方式：");
	int choose;
	scanf("%d", &choose);
	switch (choose) {
	case 1://根据ISBN删除
	{
		int result = book_query_ISBN(bookstore);
		if (result < 0) {
			printf("结果：未找到该图书！\n");
		}
		else {
			bookstore.number -= bookstore.books[result].stock;
			for (int i = result; i < bookstore.types; i++) {
				bookstore.books[i] = bookstore.books[i + 1];
			}
			bookstore.types--;
			printf("结果：删除成功。\n");
		}
	}
	break;
	case 2:  //按书名删除图书                                 /**/
	{
		char* bookname = (char*)malloc(SIZE * sizeof(char));
		printf("输入要删除的书名：");
		scanf("%s", bookname);
		int record = 0;
		for (int i = 0; i < bookstore.types;) {
			int result = book_query_bookname(bookstore, bookname, i);
			if (result >= 0) {
				bookstore.number -= bookstore.books[result].stock;
				for (int j = result; j < bookstore.types; j++) {
					bookstore.books[j] = bookstore.books[j + 1];
				}
				bookstore.types--;
				i = 0;
				record = 1;
			}
		}
		if (!record) {
			printf("结果：未找到该图书！\n");
		}
		else {
			printf("结果：删除成功。\n");
		}
	}
	default:
		break;
	}
}

//查看所有图书的信息
void see_all_books(Bookstore bookstore) {
	if (!bookstore.number) {
		printf("结果：书店是个空书店！\n");
	}
	for (int i = 0; i < bookstore.types; i++) {
		BOOK book = bookstore.books[i];
		if (i == 0) {
			printf("结果为：\n");
		}
		printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
	}
}

//存档
void save(Bookstore bookstore) {
	FILE* fp1 = fopen("c:\\users\\刘汝杨\\desktop\\书店全部图书信息.txt", "w+");
	rewind(fp1);
	const char* ISBN = (char*)malloc(SIZE * sizeof(char));
	ISBN = "ISBN";
	const char* bookname = (char*)malloc(SIZE * sizeof(char));
	bookname = "书名";
	const char* writer = (char*)malloc(SIZE * sizeof(char));
	writer = "作者";
	const char* publisher = (char*)malloc(SIZE * sizeof(char));
	publisher = "出版商";
	const char* price = (char*)malloc(SIZE * sizeof(char));
	price = "价格";
	const char* stock = (char*)malloc(SIZE * sizeof(char));
	stock = "库存量";
	fprintf(fp1, "%-8s\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-20s\t\t\t\t\t%-20s\t\t\t\t\t\n", ISBN, bookname, writer, publisher, price, stock);
	for (int i = 0; i < bookstore.types; i++) {
		fprintf(fp1, "%-8d%-40s%-40s%-40s%-20d%-20d\n", bookstore.books[i].ISBN, bookstore.books[i].bookname, bookstore.books[i].writer, bookstore.books[i].publisher, bookstore.books[i].price, bookstore.books[i].stock);
	}
	fclose(fp1);
	printf("结果：图书信息存档成功。\n");
}

//读档
void read_from_external_deposit(Bookstore& bookstore) {
	FILE* fp1 = fopen("c:\\users\\刘汝杨\\desktop\\书店全部图书信息.txt", "r");
	if (!fp1) {
		printf("结果：数据读取失败！（可能是文件不存在）");
	}
	else {
		rewind(fp1);
		bookstore.types = bookstore.number = 0;
		char ch;
		while ((ch = fgetc(fp1)) != '\n');  //从文档第二行开始读取数据
		while (!feof(fp1)) {
			bookstore.books[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
			bookstore.books[bookstore.types].writer = (char*)malloc(SIZE * sizeof(char));
			bookstore.books[bookstore.types].publisher = (char*)malloc(SIZE * sizeof(char));
			fscanf(fp1, "%8d%40s%40s%40s%20d%20d\n", &bookstore.books[bookstore.types].ISBN, bookstore.books[bookstore.types].bookname, bookstore.books[bookstore.types].writer, bookstore.books[bookstore.types].publisher, &bookstore.books[bookstore.types].price, &bookstore.books[bookstore.types].stock);
			bookstore.number += bookstore.books[bookstore.types].stock;
			bookstore.types++;
		}
		if (bookstore.number < 0) {
			bookstore.types = 0;
			bookstore.number = 0;
		}
		fclose(fp1);
		printf("结果：图书信息读取成功。\n");
	}
}

int main() {
	Bookstore bookstore;
	initial(bookstore);
	printf("\n成功进入书店管理系统！\n\n\n");
	int choose;
	printf("1.查询图书信息\n2.添加图书\n3.删除图书\n4.查看所有图书信息\n5.存档\n6.从外存读取书店信息，进入上一次存档点\n(☆按0可结束操作）\n****选择执行功能：");
	scanf("%d", &choose);
	while (choose) {
		switch (choose)
		{
		case 1:  //查找
		{
			int choose2;
			printf("查询方式：\n1.查询ISBN编号\n2.查询图书名称\n****选择查找方式（输入序号）：");
			scanf("%d", &choose2);
			switch (choose2)
			{
			case 1:  //按编号查找
			{
				int result = book_query_ISBN(bookstore);
				if (result >= 0) {  //找到该编号的图书
					BOOK book = bookstore.books[result];
					printf("结果：\n");
					printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
				}
				else {
					printf("结果：未找到该图书！\n");
				}
			}
			break;
			case 2:  //根据书名查找
			{
				BOOK book;
				char* s = (char*)malloc(SIZE * sizeof(char));
				printf("输入要查找的图书名：");
				scanf("%s", s);
				int result;
				int record = 0;  //用于记录已经打印的图书本数
				for (int i = 0; i < bookstore.types; ) {
					result = book_query_bookname(bookstore, s, i);
					if (result >= 0) {
						book = bookstore.books[result];
						if (!record) {
							printf("结果：\n");
						}
						printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
						record++;
					}
				}
				if (!record) {
					printf("结果：未找到该图书！\n");
				}
			}
			break;
			default:
				break;
			}
		}
		break;
		case 2: //添加图书
			book_add(bookstore);
			break;
		case 3:  //删除图书
			book_delete(bookstore);
			break;
		case 4:  //查看所有图书的信息
			see_all_books(bookstore);
			break;
		case 5:  //存档
			save(bookstore);
			break;
		case 6:  //读档
			read_from_external_deposit(bookstore);
			break;
		default:
			break;
		}
		printf("\n\n1.查询图书信息\n2.添加图书\n3.删除图书\n4.查看所有图书信息\n5.存档\n6.从外存读取书店信息，进入上一次存档点\n(☆按0可结束操作）\n***继续选择执行功能：");
		scanf("%d", &choose);
	}
	printf("\n\n****操作结束。****\n\n");
	return 0;
}