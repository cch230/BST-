#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Sleep 함수 

#define HIGH(a, b)   (((a) > (b)) ? (a) : (b))

#define MAX 500 //컴퓨터 죽을수도!!!!!!
// 최댓값 조절하세요!!!!(int~=9경 2천억)
#define TRUE 1
#define FALSE 0

typedef struct _node
{
	int data;
	struct _node* left;
	struct _node* right;
	int t_left;
	int t_right;
}node, * pnode;

struct thread_args
{
	int id;

};

void post_order(pnode htree, int target);
void make_node(pnode htree, int data);
pnode init_node(int data);
void pre_order(pnode htree, int target);
void in_order();
void findnum(pnode htree);
int get_height(pnode node);

pnode htree;
volatile long long num;


int main()
{
	int count = MAX;// 배열의 길이 
	int* list;//배열 
	int i, j;// 카운터 
	int num[MAX] = { 0, }; //난수넣을 배열
	
	srand((unsigned)time(NULL));//난수 초기화


	list = (int*)malloc((sizeof(int)) * count);

	for (i = 0; i < MAX; i++)
	{
		num[i] = rand() % MAX ; //0 ~ MAX-1까지 수 뽑기
		for (j = 0; j < i; j++) {
			if (num[i] == num[j])  //중복시
			{
				i--;  // 제거
				break;
			}
		}
	}

	for (i = 0; i < MAX; i++)
		list[i] = num[i];
	htree = init_node(list[0]);

	for (i = 1; i < count; i++)
		make_node(htree, list[i]);

	findnum(htree);
	printf("\n");
	return 0;
}

pnode init_node(int data)//루트 노드 생성 
{
	pnode ret = (pnode)malloc(sizeof(node));//루트 노드의 생성 
	ret->data = data;
	ret->right = NULL;
	ret->left = NULL;
	ret->t_left = TRUE;
	ret->t_right = TRUE;
	return ret;
}

void make_node(pnode htree, int data)// 노드의 추가 
{
	pnode make;
	make = (pnode)malloc(sizeof(node));
	make->data = data;
	make->left = NULL;
	make->right = NULL;// 노드를 미리 생성한다. 
	make->t_left = FALSE;
	make->t_right = FALSE;

	if (htree->data < data)//오른쪽에 추가 
	{
		if (htree->right) {
			make_node(htree->right, data);// 오른쪽 으로 탐색
			htree->t_right = 1;
		}
		else {
			htree->right = make;
		}
	}
	else if (htree->data > data)// 왼쪽에 추가 
	{
		if (htree->left) {
			htree->t_left = 1;
			make_node(htree->left, data);//왼쪽 으로 탐색 
		}
		else {
			htree->left = make;
		}
	}
	else//같은 경우 
	{
		free(make); // 노드 삭제 
		return;
	}
	return;
}

void pre_order(pnode htree, int target)//전위 순회로 탐색
{
	if (htree)
	{
		//	printf("%d ", htree->data);// 노드를 방문 

		if (htree->data == target)
		{
			return;
		}
		pre_order(htree->left, target);// 왼쪽을 순회 
		pre_order(htree->right, target);//오른쪽을 순회 
	}
}

void in_order()//중위 순회 
{
	if (htree)
	{
		while (htree->left != NULL)
		{
			htree = htree->left;
		}
		// 왼쪽을 순회 
		if (htree->data == num)
		{
			return;
		}
		while (htree->right != NULL)
		{
			htree = htree->right;
		}//오른쪽을 순회 
	}
}

void post_order(pnode htree, int target)//후위 순회 
{
	if (htree)
	{
		post_order(htree->left, target);// 왼쪽을 순회 
		post_order(htree->right, target);//오른쪽을 순회 
		if (htree->data == target)
		{
			return;
		}
	}
}

void findnum(pnode htree)
{
	clock_t start[MAX] = { 0 }, finish[MAX] = { 0 };

	int num[MAX] = { 0 };
	int state=0,i;
	double result[MAX] = { 0 };
	double sum = 0;
	double avg = 0;
	int treeheight=0;
	const char bar = '='; // 프로그레스바 문자  
	const char blank = ' '; // 비어있는 프로그레스바 문자  
	const int LEN = 20; // 프로그레스바 길이  
	const int SPEED = 50; // 카운트 증가 대기시간  
	int count = 0; // 현재 진행된 작업  
	float tick = (float)100 / LEN; // 몇 %마다 프로그레스바 추가할지 계산 
	int bar_count; // 프로그레스바 갯수 저장 변수  
	float percent; // 퍼센트 저장 변수 

	FILE* fp;
	fp = fopen("BST.txt", "a+");
	if (fp == NULL) {
		printf("파일 열기 오류 \n");
		return;
	}
	for (i = 0; i < MAX; i++)
		num[i] = i;
	while(count<MAX) {
		i = count;
		start[i] = clock();
		pre_order(htree, num[i]);
		finish[i] = clock();
		result[i] = (double)(finish[i] - start[i]) / CLOCKS_PER_SEC;
		sum += result[i];
		printf("\r%d/%d [", count, MAX); // 진행 상태 출력  
		percent = (float)count / MAX * 100; // 퍼센트 계산  
		bar_count = percent / tick; // 프로그레스바 갯수 계산  
		for (i = 0; i < LEN; i++) { // LEN길이의 프로그레스바 출력  
			if (bar_count > i) { // 프로그레스바 길이보다 i가 작으면 
				printf("%c", bar);
			}
			else { // i가 더 커지면  
				printf("%c", blank);
			}
		}
		printf("] %0.2f%%", percent); // 퍼센트 출력  
		count++;
		Sleep(SPEED); // SPEEDms 대기  
	}
	avg = sum / MAX;
	printf("_______________________\n");  //ping
	treeheight = get_height(htree);
	/*fprintf(fp,"This is BST program.\n");
	fprintf(fp, "Find to Pre_order\n");
	fprintf(fp, "_______________________\n");
	fprintf(fp, "1~%d\n", MAX);
	fprintf(fp, "Searching Time of AVG: %lf\n", avg);
	fprintf(fp, "BST High of AVG: %d\n", treeheight);*/

	printf("This is BST program.\n");
	printf("Find to Pre_order\n");
	printf("_______________________\n");
	printf("1~%d\n", MAX);
	printf("Searching Time of AVG: %lf\n", avg);
	printf("BST High of AVG: %d\n", treeheight);

	fclose(fp);	
}

int get_height(pnode node)
{
	int height = 0;
	if (node != NULL)
	{
		height = 1 + HIGH(get_height(node->left), get_height(node->right));
	}
	return height;
}
