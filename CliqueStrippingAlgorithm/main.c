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
int heightOfTree;
float delta = 1;
int k;


//function to calculate the height of a binary tree
int logFunction() {
	heightOfTree = log2(N);
	printf("log2 of N is: %d", heightOfTree);
	return heightOfTree;
	
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
int get_edges() {

	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	printf("%d", num_edges);
	return num_edges;

}

//function to calculate k
int get_k() {

	float denominator = (2 * pow((double)N, 2)) / num_edges;
	float numerator = delta * log2((double)N);
	k = floor((double)numerator / (log2((double)denominator)));
	printf("%d",k);
	return k;
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



/*********************************************************************/


struct node {
	int vertex;
	struct node* next;
};

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

/**********************************************************************/
//function to create matrix contains all neighborhood trees' list (level order)
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
	int rcount;
	int lcount;
}*root =NULL, *top= NULL;


struct Node* newNode(int data) {
	struct Node* t = (struct Node*)malloc(sizeof(struct Node));
	t->data = data;
	t->left = t->right = NULL;
	t -> rcount = t->lcount = 0;
	return t;
}

void deleteNode(struct Node* node) {
	if (node == NULL)
		return;
	if (node) {
		deleteNode(node->left);
		deleteNode(node->right);
		free(node);
	}
}

void deleteTreeWithRoot(struct Node** node_root) {
	deleteNode(*node_root);
	*node_root = NULL;
}

bool isBinaryTreeComplete(int count) {
	count = count + 1;
	while (count % 2 == 0) {
		count = count / 2;
	}
	if (count == 1) {
		return true;
	}
	else
		return false;

}

struct Node *insert(struct Node *root, int data) {
	
	if (root == NULL) {
		return newNode(data);
	}
	
	if (root->rcount == root->lcount) {
		root->left = insert(root->left, data);
		root->lcount += 1;
	}

	else if (root->rcount < root->lcount) {

		if (isBinaryTreeComplete(root->lcount)) {
			root->right = insert(root->right, data);
			root->rcount += 1;
		}
		else {
			root->left = insert(root->left, data);
			root->lcount += 1;
		}
	}
	return root;
}

//print nodes at current level
void printCurrentLevel(struct Node* root, int level) {
	if (root == NULL)
		return;
	if (level == 1) {
		printf("%d ", root->data);
	}
	else if (level > 1) {
		printCurrentLevel(root->left, level - 1);
		printCurrentLevel(root->right, level - 1);
	}
}

//function to compute the height of tree
int height(struct Node* node)
{
	if (node == NULL)
		return 0;
	else {
		/* compute the height of each subtree */
		int lheight = height(node->left);
		int rheight = height(node->right);

		/* use the larger one */
		if (lheight > rheight)
			return (lheight + 1);
		else
			return (rheight + 1);
	}
}

//level order binary tree representation
void levelOrder(struct Node* root) {
	int h = height(root);
	int i;
	for (i = 1; i <= h; i++) {
		printCurrentLevel(root, i);
		printf("\n");
	}
}

//function to print all neighborhood tree in level order
void printLevelOrder() {
	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < 2 * N; j++) {
			root = insert(root, matrix[i][j]);
		}
		levelOrder(root); 
		deleteTreeWithRoot(&root);
	}
}



/********************************Clique Stripping Algorithm**********************/
int t = 1;
int getCZero() {
	int c_0 = 0;
	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; i < 2 * N; j++) {
			root = insert(root, matrix[i][j]);
		}
		levelOrder(root);
		c_0 = c_0 + (root->left->data) * pow((root->data), (k - t));
		deleteTreeWithRoot(&root);
	}
	printf("c_0 is %d: \n", c_0);
	return c_0;
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
	printf("\n");
	
	printf("get edges:\n");
	get_edges();
	printf("\n");

	printf("get degree:\n");
	nodeDegree();
	printf("\n");

	printf("value of k is: \n");
	get_k();
	printf("\n");

	logFunction();
	printf("\n");

	printf("print matrix of neighborhood tree:\n");
	createMatrix();
		
	printf("print level order of all binary neighborhood tree:\n");
	printLevelOrder();
	printf("\n");

	printf("print c0:\n");
	getCZero();
	
	
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

//post-order binary tree representation
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


//in-order binary tree representation
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

//main block


	int n = 17;
	printf("decimal 17 to binary: ");
	decToBinary(n);
	printf("\n");

	printf("converted array:\n");
	convertStringToInteger(fpointer);
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

*/

