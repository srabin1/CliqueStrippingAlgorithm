#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>
#define MAXCHAR 100000
#define N 8
FILE* k_hat_file_ptr;


int adj_matrix[N][N];
int adj_matrix_fix[N][N];
int matrix[(2 * N) - 1][(2 * N) - 1];
int u_[N];
int v_[N];
int num_edges = 0; //final result of m_hat function
int degree;
int heightOfTree;
float delta = 1;
int k;
int c_zero = 0;
int c_one = 0;
int start = 0;
int U[2 * N][N + 1];
int K[2 * N][N + 1];
int	q = 0;// this is clique number
int m_hat_fix = 0;

//function to calculate the height of a binary tree
int logFunction() {
	heightOfTree = log2(N);
	printf("log2 of N is: %d", heightOfTree);
	return heightOfTree;

}

//function to load adjacency matrix from csv file and print it
void load_adj_matrix() {
	FILE* fpointer = fopen("Bipartite-graph2.csv", "r");
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
				adj_matrix_fix[row][col] = atoi(value);
				value = strtok(NULL, ",");
				col++;
			}
			row++;

		}
		fclose(fpointer);
	}
	int i, j;
	for (i = 1; i < N + 1; i++) {
		for (j = 1; j < N + 1; j++) {
			printf(" %d ", adj_matrix[i][j]);
		}
		printf("\n");
	}

}

//function to count number of edge in graph G
int m_hat() {
	int i, j;
	for (i = 1; i < N + 1; i++) {
		for (j = 1; j < N + 1; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	m_hat_fix = num_edges;
	printf("%d", num_edges);
	return num_edges;

}

bool isBoolean() {
	if (num_edges >= pow(N, (2 - delta))) {
		return true;
	}
	else
		return false;

}

//function to calculate k
int get_k(int numberOfEdges) {

	double denominator = (2 * pow((double)N, 2)) / num_edges;
	double numerator = delta * log2((double)N);
	k = floor(numerator / (log2(denominator)));
	printf("%d", k);
	return k;
}

//function to calculate degree of each vertex in part U
void getDegreeOfU() {
	int i, j;
	for (i = 1; i < N + 1; i++) {
		degree = 0;
		for (j = 1; j < N + 1; j++) {
			degree = degree + adj_matrix[i][j];
		}
		printf("degree of u[%d] is: %d\n", i, degree);

	}
}

void getDegreeOfV() {
	int j, i;
	for (j = 1; j < N + 1; j++) {
		degree = 0;
		for (i = 1; i < N + 1; i++) {
			degree = degree + adj_matrix[i][j];
		}
		printf("degree of v[%d] is: %d\n", j, degree);

	}
}

/*******************************************************************/

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
		printf("Vertex v[%d]: ", v);
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
	int i, j;
	for (i = 1; i < N + 1; i++) {
		for (j = (2 * N) - 1; j > 0; j--) {
			if (j < N + 1) {
				matrix[i][j + (N - 1)] = adj_matrix[i][j];
			}
		}
	}
	for (i = 1; i < N + 1; i++) {
		for (j = N - 1; j > 0; j--) {
			matrix[i][j] = matrix[i][2 * j] + matrix[i][(2 * j) + 1];
		}
	}


	for (i = 1; i < N + 1; i++) {
		for (j = 1; j < 2 * N; j++) {
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
}*root = NULL, * newRoot = NULL;

//function to create a new node
struct Node* newNode(int data) {
	struct Node* t = (struct Node*)malloc(sizeof(struct Node));
	t->data = data;
	t->left = t->right = NULL;
	t->rcount = t->lcount = 0;
	return t;
}

//function to delete all nodes of a tree
void deleteNode(struct Node* node) {
	if (node == NULL)
		return;
	if (node) {
		deleteNode(node->left);
		deleteNode(node->right);
		free(node);
	}
}

//function to delete all nodes including root node
void deleteTreeWithRoot(struct Node** node_root) {
	deleteNode(*node_root);
	*node_root = NULL;
}

//function to check if binary tree is complete
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

//function to insert new node according to the complete binary tree representation
struct Node* insert(struct Node* root, int data) {

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
	int i, j;
	for (i = 1; i < N + 1; i++) {
		for (j = 1; j < 2 * N; j++) {
			root = insert(root, matrix[i][j]);
		}
		levelOrder(root);
		deleteTreeWithRoot(&root);
	}
}


/********************************Clique Stripping Algorithm**********************/
//function to find c0 and c1
void find_czero_cone() {
	int i;
	int t = 1; int j = 1;
	int count = 0;
	while (j < N) {
		for (i = 1; i < N + 1; i++) {
			c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
			c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
		}
		printf("print c0: %d\n", c_zero);
		printf("print c1: %d\n", c_one);

		if (c_zero >= c_one) {
			c_zero = c_one = 0;
			j = 2 * j;
		}
		else {
			c_zero = c_one = 0;
			j = (2 * j) + 1;
		}
		count++;
	}
	printf("count: %d\n", count);
}

//function to find the selected path and corresponding string w
void printSelectedString() {
	int t = 1; int j = 1;
	int i;
	while (j < N) {
		for (i = 1; i < N + 1; i++) {
			c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
			c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
		}

		if (c_zero >= c_one) {
			printf("print c0: %d\n", c_zero);
			c_zero = c_one = 0;
			j = 2 * j;
		}
		else {
			printf("print c1: %d\n", c_one);
			c_zero = c_one = 0;
			j = (2 * j) + 1;
		}
	}
}

//function to print all c0 and c1 in all levels
void printAll_czero_cone() {
	int t = 1; int j = 1;
	int i;
	while (j < N) {
		if (newRoot == NULL) {
			newRoot = insert(newRoot, N);
		}
		else {

			for (i = 1; i < N + 1; i++) {
				c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
				c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
			}
			newRoot->left = insert(newRoot->left, c_zero);
			newRoot->right = insert(newRoot->right, c_one);
			c_zero = c_one = 0;
			j++;
		}
	}
	levelOrder(newRoot);
}

void printSelectedIndices() {
	int t = 1; int j = 1;
	int i;
	int size = 0;
	int arr[1000];
	while (j < N) {
		for (i = 1; i < N + 1; i++) {
			c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
			c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
		}

		if (c_zero >= c_one) {
			c_zero = c_one = 0;
			j = 2 * j;
			arr[size] = j;
		}
		else {
			c_zero = c_one = 0;
			j = (2 * j) + 1;
			arr[size] = j;
		}
		size++;
	}
	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
}


void printLastIndex() {
	int t = 1; int j = 1;
	int i;
	int size = 0;
	int last_index = 0;
	int arr[1000];
	while (j < N) {
		for (i = 1; i < N + 1; i++) {
			c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
			c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
		}

		if (c_zero >= c_one) {
			c_zero = c_one = 0;
			j = 2 * j;
			arr[size] = j;
		}
		else {
			c_zero = c_one = 0;
			j = (2 * j) + 1;
			arr[size] = j;
		}
		size++;
	}
	for (i = 0; i < size; i++) {
		if (i = size - 1) {
			last_index = arr[i];
		}
		printf("%d ", last_index);
	}

}

void runCliqueStrippingAlgorithm() {
	//int t = 1;
	int i, j;
	int size = 0;
	int count = 0;
	int last_index = 0;
	int arr[2000] = { 0 };
	int finalArrayIndex[2000] = { 0 };
	while (k > 1 && num_edges >= pow(N, (2 - delta))) {
		int t = 1;

		while (t <= k) {
			c_zero = c_one = 0;
			j = 1;
			size = 0;

			while (j < N) {
				for (i = 1; i < N + 1; i++) {
					long long int temp = pow((matrix[i][1] - 1), (k - t));
					c_zero = c_zero + (matrix[i][2 * j]) * temp;// pow((matrix[i][1] - 1), (k - t));
					c_one = c_one + (matrix[i][(2 * j) + 1]) * temp; // pow((matrix[i][1] - 1), (k - t));
				}
				printf("print c0: %d\n", c_zero);
				printf("print c1: %d\n", c_one);

				if (c_zero >= c_one) {
					c_zero = c_one = 0;
					j = 2 * j;
					arr[size] = j;
				}
				else {
					c_zero = c_one = 0;
					j = (2 * j) + 1;
					arr[size] = j;
				}
				size++;
			}
			last_index = arr[size - 1];
			printf("last index: %d", last_index);
			printf("\n");
			for (i = 0; i < size; i++) {
				arr[i] = 0;
			}
			for (i = 0; i < size; i++) {
				printf("%d", arr[i]);
			}
			printf("\n");

			int l = 0;
			int counter = 0;
			printf("\n");
			for (i = 1; i < N + 1; i++) {
				l = last_index - (N - 1);
				if (adj_matrix[i][l] != 0) {
					adj_matrix[i][l] = 0;
					counter++;
				}

			}
			//printf("print counter %d", counter);
			num_edges -= counter;
			printf("\n");
			printf("print updated number of edges %d", num_edges);
			printf("\n");
			createMatrix();

			for (i = 1; i < N + 1; i++) {
				l = last_index - (N - 1);
				finalArrayIndex[count] = l;
				//K[q][count] = l;
			}

			count++;
			struct Graph* bipartite = createGraph();
			for (j = 0; j < count; j++) {
				for (i = 1; i < N + 1; i++) {
					if (adj_matrix_fix[i][finalArrayIndex[j]] != 0) {
						addEdge(bipartite, finalArrayIndex[j], i);
					}
				}
			}

			printf("load adjacency list for bipartite graph :\n");
			printGraph(bipartite);
			printf("\n");
			printf("print final array index: ");
			for (i = 0; i < count; i++) {
				printf("%d ", finalArrayIndex[i]);
			}

			printf("\n");
			t++;

			//printf("print k-hat %d", k);
		}
		int p = 0;
		printf("K:");
		for (j = start; j < count; j++) {
			K[q][j - start] = finalArrayIndex[j];
			printf(" %d", K[q][j - start]);
		}
		K[q][count] = -1;
		printf("\n");


		for (i = 1; i < N + 1; i++) {
			int temp = 0;
			for (j = start; j < count; j++) {
				if (adj_matrix_fix[i][finalArrayIndex[j]] == 1) {
					temp++;
				}

			}
			if (temp == count - start) {
				U[q][p] = i;
				p++;
			}

		}
		U[q][p] = -1;
		printf("U: ");
		for (p = 0; p < N + 1; p++) {
			printf("%d ", U[q][p]);
		}
		printf("\n");

		printf("k is: ");
		k = get_k(num_edges);
		printf("\n");
		printf("Edges: %d", num_edges);
		q++;
		start = count;


	}
}


void get_remaining_edges() {
	int p, j, i;
	int edges = 0;
	int edge_in_clique = 0;
	int total_edges = 0;
	for (p = 0; p < q; p++) {
		j = 0;
		while (K[p][j] > 0) {
			i = 0;
			while (U[p][i] > 0) {
				edges++;
				i++;
			}
			j++;
		}
		edge_in_clique += (i)+(j);
	}
	int trivial = m_hat_fix - edges;
	printf("\nTrivial: %d \n", trivial);
	printf("\nedge_in_clique: %d \n", edge_in_clique);
	total_edges = trivial + edge_in_clique;
	float compression_ratio = (float)m_hat_fix / (float)total_edges;
	printf("\ncompression_ratio: %f \n", compression_ratio);
}

int main() {
	k_hat_file_ptr = fopen("k_values.csv", "w");
	clock_t start = clock();
	struct Graph* graph = createGraph();

	printf("load adjacency matrix:\n");
	load_adj_matrix();
	int i, j;
	for (i = 1; i < N + 1; i++) {
		for (j = 1; j < N + 1; j++) {
			if (adj_matrix[j][i] == 1) {
				addEdge(graph, i, j);
			}

		}
	}


	printf("load adjacency list for v:\n");
	printGraph(graph);
	printf("\n");



	printf("number of edges:\n");
	m_hat();
	printf("\n");



	printf("get degree of u:\n");
	getDegreeOfU();
	printf("\n");

	printf("get degree of v:\n");
	getDegreeOfV();
	printf("\n");

	printf("value of k is: \n");
	get_k(num_edges);
	printf("\n");

	printf("value of k is: \n");
	get_k(num_edges);
	fprintf(k_hat_file_ptr, "%d\n", k);
	printf("\n");

	logFunction();
	printf("\n");

	printf("print matrix of neighborhood tree:\n");
	createMatrix();
	printf("\n");

	printf("print level order of all binary neighborhood tree:\n");
	printLevelOrder();
	printf("\n");

	printf("print c0 and c1:\n");
	find_czero_cone();
	printf("\n");

	printf("print all c0 and c1:\n");
	printAll_czero_cone();
	printf("\n");

	printf("print selected string w:\n");
	printSelectedString();
	printf("\n");

	printf("print string selected indices: \n");
	printSelectedIndices();
	printf("\n");

	printf("print string last index: \n");
	printLastIndex();
	printf("\n");

	printf("print update binary tree: \n");
	runCliqueStrippingAlgorithm();
	printf("\n");


	/*printf("check out boolean function:\n");
	if (isBoolean()) {
		printf("the result is true");
	}
	else
		printf("result is false");

	printf("\n");*/
	clock_t stop = clock();
	double elapsed_time = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time elapsed for original clique partitioning algorithm in ms: %f", elapsed_time);
	fclose(k_hat_file_ptr);
	get_remaining_edges();
	return 0;
}