#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Sleep �Լ� 

#define HIGH(a, b)   (((a) > (b)) ? (a) : (b))

#define MAX 500 //��ǻ�� ��������!!!!!!
// �ִ� �����ϼ���!!!!(int~=9�� 2õ��)
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
	int count = MAX;// �迭�� ���� 
	int* list;//�迭 
	int i, j;// ī���� 
	int num[MAX] = { 0, }; //�������� �迭
	
	srand((unsigned)time(NULL));//���� �ʱ�ȭ


	list = (int*)malloc((sizeof(int)) * count);

	for (i = 0; i < MAX; i++)
	{
		num[i] = rand() % MAX ; //0 ~ MAX-1���� �� �̱�
		for (j = 0; j < i; j++) {
			if (num[i] == num[j])  //�ߺ���
			{
				i--;  // ����
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

pnode init_node(int data)//��Ʈ ��� ���� 
{
	pnode ret = (pnode)malloc(sizeof(node));//��Ʈ ����� ���� 
	ret->data = data;
	ret->right = NULL;
	ret->left = NULL;
	ret->t_left = TRUE;
	ret->t_right = TRUE;
	return ret;
}

void make_node(pnode htree, int data)// ����� �߰� 
{
	pnode make;
	make = (pnode)malloc(sizeof(node));
	make->data = data;
	make->left = NULL;
	make->right = NULL;// ��带 �̸� �����Ѵ�. 
	make->t_left = FALSE;
	make->t_right = FALSE;

	if (htree->data < data)//�����ʿ� �߰� 
	{
		if (htree->right) {
			make_node(htree->right, data);// ������ ���� Ž��
			htree->t_right = 1;
		}
		else {
			htree->right = make;
		}
	}
	else if (htree->data > data)// ���ʿ� �߰� 
	{
		if (htree->left) {
			htree->t_left = 1;
			make_node(htree->left, data);//���� ���� Ž�� 
		}
		else {
			htree->left = make;
		}
	}
	else//���� ��� 
	{
		free(make); // ��� ���� 
		return;
	}
	return;
}

void pre_order(pnode htree, int target)//���� ��ȸ�� Ž��
{
	if (htree)
	{
		//	printf("%d ", htree->data);// ��带 �湮 

		if (htree->data == target)
		{
			return;
		}
		pre_order(htree->left, target);// ������ ��ȸ 
		pre_order(htree->right, target);//�������� ��ȸ 
	}
}

void in_order()//���� ��ȸ 
{
	if (htree)
	{
		while (htree->left != NULL)
		{
			htree = htree->left;
		}
		// ������ ��ȸ 
		if (htree->data == num)
		{
			return;
		}
		while (htree->right != NULL)
		{
			htree = htree->right;
		}//�������� ��ȸ 
	}
}

void post_order(pnode htree, int target)//���� ��ȸ 
{
	if (htree)
	{
		post_order(htree->left, target);// ������ ��ȸ 
		post_order(htree->right, target);//�������� ��ȸ 
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
	const char bar = '='; // ���α׷����� ����  
	const char blank = ' '; // ����ִ� ���α׷����� ����  
	const int LEN = 20; // ���α׷����� ����  
	const int SPEED = 50; // ī��Ʈ ���� ���ð�  
	int count = 0; // ���� ����� �۾�  
	float tick = (float)100 / LEN; // �� %���� ���α׷����� �߰����� ��� 
	int bar_count; // ���α׷����� ���� ���� ����  
	float percent; // �ۼ�Ʈ ���� ���� 

	FILE* fp;
	fp = fopen("BST.txt", "a+");
	if (fp == NULL) {
		printf("���� ���� ���� \n");
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
		printf("\r%d/%d [", count, MAX); // ���� ���� ���  
		percent = (float)count / MAX * 100; // �ۼ�Ʈ ���  
		bar_count = percent / tick; // ���α׷����� ���� ���  
		for (i = 0; i < LEN; i++) { // LEN������ ���α׷����� ���  
			if (bar_count > i) { // ���α׷����� ���̺��� i�� ������ 
				printf("%c", bar);
			}
			else { // i�� �� Ŀ����  
				printf("%c", blank);
			}
		}
		printf("] %0.2f%%", percent); // �ۼ�Ʈ ���  
		count++;
		Sleep(SPEED); // SPEEDms ���  
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
