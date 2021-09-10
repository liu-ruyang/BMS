#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

#define bookstore_initial_size  200
#define SIZE 20
#define bookstore_size_increasement 50

//ÿһ��ISBN��Ӧ��ͼ����Ϣ
typedef struct {
	int ISBN;  //���ISBN���
	char* bookname;  //����
	char* writer;  //�������
	char* publisher;  //ͼ�������
	int price;  //ͼ��ļ۸�
	int stock;  //�ñ�ŵ�ͼ��Ŀ����
}BOOK;

typedef struct {
	BOOK* books;
	int types;  //��ǰ����ISBN������
	int number;  //��ǰ����������
	int bookstore_capacity;  //�������
}Bookstore;

//����ʼ��
void initial(Bookstore& bookstore) {
	bookstore.books = (BOOK*)malloc(bookstore_initial_size * sizeof(BOOK));
	if (!bookstore.books) {
		printf("��ʼ���ڴ����ʧ�ܣ�\n");
		exit(-1);
	}
	bookstore.number = bookstore.types = 0;
	bookstore.bookstore_capacity = bookstore_initial_size;
}

//�������ISBN��Ų���ͼ��
int book_query_ISBN(Bookstore bookstore) {
	int ISBN;
	printf("������ISBN��ţ�");
	scanf("%d", &ISBN);
	for (int i = 0; i < bookstore.types; i++) {
		if (bookstore.books[i].ISBN == ISBN) {
			return i;
		}
	}
	return -1;
}

//������������ͼ��
int book_query_bookname(Bookstore bookstore, char* s, int& i) {
	//�����ҵ������ظ�����±ꣻ��δ���ҵ�������-1��
	for (; i < bookstore.types; i++) {
		if (!strcmp(s, bookstore.books[i].bookname)) {  //�鵽ͼ�飬������
			i++;
			return i - 1;
		}
	}
	return -1;  //���һ�����ǣ�����Ҳ��Ҫ����
}

//���ͼ��
void book_add(Bookstore& bookstore) {
	if (bookstore.number >= bookstore.bookstore_capacity) {
		if ((bookstore.books = (BOOK*)realloc(bookstore.books, (bookstore.bookstore_capacity + bookstore_size_increasement) * sizeof(BOOK)))) {
			printf("�������ʧ�ܣ�\n");
			exit(-1);
		}
	}
	BOOK book;
	book.bookname = (char*)malloc(SIZE * sizeof(char));
	book.writer = (char*)malloc(SIZE * sizeof(char));
	book.publisher = (char*)malloc(SIZE * sizeof(char));
	printf("�����������ͼ���ISBN�����������ߡ��������Լ��۸��Կո��������");
	scanf("%d%s%s%s%d", &book.ISBN, book.bookname, book.writer, book.publisher, &book.price);
	int i;
	for (i = 0; i < bookstore.types; i++) {
		if (bookstore.books[i].ISBN == book.ISBN) {  //ԭ���Ѵ��ڸñ�ŵ���
			bookstore.books[i].stock++;
			bookstore.number++;
			break;
		}
	}
	if (i >= bookstore.types) {  //ԭ��û�иñ�ŵ��飬��Ҫ�����
		bookstore.books[bookstore.types] = book;
		bookstore.books[bookstore.types].stock = 1;
		bookstore.types++;
		bookstore.number++;
	}
	printf("ͼ����ӳɹ���\n");
}

//ɾ��ͼ��
void book_delete(Bookstore& bookstore) {
	printf("ɾ����ʽ��\n1.�����ɾ��\n2.������ɾ��\n****ѡ��ɾ����ʽ��");
	int choose;
	scanf("%d", &choose);
	switch (choose) {
	case 1://����ISBNɾ��
	{
		int result = book_query_ISBN(bookstore);
		if (result < 0) {
			printf("�����δ�ҵ���ͼ�飡\n");
		}
		else {
			bookstore.number -= bookstore.books[result].stock;
			for (int i = result; i < bookstore.types; i++) {
				bookstore.books[i] = bookstore.books[i + 1];
			}
			bookstore.types--;
			printf("�����ɾ���ɹ���\n");
		}
	}
	break;
	case 2:  //������ɾ��ͼ��                                 /**/
	{
		char* bookname = (char*)malloc(SIZE * sizeof(char));
		printf("����Ҫɾ����������");
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
			printf("�����δ�ҵ���ͼ�飡\n");
		}
		else {
			printf("�����ɾ���ɹ���\n");
		}
	}
	default:
		break;
	}
}

//�鿴����ͼ�����Ϣ
void see_all_books(Bookstore bookstore) {
	if (!bookstore.number) {
		printf("���������Ǹ�����꣡\n");
	}
	for (int i = 0; i < bookstore.types; i++) {
		BOOK book = bookstore.books[i];
		if (i == 0) {
			printf("���Ϊ��\n");
		}
		printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
	}
}

//�浵
void save(Bookstore bookstore) {
	FILE* fp1 = fopen("c:\\users\\������\\desktop\\���ȫ��ͼ����Ϣ.txt", "w+");
	rewind(fp1);
	const char* ISBN = (char*)malloc(SIZE * sizeof(char));
	ISBN = "ISBN";
	const char* bookname = (char*)malloc(SIZE * sizeof(char));
	bookname = "����";
	const char* writer = (char*)malloc(SIZE * sizeof(char));
	writer = "����";
	const char* publisher = (char*)malloc(SIZE * sizeof(char));
	publisher = "������";
	const char* price = (char*)malloc(SIZE * sizeof(char));
	price = "�۸�";
	const char* stock = (char*)malloc(SIZE * sizeof(char));
	stock = "�����";
	fprintf(fp1, "%-8s\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-40s\t\t\t\t\t\t\t\t\t\t\t%-20s\t\t\t\t\t%-20s\t\t\t\t\t\n", ISBN, bookname, writer, publisher, price, stock);
	for (int i = 0; i < bookstore.types; i++) {
		fprintf(fp1, "%-8d%-40s%-40s%-40s%-20d%-20d\n", bookstore.books[i].ISBN, bookstore.books[i].bookname, bookstore.books[i].writer, bookstore.books[i].publisher, bookstore.books[i].price, bookstore.books[i].stock);
	}
	fclose(fp1);
	printf("�����ͼ����Ϣ�浵�ɹ���\n");
}

//����
void read_from_external_deposit(Bookstore& bookstore) {
	FILE* fp1 = fopen("c:\\users\\������\\desktop\\���ȫ��ͼ����Ϣ.txt", "r");
	if (!fp1) {
		printf("��������ݶ�ȡʧ�ܣ����������ļ������ڣ�");
	}
	else {
		rewind(fp1);
		bookstore.types = bookstore.number = 0;
		char ch;
		while ((ch = fgetc(fp1)) != '\n');  //���ĵ��ڶ��п�ʼ��ȡ����
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
		printf("�����ͼ����Ϣ��ȡ�ɹ���\n");
	}
}

int main() {
	Bookstore bookstore;
	initial(bookstore);
	printf("\n�ɹ�����������ϵͳ��\n\n\n");
	int choose;
	printf("1.��ѯͼ����Ϣ\n2.���ͼ��\n3.ɾ��ͼ��\n4.�鿴����ͼ����Ϣ\n5.�浵\n6.������ȡ�����Ϣ��������һ�δ浵��\n(�0�ɽ���������\n****ѡ��ִ�й��ܣ�");
	scanf("%d", &choose);
	while (choose) {
		switch (choose)
		{
		case 1:  //����
		{
			int choose2;
			printf("��ѯ��ʽ��\n1.��ѯISBN���\n2.��ѯͼ������\n****ѡ����ҷ�ʽ��������ţ���");
			scanf("%d", &choose2);
			switch (choose2)
			{
			case 1:  //����Ų���
			{
				int result = book_query_ISBN(bookstore);
				if (result >= 0) {  //�ҵ��ñ�ŵ�ͼ��
					BOOK book = bookstore.books[result];
					printf("�����\n");
					printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
				}
				else {
					printf("�����δ�ҵ���ͼ�飡\n");
				}
			}
			break;
			case 2:  //������������
			{
				BOOK book;
				char* s = (char*)malloc(SIZE * sizeof(char));
				printf("����Ҫ���ҵ�ͼ������");
				scanf("%s", s);
				int result;
				int record = 0;  //���ڼ�¼�Ѿ���ӡ��ͼ�鱾��
				for (int i = 0; i < bookstore.types; ) {
					result = book_query_bookname(bookstore, s, i);
					if (result >= 0) {
						book = bookstore.books[result];
						if (!record) {
							printf("�����\n");
						}
						printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, book.stock);
						record++;
					}
				}
				if (!record) {
					printf("�����δ�ҵ���ͼ�飡\n");
				}
			}
			break;
			default:
				break;
			}
		}
		break;
		case 2: //���ͼ��
			book_add(bookstore);
			break;
		case 3:  //ɾ��ͼ��
			book_delete(bookstore);
			break;
		case 4:  //�鿴����ͼ�����Ϣ
			see_all_books(bookstore);
			break;
		case 5:  //�浵
			save(bookstore);
			break;
		case 6:  //����
			read_from_external_deposit(bookstore);
			break;
		default:
			break;
		}
		printf("\n\n1.��ѯͼ����Ϣ\n2.���ͼ��\n3.ɾ��ͼ��\n4.�鿴����ͼ����Ϣ\n5.�浵\n6.������ȡ�����Ϣ��������һ�δ浵��\n(�0�ɽ���������\n***����ѡ��ִ�й��ܣ�");
		scanf("%d", &choose);
	}
	printf("\n\n****����������****\n\n");
	return 0;
}