#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

/*�������ͼ���ISBN������д���*/

#define SIZE 20  //������Ҫ�Ŀռ��С
#define type_initial_size 20  //ÿһ�����ʼ�ɴ����
#define type_size_increasement 10
#define bookstore_initial_size 200
#define bookstore_size_increasement 50

//ÿ�������Ϣ
typedef struct {
	int ISBN;  //ͼ���ISBN��
	char* bookname;  //����
	char* writer;  //ͼ������
	char* publisher;  //ͼ�������
	int price;  //ͼ��۸�
}BOOK;

//������
typedef struct {
	char* bookname;
	int stock;  //��ͼ��Ŀ����
}BOOK_STOCK;

//���ϵͳ
typedef struct {
	BOOK* books;
	BOOK_STOCK* book_stock;
	int book_stock_capacity;
	int types;  //�������
	int number;  //���ͼ������
	int bookstore_capacity;  //�������
}Bookstore;

//ͼ�����ϵͳ�ĳ�ʼ��
void initial(Bookstore& bookstore) {
	bookstore.books = (BOOK*)malloc(bookstore_initial_size * sizeof(BOOK));
	bookstore.book_stock = (BOOK_STOCK*)malloc(type_initial_size * sizeof(BOOK_STOCK));
	if (!bookstore.books || !bookstore.book_stock) {
		printf("��ʼ���ڴ����ʧ�ܣ�\n");
		exit(-1);
	}
	bookstore.number = 0;
	bookstore.types = 0;
	bookstore.book_stock_capacity = type_initial_size;
	bookstore.bookstore_capacity = bookstore_initial_size;
}

//����ISBN��Ų���ͼ��
int book_query_ISBN(Bookstore bookstore) {
	//�����ҵ������ظ�����±ꣻ��δ���ҵ�������-1��
	int ISBN;
	printf("������ISBN��ţ�");
	scanf("%d", &ISBN);
	for (int i = 0; i < bookstore.number; i++) {
		if (bookstore.books[i].ISBN == ISBN) {
			return i;
		}
	}
	return -1;
}

//����ͼ�����Ʋ���ͼ��
int book_query_bookname(Bookstore bookstore, char* s, int& i) {
	//�����ҵ������ظ�����±ꣻ��δ���ҵ�������-1��
	for (; i < bookstore.number; i++) {
		if (!strcmp(s, bookstore.books[i].bookname)) {  //�鵽ͼ�飬������
			i++;
			return i - 1;
		}
	}
	return -1;  //���һ�����ǣ�����Ҳ��Ҫ����
}

//ͼ����Ϣ�����
void book_add(Bookstore& bookstore) {
	if (bookstore.number >= bookstore.bookstore_capacity) {  //�������
		bookstore.books = (BOOK*)realloc(bookstore.books, (bookstore.bookstore_capacity + bookstore_size_increasement) * sizeof(BOOK));
		if (!bookstore.books) {
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
	bookstore.books[bookstore.number++] = book;
	/*��¼ÿ����Ŀ����*/
	if (bookstore.types > 0) {  //�������
		int i;
		for (i = 0; i < bookstore.types; i++) {
			if (!strcmp(bookstore.book_stock[i].bookname, book.bookname)) {  //��һ������
				bookstore.book_stock[i].stock++;
				break;
			}
		}
		if (i >= bookstore.types) {  //û��һ������
			if (bookstore.types >= bookstore.book_stock_capacity) {
				bookstore.book_stock = (BOOK_STOCK*)realloc(bookstore.book_stock, (bookstore.types + type_size_increasement) * sizeof(BOOK_STOCK));
				if (!bookstore.book_stock) {
					printf("���ͼ�������ַ�ռ����ʧ�ܣ�\n");
					exit(-1);
				}
			}
			bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
			bookstore.book_stock[bookstore.types].bookname = book.bookname;
			bookstore.book_stock[bookstore.types].stock = 1;
			bookstore.types++;
		}

	}
	else {  //���û����
		bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
		bookstore.book_stock[bookstore.types].bookname = book.bookname;
		bookstore.book_stock[bookstore.types].stock = 1;
		bookstore.types = 1;
	}
	printf("�������ӳɹ���\n");
}

//ͼ����Ϣ��ɾ��
void book_delete(Bookstore& bookstore) {
	printf("ɾ����ʽ��\n1.�����ɾ��\n2.������ɾ��\n****ѡ��ɾ����ʽ��");
	int choose;
	scanf("%d", &choose);
	switch (choose)
	{
	case 1:  //�����ɾ��ͼ��
	{
		int result = book_query_ISBN(bookstore);  //����ͼ��
		if (result < 0) {
			printf("�����δ�ҵ���ͼ�飡\n");
		}
		else {
			for (int i = 0; i < bookstore.types; i++) {  //��ɾ����ͼ��ı������и���
				if (!strcmp(bookstore.book_stock[i].bookname, bookstore.books[result].bookname)) {  //����ͼ��ɾ��һ��
					bookstore.book_stock[i].stock--;
					if (!bookstore.book_stock[i].stock) {  //����ͼ��ɾ��
						for (int j = i; j < bookstore.types; j++) {
							bookstore.book_stock[j] = bookstore.book_stock[j + 1];
						}
						bookstore.types--;
						break;
					}
					break;
				}
			}
			for (int i = result; i < bookstore.number - 1; i++) {  //������ܵ�ͼ����Ϣ���и���
				bookstore.books[i] = bookstore.books[i + 1];
			}
			bookstore.number--;
			printf("�����ɾ���ɹ���\n");
		}
	}
	break;
	case 2:  //������ɾ��ͼ��
	{
		char* s = (char*)malloc(SIZE * sizeof(char));
		printf("����Ҫɾ����������");
		scanf("%s", s);
		int record;  //Ҫɾ����ͼ����book_stock�е��±�
		for (record = 0; record < bookstore.types; record++) {
			if (!strcmp(bookstore.book_stock[record].bookname, s)) {
				break;
			}
		}
		if (record == bookstore.types) {
			printf("�����δ�ҵ���ͼ�飡\n");
		}
		else {
			int result;
			int i = 0;
			for (; i < bookstore.number;) {
				result = book_query_bookname(bookstore, s, i);  //ÿ���й�һ�ξ��൱���ҵ�һ��Ҫɾ����ͼ��
				if (result >= 0) {  //�ҵ�ͼ�鲢ɾ���������ͼ����Ҫ��ǰ��
					for (int j = result; j < bookstore.number; j++) {
						bookstore.books[j] = bookstore.books[j + 1];
					}
					bookstore.book_stock[record].stock--;
					if (!bookstore.book_stock[record].stock) {  //����ͼ��ɾ��
						for (int j = record; j < bookstore.types; j++) {
							bookstore.book_stock[j] = bookstore.book_stock[j + 1];
						}
						bookstore.types--;
					}
					bookstore.number--;
					i = 0;
				}
			}
			printf("�����ɾ���ɹ���\n");
		}
	}
	break;
	default:
		break;
	}
}

//�鿴����ͼ����Ϣ
void see_all_books(Bookstore& bookstore) {
	if (!bookstore.number) {
		printf("���������Ǹ�����꣡\n");
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
			printf("���Ϊ��\n");
		}
		printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[j].stock);
	}
}

//��������ͼ����Ϣ��������һ�δ浵�㣩
void read_from_external_deposit(Bookstore& bookstore) {
	FILE* fp1 = fopen("c:\\users\\������\\desktop\\���ȫ��ͼ����Ϣ.txt", "r");
	FILE* fp2 = fopen("c:\\users\\������\\desktop\\ͼ������Ϣ.txt", "r");
	if (!fp1 || !fp1) {
		printf("��������ݶ�ȡʧ�ܣ����������ļ������ڣ�");
	}
	else {
		rewind(fp1);
		rewind(fp2);
		bookstore.types = 0;
		bookstore.number = 0;
		char ch;
		while ((ch = fgetc(fp2)) != '\n');  //���ĵ��ڶ��п�ʼ��ȡ����
		while (!feof(fp2)) {
			bookstore.book_stock[bookstore.types].bookname = (char*)malloc(SIZE * sizeof(char));
			fscanf(fp2, "%s%d", bookstore.book_stock[bookstore.types].bookname, &(bookstore.book_stock[bookstore.types].stock));
			bookstore.types++;
		}
		bookstore.types--;
		while ((ch = fgetc(fp1)) != '\n');  //���ĵ��ڶ��п�ʼ��ȡ����
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
		printf("�����ͼ����Ϣ��ȡ�ɹ���\n");
	}
}

//�浵
void book_update(Bookstore bookstore) {
	FILE* fp1 = fopen("c:\\users\\������\\desktop\\���ȫ��ͼ����Ϣ.txt", "w+");
	FILE* fp2 = fopen("c:\\users\\������\\desktop\\ͼ������Ϣ.txt", "w+");
	rewind(fp1);
	rewind(fp2);
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
	printf("�����ͼ����Ϣ�浵�ɹ���\n");
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
		case 1://����
		{
			int choose2;
			printf("��ѯ��ʽ��\n1.��ѯISBN���\n2.��ѯͼ������\n****ѡ����ҷ�ʽ��������ţ���");
			scanf("%d", &choose2);
			switch (choose2) {
			case 1:  //����Ų���
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
					printf("�����\n");
					printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[i].stock);
				}
				else {
					printf("�����δ�ҵ���ͼ�飡\n");
				}
			}
			break;
			case 2:  //����������
			{
				BOOK book;
				char* s = (char*)malloc(SIZE * sizeof(char));
				printf("����Ҫ���ҵ�ͼ������");
				scanf("%s", s);
				int result;
				int record = 0;  //���ڼ�¼�Ѿ���ӡ��ͼ�鱾��
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
							printf("�����\n");
						}
						printf("ISBN��%-8d  ������%-20s  ���ߣ�%-10s  �����磺%-10s  �۸�%-5d  �������%-5d\n", book.ISBN, book.bookname, book.writer, book.publisher, book.price, bookstore.book_stock[i].stock);
						record++;
					}
				}
				if (!record) {
					printf("�����δ�ҵ���ͼ�飡\n");
				}
			}
			default:
				break;
			}
		}
		break;
		case 2://���
			book_add(bookstore);
			break;
		case 3://ɾ��
			book_delete(bookstore);
			break;
		case 4://�鿴����ͼ����Ϣ
			see_all_books(bookstore);
			break;
		case 5://�浵
			book_update(bookstore);
			break;
		case 6://������ȡ��Ϣ��������һ�δ浵��
			read_from_external_deposit(bookstore);
			break;
		default:
			break;
		}
		printf("\n\n1.��ѯͼ����Ϣ\n2.���ͼ��\n3.ɾ��ͼ��\n4.�鿴����ͼ����Ϣ\n5.�浵\n6.������ȡ�����Ϣ��������һ�δ浵��\n(�0�ɽ���������\n***����ѡ��ִ�й��ܣ�");
		scanf("%d", &choose);
	}
	printf("\n\n  ����������");
	return 0;
}

