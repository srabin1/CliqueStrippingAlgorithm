#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <string.h>
#include<stdbool.h>
#include<math.h>
#define MAXCHAR 100000
#define N 8


int adj_matrix[N][N];
int u_[N];
int v_[N];
int num_edges = 0;
int degree;
int height;



int logFunction() {
	height = log2(N);
	printf("log2 of 8 is: %d", height);
	return height;
	
}


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


void displayPostOrder(struct Node* Node) {

	if (Node == NULL) {
		return;
	}
	if (Node != NULL) {
		displayPostOrder(Node->left);
		displayPostOrder(Node->right);
		printf("%d\n", Node->data);
		
	}
}

void recursiveCall(struct Node* root) {
	for (int i = 1; i < N + 1; i++) {
		root = newNode(i);
		for (int j = 1; j < N + 1; j++) {
			if (height > 0 && height <= 2) {
				if (adj_matrix[i][j] == 1 && j <= N / 2) {
					root->left = newNode(2 * i);
				}
				else if (adj_matrix[i][j] == 1 && j > N / 2) {
					root->right = newNode(2*i + 1);
				}
			}
			 
		}
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
		int row = 1;
		int col = 1;
		while (fgets(line, MAXCHAR, fpointer)) {
			col = 1;
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

	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
			printf(" %d ", adj_matrix[i][j]);
		}
		printf("\n");
	}

}
void get_edges() {

	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	printf("%d", num_edges);

}




int hadEdge(int u, int v) {
	for (u = 0; u < N; u++) {
		for (v = 0; v < N; v++) {
			if (adj_matrix[u][v] == 1) {
				printf("There is an edge between u[%d] and v[%d]", u, v);
				return true;
			}
			else {
				printf("there is no edge between u[%d] and v[%d]", u, v);
				return false;
			}
		}
	}


}
struct node {
	int vertex;
	struct node* next;
};

struct node* createNode(int);

struct Graph {
	int numVertices;
	struct node** adjList;
};

//create node function
struct node* createNode(int v) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

//create a graph
struct Graph* createGraph() {
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->numVertices = N;
	graph->adjList = malloc(N * sizeof(struct node*));
	int i;
	for (i = 1; i < N + 1; i++)
		graph->adjList[i] = NULL;
		
	return graph;
}

void addEdge(struct Graph* graph, int s, int d) {
	// Add edge from s to d
	struct node* newNode = createNode(d);
	newNode->next = graph->adjList[s];
	graph->adjList[s] = newNode;

	
}

// Print the graph
void printGraph(struct Graph* graph) {
	int v;
	for (v = 1; v < graph->numVertices + 1; v++) {
		struct node* temp = graph->adjList[v];
		printf("Vertex u[%d]: ", v);
		while (temp) {
			printf("%d", temp->vertex);
			temp = temp->next;
			printf("->");
		}
		printf("\n");
	}
}

void nodeDegree() {
	
	for (int i = 1; i < N + 1; i++) {
		degree = 0;
		for (int j = 1; j < N + 1; j++) {
			degree = degree + adj_matrix[i][j];		
		}
		printf("degree of u[%d] is: %d\n", i, degree);
				
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
	struct Graph* graph = createGraph(N);
	
	printf("decimal 17 to binary: ");
	decToBinary(n);
	printf("\n");
		
	printf("load adjacency matrix:\n");
	load_adj_matrix();
	for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {
			if (adj_matrix[i][j] == 1) {
				addEdge(graph, i, j);
			}

		}
	}
	printf("load adjacency list:\n");
	printGraph(graph);
	
	printf("get edges:\n");
	get_edges();
	printf("\n");

	printf("get degree:\n");
	nodeDegree();

	printf("is edge?\n");
	printf("\n");
	
	logFunction();


	//printf("converted array:\n");
	//convertStringToInteger(fpointer);
	return 0;
}
 

