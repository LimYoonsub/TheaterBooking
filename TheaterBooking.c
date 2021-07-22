#include <stdio.h>
#include <string.h>

#define ROW 3
#define COL 5
#define USERS 10

struct user_info
{
	int id_num;
	char id[10];
	char password[10];
};

int print_seats(int s_array[ROW][COL]);
int reserve(int s_array[ROW][COL], char row, int col, int id_num);
int cancel(int s_array[ROW][COL], char row, int col, int id_num);
int load_user_info(struct user_info list[]);
int input_info(char p_id[], char p_pw[]);
int check_login(struct user_info list[], char p_id[], char p_pw[]);
int binary_search(struct user_info list[], int size, char p_id[], char p_pw[]);

int load_reservation_info(int s[][COL]);
int save_reservation_info(int s[][COL]);


int main(void)
{

	struct user_info user[USERS];
	if (load_user_info(user) == -1)
	{
		printf("���� ���� ����. ���α׷� ����\n");
		return 0;
	}

	int s[ROW][COL];

	load_reservation_info(s);

	int select;
	char s_row;
	int s_col;
	char key_id[10];
	char key_pw[10];
	int log = -1;
	char c;

	while (1) {

	
		do {
			printf("�α����� �ϼ���.\n");
			input_info(key_id, key_pw);
			int check = check_login(user, key_id, key_pw);
			if (check == -1)
				printf("ID�� �������� �ʽ��ϴ�.\n");
			else if (check == -2)
				printf("ID ����. PW�� ��ġ���� �ʽ��ϴ�.\n");
			else {
				printf("�α��� ����. %d �� �ݰ����ϴ�.\n", check);
				log = check;
			}

		} while (log == -1);

		do {

			printf("1 - �¼�Ȯ���ϱ�\n");
			printf("2 - �����ϱ�\n");
			printf("3 - ��������ϱ�\n");
			printf("4 - �ٸ� ����ڷ� �α����ϱ�\n");
			printf("5 - �����ϱ�\n");

			printf("�޴��� �����Ͻÿ� : ");
			scanf("%d", &select);
			while ((c = getchar()) != '\n' && c != EOF) {};

			if (select == 1) {

				printf("���õ� �޴� = �¼�Ȯ���ϱ�\n\n");
				print_seats(s);
			}
			else if (select == 2) {

				printf("���õ� �޴� = �����ϱ�\n\n");

				printf("������ ���ϴ� �ڸ���(�� - A2) ? : ");
				scanf("%c%d", &s_row, &s_col);
				while (c = getchar() != '\n' && c != EOF) {};


				if (reserve(s, s_row, s_col, log) == 1)
					printf("���� ����\n");
				else
					printf("�̹� ����� �¼��Դϴ�.\n");

				print_seats(s);

			}
			else if (select == 3) {

				printf("���õ� �޴� = ��������ϱ�\n\n");

				printf("������ ���ϴ� �ڸ���(�� - A2) ? : ");
				scanf("%c%d", &s_row, &s_col);
				while (c = getchar() != '\n' && c != EOF) {};


				if (cancel(s, s_row, s_col, log) == 1)
					printf("������� �Ϸ�\n");
				else if (cancel(s, s_row, s_col, log) == -2)
					printf("�ٸ� ���� ������ �¼��Դϴ�.\n");
				else
					printf("����Ǿ� ���� ���� �ڸ��Դϴ�.\n");
				print_seats(s);

			}
			else if (select == 4) {
				printf("�α׾ƿ� �Ǿ����ϴ�. �ٽ� �α��� �ϼ���.\n");
				log = -1;
				break;
			}

		} while (select != 5);

		if (select == 5) break;
	}
	printf("���õ� �޴� = �����ϱ�\n");
	printf("�̿��� �ּż� �����մϴ�.\n");

	save_reservation_info(s);

	return 0;

}


// ��� �Լ�

int print_seats(int s_array[ROW][COL]) {

	char col = 'A';
	printf("   0   1   2   3   4\n");
	printf("--------------------\n");
	for (int i = 0; i < ROW; i++) {
		printf("%c", col);
		for (int j = 0; j < COL; j++)
			printf("  %-2d", s_array[i][j]);
		col++;
		printf("\n");
	}

	return 0;
}


// ���� �Լ� // log ������ user ���� // id ������ log�� �޾� // ��ġ �� ��� ����

int reserve(int s_array[ROW][COL], char row, int col, int id_num) {

	int r;
	switch (row) {
	case 65:
		r = 0;
		break;
	case 66:
		r = 1;
		break;
	case 67:
		r = 2;
		break;
	}

	if (s_array[r][col] == 0) {
		s_array[r][col] = id_num;
		return 1;
	}
	else
		return -1;
}


// ������� �Լ� // log ������ user ���� // id ������ log�� �޾� // ��ġ �� ��� ����

int cancel(int s_array[ROW][COL], char row, int col, int id_num) {

	int r;
	switch (row) {
	case 65:
		r = 0;
		break;
	case 66:
		r = 1;
		break;
	case 67:
		r = 2;
		break;
	}

	if (s_array[r][col] == id_num) {
		s_array[r][col] = 0;
		return 1;
	}
	else
		if (s_array[r][col] != 0) // �ٸ� ��� �����
			return -2;
		else // ���� �ȵǾ����� ��
			return -1;
}

// user id pw info

int load_user_info(struct user_info list[]) {

	FILE* fp = NULL;

	if ((fp = fopen("user_data.txt", "r")) == NULL)
	{
		fprintf(stderr, "���� ������ �� �� �����ϴ�.\n");
		return -1;
	}

	int i;

	for(i = 0; i < 10; i ++)
	{
		fscanf(fp, "%d %s %s", &list[i].id_num, list[i].id, list[i].password); // list[i].id_num �� int ���̶� & �ٿ���� ?
	}

	char min[10];
	int min_idx;
	char temp[10];
	int temp_num;

	for (i = 0; i < 9; i++)
	{
		min_idx = i;
		strcpy(min, list[i].id);

		for (int j = i + 1; j < 10; j++)
		{
			if (strcmp(min, list[j].id) > 0)
			{
				strcpy(min, list[j].id);
				min_idx = j;
			}
		}

		strcpy(temp, list[min_idx].id);
		strcpy(list[min_idx].id, list[i].id);
		strcpy(list[i].id, temp);

		strcpy(temp, list[min_idx].password);
		strcpy(list[min_idx].password, list[i].password);
		strcpy(list[i].password, temp);

		temp_num = list[min_idx].id_num;
		list[min_idx].id_num = list[i].id_num;
		list[i].id_num = temp_num;

	}

	fclose(fp);

	return 0;
}

// input login info
int input_info(char p_id[], char p_pw[]) {
	printf("ID �Է� : ");
	gets_s(p_id, 10);
	printf("PW �Է� : ");
	gets_s(p_pw, 10); 
	
	return 0;
}


// check login info // ���� ID : -1 // ID PW ��ġ : ID // ����ġ : -2 return
int check_login(struct user_info list[], char p_id[], char p_pw[]) {

	int check;
	check = binary_search(list, USERS, p_id, p_pw);
	if (check == -1)
		return -1;
	else
	{
		if (strcmp(list[check].password, p_pw) == 0)
			return list[check].id_num;
		else
			return -2;
	}

}

// Ž��. ID ��ġ ���θ� �ľ�. index(������) / -1(���� ��) �� return
int binary_search(struct user_info list[], int size, char p_id[], char p_pw[]) {

	int low, high, middle;
	low = 0;
	high = size - 1;

	while (low <= high) {
		middle = (low + high) / 2;
		if (strcmp(list[middle].id, p_id) == 0)
			return middle;
		else if (strcmp(list[middle].id, p_id) < 0)
			low = middle + 1;
		else
			high = middle - 1;

	}

	return -1;
}

int load_reservation_info(int s[][COL])
{
	FILE* fp = NULL;
	if ((fp = fopen("reservation_data.txt", "r")) == NULL)
	{
		fprintf(stderr,"���� ���� ������ �� �� �����ϴ�.\n");
		return -1;
	}
	
	while (!feof(fp))
	{
		for (int i = 0; i < 3; i++)
		{ 
			for (int j = 0; j < COL; j++)
				fscanf(fp, "%d", &s[i][j]);
		}
	}
	fclose(fp);

	return 0;
}

int save_reservation_info(int s[][COL])
{
	FILE* fp = NULL;
	if ((fp = fopen("reservation_data.txt", "w")) == NULL)
	{
		fprintf(stderr, "���� ���� ������ �� �� �����ϴ�.\n");
		return -1;
	}


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < COL; j++)
			fprintf(fp, "%d ", s[i][j]);
		fprintf(fp,"\n");
	}
	

	fclose(fp);

	return 0;
}
