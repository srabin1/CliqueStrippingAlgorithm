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
int matrix[(2 * N) - 1][(2 * N) - 1];
int u_[N];
int v_[N];
int num_edges = 0;
int degree;
int height;


//function to calculate the height of a binary tree
int logFunction() {
	height = log2(N);
	printf("log2 of N is: %d", height);
	return height;
	
}

//function to load adjacency matrix from csv file and print it
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
		fclose(fpointer);
	}

	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
			printf(" %d ", adj_matrix[i][j]);
		}
		printf("\n");
	}

}

//function to count number of edge in graph G
void get_edges() {

	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	printf("%d", num_edges);

}

//function to calculate degree of each vertex in part U
void nodeDegree() {

	for (int i = 1; i < N + 1; i++) {
		degree = 0;
		for (int j = 1; j < N + 1; j++) {
			degree = degree + adj_matrix[i][j];
		}
		printf("degree of u[%d] is: %d\n", i, degree);

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

//function to create a new node
struct node* createNode(int v) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

//create a graph's adjacency list
struct Graph* createGraph() {
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->numVertices = N;
	graph->adjList = malloc(N * sizeof(struct node*));
	int i;
	for (i = 1; i < N + 1; i++)
		graph->adjList[i] = NULL;
		
	return graph;
}


// Print the graph adjacency list
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

//function to add edge from vertex s to vertex d
void addEdge(struct Graph* graph, int s, int d) {
	struct node* newNode = createNode(d);
	newNode->next = graph->adjList[s];
	graph->adjList[s] = newNode;
}

/*************************************************************/
void createMatrix() {
	for (int i = 1; i < N+1; i++) {
		for (int j = (2*N)-1 ; j > 0; j--) {
			if (j < N+1) {
				matrix[i][j+ (N-1)] = adj_matrix[i][j];
				
			}
			
			
		}
	}
	for (int i = 1; i < N + 1; i++) {
		for (int j = N-1; j > 0; j--) {
			matrix[i][j] = matrix[i][2 * j] + matrix[i][(2 * j) + 1];
		}
	}


	
	for (int i = 1; i < N+1; i++) {
		for (int j = 1; j < 2 * N; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}
/****************************************************************/

struct Node {
	int data;
	struct Node* left;
	struct Node* right;
}*root =NULL, *top= NULL;

struct Node* newNode(int data) {
	struct Node* t = (struct Node*)malloc(sizeof(struct Node));
	t->data = data;
	t->left = t->right = NULL;
	return t;
}

struct Node *insert(struct Node *p, int key) {
	
	if (p == NULL) {
		return newNode(key);
	}
	
	if (key < p->data) {
		p->left = insert(p->left, key);
	}
	else if (key > p->data) {
		p->right = insert(p->right, key);
	}
	return p;
}

struct node* insertNeighborhood(struct Node* p, int n) {
	while(n>1) { 
		if (p == NULL) {
			return newNode(n);

		}
		for (int i = 1; i < N + 1; i++) {
			for (int j = 1; j < n + 1; j++) {
				if (adj_matrix[i][j] == 1 && j < (n / 2) + 1) {
					p->left = insertNeighborhood(p->left, (n / 2));
				}
				else if (adj_matrix[i][j] == 1 && j > (n / 2) + 1) {
					p->right = insertNeighborhood(p->right, n / 2);
				}
				return p;
			}
		}
	}
	

}

//post-order binary representation
void postOrder(struct Node* Node) {
	if (Node == NULL) {
		return;
	}
	if (Node != NULL) {
		postOrder(Node->left);
		postOrder(Node->right);
		printf("%d\n", Node->data);

	}
}


//in-order binary representation
void inOrder(struct Node* Node) {
	if (Node == NULL) {
		return;
	}
	if (Node != NULL) {
		inOrder(Node->left);
		printf("%d\n", Node->data);
		inOrder(Node->right);
	}
}


int main() {
	
	
	struct Graph* graph = createGraph(N);
			
	printf("load adjacency matrix:\n");
	load_adj_matrix();
	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
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

	logFunction();
	printf("\n");

	printf("print matrix of neighborhood tree:\n");
	createMatrix();
		
	root = insert(root, 50);
	insert(root, 30);
	insert(root, 20);
	insert(root, 40);
	insert(root, 70);
	insert(root, 60);
	insert(root, 80);
	printf("print inorder list for binary tree: \n");
	inOrder(root);
	printf("\n");

	printf("print inorder list for neighborhood tree: \n");
	int n = 8;
	top = insertNeighborhood(top, n);
	insertNeighborhood(top, 4);
	//insertNeighborhood(top, 2);
	inOrder(top);

	for (int i = N+1; i > 1; i = i / 2) {
		insertNeighborhood(top, i);
	}
	
	

	
	
	return 0;
}
 

/*
//function to convert String data type to integer data type
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

//fucntion to check if there is an edge between two vertices

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

//function to convert decimal number to binary number



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

struct Node* insertLevelOrder(int arr[], int i, int n) {
	struct Node* root = NULL;
	if (i < n) {
		root = newNode(arr[i]);
		root->left = insertLevelOrder(arr, 2 * i + 1, n);
		root->right = insertLevelOrder(arr, 2 * i + 2, n);

	}
	return root;
}

//main block


	int n = 17;
	printf("decimal 17 to binary: ");
	decToBinary(n);
	printf("\n");

	printf("converted array:\n");
	convertStringToInteger(fpointer);

*/

