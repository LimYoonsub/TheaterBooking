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
		printf("파일 열기 실패. 프로그램 종료\n");
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
			printf("로그인을 하세요.\n");
			input_info(key_id, key_pw);
			int check = check_login(user, key_id, key_pw);
			if (check == -1)
				printf("ID가 존재하지 않습니다.\n");
			else if (check == -2)
				printf("ID 존재. PW가 일치하지 않습니다.\n");
			else {
				printf("로그인 성공. %d 님 반갑습니다.\n", check);
				log = check;
			}

		} while (log == -1);

		do {

			printf("1 - 좌석확인하기\n");
			printf("2 - 예약하기\n");
			printf("3 - 예약취소하기\n");
			printf("4 - 다른 사용자로 로그인하기\n");
			printf("5 - 종료하기\n");

			printf("메뉴를 선택하시오 : ");
			scanf("%d", &select);
			while ((c = getchar()) != '\n' && c != EOF) {};

			if (select == 1) {

				printf("선택된 메뉴 = 좌석확인하기\n\n");
				print_seats(s);
			}
			else if (select == 2) {

				printf("선택된 메뉴 = 예약하기\n\n");

				printf("예약을 원하는 자리는(예 - A2) ? : ");
				scanf("%c%d", &s_row, &s_col);
				while (c = getchar() != '\n' && c != EOF) {};


				if (reserve(s, s_row, s_col, log) == 1)
					printf("예약 성공\n");
				else
					printf("이미 예약된 좌석입니다.\n");

				print_seats(s);

			}
			else if (select == 3) {

				printf("선택된 메뉴 = 예약취소하기\n\n");

				printf("예약을 원하는 자리는(예 - A2) ? : ");
				scanf("%c%d", &s_row, &s_col);
				while (c = getchar() != '\n' && c != EOF) {};


				if (cancel(s, s_row, s_col, log) == 1)
					printf("예약취소 완료\n");
				else if (cancel(s, s_row, s_col, log) == -2)
					printf("다른 고객이 예약한 좌석입니다.\n");
				else
					printf("예약되어 있지 않은 자리입니다.\n");
				print_seats(s);

			}
			else if (select == 4) {
				printf("로그아웃 되었습니다. 다시 로그인 하세요.\n");
				log = -1;
				break;
			}

		} while (select != 5);

		if (select == 5) break;
	}
	printf("선택된 메뉴 = 종료하기\n");
	printf("이용해 주셔서 감사합니다.\n");

	save_reservation_info(s);

	return 0;

}


// 출력 함수

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


// 예약 함수 // log 값으로 user 구별 // id 변수를 log로 받아 // 일치 시 기능 수행

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


// 예약취소 함수 // log 값으로 user 구별 // id 변수를 log로 받아 // 일치 시 기능 수행

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
		if (s_array[r][col] != 0) // 다른 사람 예약시
			return -2;
		else // 예약 안되어있을 때
			return -1;
}

// user id pw info

int load_user_info(struct user_info list[]) {

	FILE* fp = NULL;

	if ((fp = fopen("user_data.txt", "r")) == NULL)
	{
		fprintf(stderr, "유저 파일을 열 수 없습니다.\n");
		return -1;
	}

	int i;

	for(i = 0; i < 10; i ++)
	{
		fscanf(fp, "%d %s %s", &list[i].id_num, list[i].id, list[i].password); // list[i].id_num 은 int 형이라 & 붙여줘야 ?
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
	printf("ID 입력 : ");
	gets_s(p_id, 10);
	printf("PW 입력 : ");
	gets_s(p_pw, 10); 
	
	return 0;
}


// check login info // 없는 ID : -1 // ID PW 일치 : ID // 불일치 : -2 return
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

// 탐색. ID 일치 여부만 파악. index(있을시) / -1(없을 시) 로 return
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
		fprintf(stderr,"예약 정보 파일을 열 수 없습니다.\n");
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
		fprintf(stderr, "예약 정보 파일을 열 수 없습니다.\n");
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
