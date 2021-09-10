#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

/*本程序对图书的ISBN的理解有错误*/

#define SIZE 20  //名称需要的空间大小
#define type_initial_size 20  //每一种书初始可存放量
#define type_size_increasement 10
#define bookstore_initial_size 200
#define bookstore_size_increasement 50

//每本书的信息
typedef struct {
	int ISBN;  //图书的ISBN号
	char* bookname;  //书名
	char* writer;  //图书作者
	char* publisher;  //图书出版商
	int price;  //图书价格
}BOOK;

//库存管理
typedef struct {
	char* bookname;
	int stock;  //该图书的库存量
}BOOK_STOCK;

//书店系统
typedef struct {
	BOOK* books;
	BOOK_STOCK* book_stock;
	int book_stock_capacity;
	int types;  //书的总类
	int number;  //书店图书总量
	int bookstore_capacity;  //书店容量
}Bookstore;

//图书管理系统的初始化
void initial(Bookstore& bookstore) {
	bookstore.books = (BOOK*)malloc(bookstore_initial_size * sizeof(BOOK));
	bookstore.book_stock = (BOOK_STOCK*)malloc(type_initial_size * sizeof(BOOK_STOCK));
	if (!bookstore.books || !bookstore.book_stock) {
		printf("初始化内存分配失败！\n");
		exit(-1);
	}
	bookstore.number = 0;
	bookstore.types = 0;
	bookstore.book_stock_capacity = type_initial_size;
	bookstore.bookstore_capacity = bookstore_initial_size;
}

//根据ISBN编号查找图书
int book_query_ISBN(Bookstore bookstore) {
	//若查找到，返回该书的下标；若未查找到，返回-1；
	int ISBN;
	printf("请输入ISBN编号：");
	scanf("%d", &ISBN);
	for (int i = 0; i < bookstore.number; i++) {
		if (bookstore.books[i].ISBN == ISBN) {
			return i;
		}
	}
	return -1;
}

//按照图书名称查找图书
int book_query_bookname(Bookstore bookstore, char* s, int& i) {
	//若查找到，返回该书的下标；若未查找到，返回-1；
	for (; i < bookstore.number; i++) {
		if (!strcmp(s, bookstore.books[i].bookname)) {  //查到图书，并返回
			i++;
			return i - 1;
		}
	}
	return -1;  //最后一本不是，但是也需要返回
}

//图书信息的添加
void book_add(Bookstore& bookstore) {
	if (bookstore.number >= bookstore.bookstore_capacity) {  //书店扩容
		bookstore.books = (BOOK*)realloc(bookstore.books, (bookstore.bookstore_capacity + bookstore_size_increasement) * sizeof(BOOK));
		if (!bookstore.books) {
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
	bookstore.books[bookstore.number++] = book;
	/*记录每类书的库存量*/
	if (bookstore.types > 0) {  //书店有书
		int i;
		for (i = 0; i < bookstore.types; i++) {
			if (!strcmp(bookstore.book_stock[i].bookname, book.bookname)) {  //有一样的书
				bookstore.book_stock[i].stock++;
				break;
			}
		}
		if (i >= bookstore.types) {  //没有一样的书
			if (bookstore.types >= bookstore.book_stock_capacity) {
				bookstore.book_stock = (BOOK_STOCK*)realloc(bookstore.book_stock, (bookstore.types + type_size_increasement) * sizeof(BOOK_STOCK));
				if (!bookstore.book_stock) {
					printf("存放图书种类地址空间分配失败！\n");
					exit(-1);
				}
			}
			bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
			bookstore.book_stock[bookstore.types].bookname = book.bookname;
			bookstore.book_stock[bookstore.types].stock = 1;
			bookstore.types++;
		}

	}
	else {  //书店没有书
		bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
		bookstore.book_stock[bookstore.types].bookname = book.bookname;
		bookstore.book_stock[bookstore.types].stock = 1;
		bookstore.types = 1;
	}
	printf("结果：添加成功。\n");
}

//图书信息的删除
void book_delete(Bookstore& bookstore) {
	printf("删除方式：\n1.按编号删除\n2.按书名删除\n****选择删除方式：");
	int choose;
	scanf("%d", &choose);
	switch (choose)
	{
	case 1:  //按编号删除图书
	{
		int result = book_query_ISBN(bookstore);  //查找图书
		if (result < 0) {
			printf("结果：未找到该图书！\n");
		}
		else {
			for (int i = 0; i < bookstore.types; i++) {  //对删除的图书的本数进行更新
				if (!strcmp(bookstore.book_stock[i].bookname, bookstore.books[result].bookname)) {  //该类图书删减一本
					bookstore.book_stock[i].stock--;
					if (!bookstore.book_stock[i].stock) {  //该类图书删尽
						for (int j = i; j < bookstore.types; j++) {
							bookstore.book_stock[j] = bookstore.book_stock[j + 1];
						}
						bookstore.types--;
						break;
					}
					break;
				}
			}
			for (int i = result; i < bookstore.number - 1; i++) {  //对书店总的图书信息进行更新
				bookstore.books[i] = bookstore.books[i + 1];
			}
			bookstore.number--;
			printf("结果：删除成功。\n");
		}
	}
	break;
	case 2:  //按书名删除图书
	{
		char* s = (char*)malloc(SIZE * sizeof(char));
		printf("输入要删除的书名：");
		scanf("%s", s);
		int record;  //要删除的图书在book_stock中的下标
		for (record = 0; record < bookstore.types; record++) {
			if (!strcmp(bookstore.book_stock[record].bookname, s)) {
				break;
			}
		}
		if (record == bookstore.types) {
			printf("结果：未找到该图书！\n");
		}
		else {
			int result;
			int i = 0;
			for (; i < bookstore.number;) {
				result = book_query_bookname(bookstore, s, i);  //每进行过一次就相当于找到一本要删除的图书
				if (result >= 0) {  //找到图书并删除，后面的图书需要向前移
					for (int j = result; j < bookstore.number; j++) {
						bookstore.books[j] = bookstore.books[j + 1];
					}
					bookstore.book_stock[record].stock--;
					if (!bookstore.book_stock[record].stock) {  //该类图书删尽
						for (int j = record; j < bookstore.types; j++) {
							bookstore.book_stock[j] = bookstore.book_stock[j + 1];
						}
						bookstore.types--;
					}
					bookstore.number--;
					i = 0;
				}
			}
			printf("结果：删除成功。\n");
		}
	}
	break;
	default:
		break;
	}
}

//查看所有图书信息
void see_all_books(Bookstore& bookstore) {
	if (!bookstore.number) {
		printf("结果：书店是个空书店！\n");
	}
	for (int i = 0; i < bookstore.number; i++) {
		BOOK book = bookstore.books[i];
		int j;
		for (j = 0; j < bookstore.types; j++) {
			if (!strcmp(bookstore.book_stock[j].bookname, book.bookname)) {
				break;
			}
		}
		if (i == 0) {
			printf("结果为：\n");
		}
		printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[j].stock);
	}
}

//从外存读入图书信息（进入上一次存档点）
void read_from_external_deposit(Bookstore& bookstore) {
	FILE* fp1 = fopen("c:\\users\\刘汝杨\\desktop\\书店全部图书信息.txt", "r");
	FILE* fp2 = fopen("c:\\users\\刘汝杨\\desktop\\图书库存信息.txt", "r");
	if (!fp1 || !fp1) {
		printf("结果：数据读取失败！（可能是文件不存在）");
	}
	else {
		rewind(fp1);
		rewind(fp2);
		bookstore.types = 0;
		bookstore.number = 0;
		char ch;
		while ((ch = fgetc(fp2)) != '\n');  //从文档第二行开始读取数据
		while (!feof(fp2)) {
			bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
			fscanf(fp2, "%s%d", bookstore.book_stock[bookstore.types].bookname, &(bookstore.book_stock[bookstore.types].stock));
			bookstore.types++;
		}
		bookstore.types--;
		while ((ch = fgetc(fp1)) != '\n');  //从文档第二行开始读取数据
		while (!feof(fp1)) {
			bookstore.books[bookstore.number].bookname = (char*)malloc(SIZE * sizeof(char));
			bookstore.books[bookstore.number].writer = (char*)malloc(SIZE * sizeof(char));
			bookstore.books[bookstore.number].publisher = (char*)malloc(SIZE * sizeof(char));
			fscanf(fp1, "%d%s%s%s%d", &bookstore.books[bookstore.number].ISBN, bookstore.books[bookstore.number].bookname, bookstore.books[bookstore.number].writer, bookstore.books[bookstore.number].publisher, &bookstore.books[bookstore.number].price);
			bookstore.number++;
		}
		bookstore.number--;
		fclose(fp1);
		fclose(fp2);
		printf("结果：图书信息读取成功。\n");
	}
}

//存档
void book_update(Bookstore bookstore) {
	FILE* fp1 = fopen("c:\\users\\刘汝杨\\desktop\\书店全部图书信息.txt", "w+");
	FILE* fp2 = fopen("c:\\users\\刘汝杨\\desktop\\图书库存信息.txt", "w+");
	rewind(fp1);
	rewind(fp2);
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
	fprintf(fp1, "%-8s%-40s%-40s%-40s%-20s\n", ISBN, bookname, writer, publisher, price);
	for (int i = 0; i < bookstore.number; i++) {
		fprintf(fp1, "%-8d%-40s%-40s%-40s%-20d\n", bookstore.books[i].ISBN, bookstore.books[i].bookname, bookstore.books[i].writer, bookstore.books[i].publisher, bookstore.books[i].price);
	}
	fprintf(fp2, "%-40s%-20s\n", bookname, stock);
	for (int i = 0; i < bookstore.types; i++) {
		fprintf(fp2, "%-40s%-20d\n", bookstore.book_stock[i].bookname, bookstore.book_stock[i].stock);
	}
	fclose(fp1);
	fclose(fp2);
	printf("结果：图书信息存档成功。\n");
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
		case 1://查找
		{
			int choose2;
			printf("查询方式：\n1.查询ISBN编号\n2.查询图书名称\n****选择查找方式（输入序号）：");
			scanf("%d", &choose2);
			switch (choose2) {
			case 1:  //按编号查找
			{
				int result = book_query_ISBN(bookstore);
				if (result >= 0) {
					BOOK book = bookstore.books[result];
					int i;
					for (i = 0; i < bookstore.types; i++) {
						if (!strcmp(bookstore.book_stock[i].bookname, book.bookname)) {
							break;
						}
					}
					printf("结果：\n");
					printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[i].stock);
				}
				else {
					printf("结果：未找到该图书！\n");
				}
			}
			break;
			case 2:  //按书名查找
			{
				BOOK book;
				char* s = (char*)malloc(SIZE * sizeof(char));
				printf("输入要查找的图书名：");
				scanf("%s", s);
				int result;
				int record = 0;  //用于记录已经打印的图书本数
				for (int i = 0; i < bookstore.number;) {
					result = book_query_bookname(bookstore, s, i);
					if (result >= 0) {
						book = bookstore.books[result];
						int i;
						for (i = 0; i < bookstore.types; i++) {
							if (!strcmp(bookstore.book_stock[i].bookname, book.bookname)) {
								break;
							}
						}
						if (!record) {
							printf("结果：\n");
						}
						printf("ISBN：%-8d  书名：%-20s  作者：%-10s  出版社：%-10s  价格：%-5d  库存量：%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[i].stock);
						record++;
					}
				}
				if (!record) {
					printf("结果：未找到该图书！\n");
				}
			}
			default:
				break;
			}
		}
		break;
		case 2://添加
			book_add(bookstore);
			break;
		case 3://删除
			book_delete(bookstore);
			break;
		case 4://查看所有图书信息
			see_all_books(bookstore);
			break;
		case 5://存档
			book_update(bookstore);
			break;
		case 6://从外存读取信息，进入上一次存档点
			read_from_external_deposit(bookstore);
			break;
		default:
			break;
		}
		printf("\n\n1.查询图书信息\n2.添加图书\n3.删除图书\n4.查看所有图书信息\n5.存档\n6.从外存读取书店信息，进入上一次存档点\n(☆按0可结束操作）\n***继续选择执行功能：");
		scanf("%d", &choose);
	}
	printf("\n\n  操作结束。");
	return 0;
}

