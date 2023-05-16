/* Comments are added based on the Clique Partitioning (CPA) and 
Clique Stripping algorithm (CSA) in Feder and Motwani's paper*/

/*On windows follow this instruction to get a running time:
��1.clock_t start = clock();
��2.clock_t stop = clock();
��3.elapsed = ((double)(stop - start)) / CLOCKS_PER_SEC * 1000.0;
 */

/*On Grid follow this instruction to get a running time:
0. struct timespec begin, end;
1.clock_gettime(CLOCK_REALTIME, &begin);
2.clock_gettime(CLOCK_REALTIME, &end);    �����
3.long seconds = end.tv_sec - begin.tv_sec;
4.long nanoseconds = end.tv_nsec - begin.tv_nsec;
5.elapsed = (seconds + nanoseconds * 1e-9) * 1000;
*/


#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>
#include<stdint.h>
//#include<direct.h>
#define MAXCHAR 1000000
#define N 513

FILE* k_hat_file_ptr;  // Stores k_hat values
FILE* compression_ratio_ptr;	// Stores compression ratio
FILE* time_elapsed_ptr;		// Stores execution time
FILE* file; // Stores the name of the .csv file to read
FILE* results;	// Stores the results


int graph_nodes;
int adj_matrix[N][N];  // Given adjacency matrix
int adj_matrix_fix[N][N];	
int matrix[(2 * N) - 1][(2 * N) - 1]; // Stores level order of all binary trees
int num_edges; //final result of m_hat function
int degree;	
int heightOfTree;
float delta = 1;
int k;	// k_hat
long long int  c_zero = 0;
long long int  c_one = 0;
int start = 0;
int U[2 * N][N + 1];	// Stores left partition of the delta-clique
int K[2 * N][N + 1];	// Stores right partition of the delta-clique
int	q;	// this is clique number
int m_hat_fix = 0;	// Initial edges in the given graph
float compression_ratio;
char f_name[100];
double elapsed_time;
struct timespec begin, end;



//function to calculate the height of a binary tree
int logFunction() {
	heightOfTree = log2(N);
	printf("log2 of N is: %d", heightOfTree);
	return heightOfTree;

}

//function to load adjacency matrix from csv file
void load_adj_matrix() {

	char line[MAXCHAR];
	file = fopen(f_name, "r");
	if (!file)
		printf("Cann't open the file\n");
	else {
		int row = 1;
		int col = 1;
		while (fgets(line, MAXCHAR, file)) {
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
		fclose(file);
	}
}

//function to count number of edges in graph G
/* Algo CPA step 1 and step 3.3*/
int m_hat() {
	int i, j;
	for (i = 1; i < graph_nodes + 1; i++) {
		for (j = 1; j < graph_nodes + 1; j++) {
			num_edges = num_edges + adj_matrix[i][j];
		}
	}
	m_hat_fix = num_edges;
	return num_edges;
}


//function to calculate k_hat 
/* Algo CPA: step 3.1 */
int get_k(int numberOfEdges) {
	double denominator = (2 * pow((double)graph_nodes, 2)) / num_edges;
	double numerator = delta * log2((double)graph_nodes);
	k = floor(numerator / (log2(denominator)));
	return k;
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
	graph->numVertices = graph_nodes;
	graph->adjList = malloc(graph_nodes * sizeof(struct node*));
	int i;
	for (i = 1; i < graph_nodes + 1; i++)
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
/* Lemma 2.2*/
void createMatrix() {
	int i, j;
	for (i = 1; i < graph_nodes + 1; i++) {
		for (j = (2 * graph_nodes) - 1; j > 0; j--) {
			if (j < graph_nodes + 1) {
				matrix[i][j + (graph_nodes - 1)] = adj_matrix[i][j];
			}
		}
	}
	for (i = 1; i < graph_nodes + 1; i++) {
		for (j = graph_nodes - 1; j > 0; j--) {
			matrix[i][j] = matrix[i][2 * j] + matrix[i][(2 * j) + 1];
		}
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
	for (i = 1; i < graph_nodes + 1; i++) {
		for (j = 1; j < 2 * graph_nodes; j++) {
			root = insert(root, matrix[i][j]);
		}
		levelOrder(root);
		deleteTreeWithRoot(&root);
	}
}


/********************************Clique Stripping Algorithm**********************/
//function to find c0, c1, selected path and corresponding string w.
/* Algo CSA: Steps 3.2 and 3.3 */
void find_czero_cone() {
	int i;
	int t = 1; int j = 1;
	int count = 0;
	while (j < graph_nodes) {
		for (i = 1; i < graph_nodes + 1; i++) {
			c_zero = c_zero + (matrix[i][2 * j]) * pow((matrix[i][1] - 1), (k - t));
			c_one = c_one + (matrix[i][(2 * j) + 1]) * pow((matrix[i][1] - 1), (k - t));
		}
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
}


//function to print all c0 and c1 in all levels
void printAll_czero_cone() {
	int t = 1; int j = 1;
	int i;
	while (j < graph_nodes) {
		if (newRoot == NULL) {
			newRoot = insert(newRoot, graph_nodes);
		}
		else {

			for (i = 1; i < graph_nodes + 1; i++) {
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


/* Prints the selected vertices on the path */
void printSelectedIndices() {
	int t = 1; int j = 1;
	int i;
	int size = 0;
	int arr[1000];
	while (j < graph_nodes) {
		for (i = 1; i < graph_nodes + 1; i++) {
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

/* Used in find_ncr function */
long double factorial(int n) {
	if (n == 0)
		return 1;
	long double factorial = 1;
	for (int i = 2; i <= n; i++)
		factorial = factorial * (long double)i;
	return factorial;
}

/* Used to calculate the number of distinct ordered sets */
long double find_ncr(int n, int r) {
	long double  result;
	result = factorial(n) / (factorial(r) * factorial(n - r));
	return result;
}

/* Prints the index of selected vertex */
void printLastIndex() {
	int t = 1; int j = 1;
	int i;
	int size = 0;
	int last_index = 0;
	int arr[1000];
	while (j < graph_nodes) {
		for (i = 1; i < graph_nodes + 1; i++) {
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

/* Algo CSA */
void runCliqueStrippingAlgorithm() {
	//int t = 1;
	int i, j;
	int size = 0;
	int count = 0;
	int last_index = 0;
	int arr[1000] = { 0 };
	int finalArrayIndex[2000] = { 0 };
	int sumMatrix;
	/* Algo CPA: Step 3 */
	while (k > 1 && num_edges >= pow(graph_nodes, (2 - delta))) {
		sumMatrix = 0;
		int t = 1;
		/* Algo CSA: Step 6 */
		while (t <= k) {
			c_zero = c_one = 0;
			j = 1;
			size = 0;
			while (j < graph_nodes) {
				for (i = 1; i < graph_nodes + 1; i++) {
					long double temp = find_ncr((matrix[i][1] - 1), (k - t));
					c_zero = c_zero + (matrix[i][2 * j]) * temp;
					c_one = c_one + (matrix[i][(2 * j) + 1]) * temp; 
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
			last_index = arr[size - 1];
			for (i = 0; i < size; i++) {
				arr[i] = 0;
			}

			int l = 0;
			int counter = 0;
			l = last_index - (graph_nodes - 1);

			/* Algo CSA: Step 5, Updating neighbourhood trees*/
			for (i = 1; i < graph_nodes + 1; i++) {
				matrix[i][l + (graph_nodes - 1)] = 0; 
				sumMatrix += matrix[i][1];
				for (j = last_index; j > 0; j = (int)floor(j / 2)) {
					matrix[i][j] = matrix[i][2 * j] + matrix[i][(2 * j) + 1];
				}
			}
			finalArrayIndex[count] = l;
			count++;
			struct Graph* bipartite = createGraph();
			for (j = 0; j < count; j++) {
				for (i = 1; i < graph_nodes + 1; i++) {
					if (adj_matrix_fix[i][finalArrayIndex[j]] != 0) {
						addEdge(bipartite, finalArrayIndex[j], i);
					}
				}
			}
			t++;
		}
		int p = 0;  // gets the common neighbours of the clique q. p == |U_k|; Size of left partition of the clique
		for (j = start; j < count; j++) {
			K[q][j - start] = finalArrayIndex[j];
		}
		K[q][k] = -1;
		for (i = 1; i < graph_nodes + 1; i++) {
			int temp = 0; // get the number of edges of u_i with the selected vertices in the right partition
			for (j = start; j < count; j++) {
				temp += adj_matrix[i][finalArrayIndex[j]];
			}
			/* Check if u_i has an edge with all the selected vertices in the right partition */
			if (temp == k) {
				for (j = start; j < count; j++) {
					adj_matrix[i][finalArrayIndex[j]] = 0;
				}
			}

		}


		for (i = 1; i < graph_nodes + 1; i++) {
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
		/* Algo CPA: Step 3.3, updating m_hat */
		num_edges -= k * p;
		k = get_k(num_edges);
		q++;
		start = count;
		if (sumMatrix == 0)
			break;
	}
}

/* Functino to calculate the compression ratio */
void get_remaining_edges() {
	int p, j, i;
	int edges = 0; // Edges removed from the given graph
	int edge_in_clique = 0; // Edges in all cliques
	int total_edges = 0; // Trivial edges and  edges in the clique 
	int trivial = 0;
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
	trivial = m_hat_fix - edges;
	total_edges = num_edges + edge_in_clique;
	compression_ratio = (float)m_hat_fix / (float)total_edges;
	printf("\ncompression_ratio: %f Initial Edges: %d Total Edges: %d Edges Clique: %d Trivial: %d Num_Edges: %d\n", compression_ratio, m_hat_fix, total_edges, edge_in_clique, trivial, num_edges);
}

void runCliquePartitioningAlgorithm() {
	struct Graph* graph = createGraph();

	load_adj_matrix();
	int i, j;
	for (i = 1; i < graph_nodes + 1; i++) {
		for (j = 1; j < graph_nodes + 1; j++) {
			if (adj_matrix[j][i] == 1) {
				addEdge(graph, i, j);
			}
		}
	}
	 clock_t start = clock();
	//clock_gettime(CLOCK_REALTIME, &begin);
	m_hat();
	get_k(num_edges);
	createMatrix();
	runCliqueStrippingAlgorithm();
	clock_t stop = clock();
	//clock_gettime(CLOCK_REALTIME, &end);
	//long seconds = end.tv_sec - begin.tv_sec;
	//long nanoseconds = end.tv_nsec - begin.tv_nsec;
	//elapsed_time = (seconds + nanoseconds * 1e-9) * 1000;
	elapsed_time = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("Time elapsed for original clique partitioning algorithm in ms: %lf", elapsed_time);
	get_remaining_edges();
}

int main(int argc, char* argv[]) {
	char cores[5] = "fm";
	int nodes =  atoi(argv[1]);
	graph_nodes = nodes; 
	int density =  atoi(argv[2]);
	int exp = atoi(argv[3]);
	num_edges = 0;
	q = 0;
	printf("____Density: %d _______ Experimtent: %d ______________", density, exp);
	sprintf(f_name, "New_generated_data/Bipartite_%dX%d/%d/Bipartite_%dX%d_%d_%d.csv", nodes, nodes, density, nodes, nodes, density, exp);
	runCliquePartitioningAlgorithm();
	printf("%d,%d,%d,%lf, %lf,%s\n", nodes, density, exp, compression_ratio, elapsed_time, cores);

	return 0;
}