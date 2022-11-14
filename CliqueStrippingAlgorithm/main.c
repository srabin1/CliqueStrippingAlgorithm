#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <string.h>
#include<stdbool.h>
#define MAXCHAR 100000
#define N 8


int adj_matrix[N][N];
int u_[N];
int v_[N];
int num_edges = 0;


struct Node {
	int data;
	struct Node* left;
	struct Node* right;
};

struct Node* newNode(int data) {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->data = data;
	node->left = node->right = NULL;
	return node;
}


void display(struct Node* root) {

	if (root != NULL) {
		display(root->left);
		printf("%d\n", root->data);
		display(root->right);
	}
}

struct Node* insertLevelOrder(int arr[], int i, int n) {
	struct Node* root = NULL;
	if (i < n) {
		root = newNode(arr[i]);
		root->left = insertLevelOrder(arr, 2 * i + 1, n);
		root->right = insertLevelOrder(arr, 2 * i + 2, n);

	}
	return root;
}

void inOrder(struct Node* root) {
	if (root != NULL) {
		inOrder(root->left);
		printf("%d", root->data);
		inOrder(root->right);
	}

}


void load_adj_matrix() {
	FILE* fpointer = fopen("Bipartite-graph.csv", "r");
	char line[MAXCHAR];
	if (!fpointer)
		printf("Cann't open the file\n");
	else {
		int row = 0;
		int col = 0;
		while (fgets(line, MAXCHAR, fpointer)) {
			col = 0;
			//strtok break down each line into smaller string
			char* value = strtok(line, ",");
			while (value != NULL) {
				adj_matrix[row][col] = atoi(value);
				value = strtok(NULL, ",");
				col++;
			}
			row++;
			
		}
		//printf("\n");
		fclose(fpointer);
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf(" %d ", adj_matrix[i][j]);
		}
		printf("\n");
	}

}
void get_edges() {
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	printf("%d", num_edges);

}

void left_part_u() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			u_[i] = adj_matrix[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		printf("%d", u_[i]);
	}
	printf("\n");

}

bool isEdges() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (adj_matrix[i][j] == 1) {
				return true;
			}
			else
				return false;
		}
	}

	
}



void decToBinary(int n)
{
	// array to store binary number
	int binaryNum[32];

	// counter for binary array
	int i = 0;
	while (n > 0) {
		// storing remainder in binary array
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	// printing binary array in reverse order
	for (int j = i - 1; j >= 0; j--)
		printf("%d", binaryNum[j]);
}


int* convertStringToInteger(char* c)
{
	int len = strlen(c);//c.length()
	int i, j=0;
	int* a = (int*)malloc(len * sizeof(int));
	for (i = 0; i < len; i++)
		if (c[i] == ",")
			continue;
		if (c[i] == " ") {
			j++;
			}
		else {
			a[i] = c[j] *10 +(c[i] - 48);
		}
	return a;
}

int main() {
	
	int n = 17;
	printf("decimal 17 to binary: ");
	decToBinary(n);
	printf("\n");
	
	printf("load adjacency matrix:\n");
	load_adj_matrix();
	
	printf("get edges:\n");
	get_edges();
	printf("\n");

	printf("is edge?\n");
	isEdges();
	printf("\n");
	printf("Part U:\n");
	left_part_u();


	//printf("converted array:\n");
	//convertStringToInteger(fpointer);
	return 0;
}


