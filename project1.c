#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <windows.h>


struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
};

struct AdjList
{
    // Pointer to head node of list
    struct AdjListNode *head;
};


struct Graph
{
    int V;
    struct AdjList *array;
};

struct MinHeapNode
{
    int v;
    int dist;
};

struct MinHeap
{
    int size;

    int capacity;

    int *pos;
    struct MinHeapNode **array;
};

void generateGraph(struct Graph *);
struct AdjListNode *newAdjListNode(int, int);
struct Graph *createGraph(int);
void addEdge(struct Graph *, int, int, int);
void delEdge(struct Graph *, int, int);
struct MinHeapNode *newMinHeapNode(int, int);
struct MinHeap *createMinHeap(int);
void swapMinHeapNode(struct MinHeapNode **, struct MinHeapNode **);
void minHeapify(struct MinHeap *, int);
int isEmpty(struct MinHeap *);
struct MinHeapNode *extractMin(struct MinHeap *);
void decreaseKey(struct MinHeap *, int, int);
int isInMinHeap(struct MinHeap *, int);
void dijkstra(struct Graph *, int, int);
void view_cities();
void insert_city(struct Graph *, char[]);
void insert_distances(struct Graph *, int);
void modify(struct Graph *, int, int, int);
void ShowAdjList(struct Graph *);
void printPath(int[], int);

char names[30][30] = {"UTTARKASHI", "TEHRI GARHWAL", "HARIDWAR", "PAURI GARHWAL", "DEHRADUN", "CHAMOLI", "RUDRAPRAYAG", "ALMORA", "NANITAL", "UDHAMSINGH_NAGAR","CHAMPAWAT","BAGESHWAR","PITHOGARH"};
int cities = 13;

void view_cities()
{
    system("cls");
    printf("\nCITY LIST:");
    
     for (int i = 0; i < cities; i++)
    {
        Sleep(50);
        printf("\n\n%d) %s", i + 1, names[i]);
    }
    printf("\n");
    Sleep(40);
}

void insert_city(struct Graph *graph, char name[])
{
    strcpy(names[cities - 1], name);

    printf("\nUpdating");
    for (int i = 0; i < 150; i += 25)
    {
        printf(".");
        Sleep(50);
    }
    printf("\n\nName added successfully.\n");
    insert_distances(graph, cities - 1);
}

void insert_distances(struct Graph *graph, int index)
{
    for (int i = 0; i < index; ++i)
    {
        int dist;
        printf("\nDistance of %s from %s (Type -1 if there is no direct connection): ", names[index], names[i]);
        scanf("%d", &dist);
        if (dist != -1)
            addEdge(graph, i, index, dist);
    }
    printf("\nUpdating");
    for (int i = 0; i < 100; i += 25)
    {
        printf(".");
        Sleep(50);
    }
    printf("\n\nDistances added successfully.\n");
}

void modify(struct Graph *graph, int a, int b, int dist)
{

    if (dist == -1)
    {
        delEdge(graph, a, b);
    }
    else
    {
        struct AdjListNode *temp = graph->array[a].head;
        while (temp->next && temp->next->dest != b)
        {
            temp = temp->next;
        }
        if (temp->next == NULL)
        {
            addEdge(graph, a, b, dist);
        }
        else
        {
            temp->next->weight = dist;
            temp = graph->array[b].head;
            while (temp->next && temp->next->dest != a)
            {
                temp = temp->next;
            }
            temp->next->weight = dist;
        }
    }

    printf("\nDistance successfully updated.");
    printf("\nReturning to main menu");
    for (int i = 0; i < 150; i += 25)
    {
        printf(".");
        Sleep(50);
    }
}

void delEdge(struct Graph *graph, int a, int b)
{
    struct AdjListNode *temp = graph->array[a].head;
    if (temp->dest == b)
    {
        graph->array[a].head = temp->next;
    }
    else
    {
        struct AdjListNode *d = temp->next;
        while (d && d->dest != b)
        {
            temp = d;
            d = d->next;
        }
        temp->next = d->next;
        free(d);
    }
    temp = graph->array[b].head;
    if (temp->dest == a)
    {
        graph->array[b].head = temp->next;
    }
    else
    {
        while (temp->next && temp->next->dest != a)
        {
            temp = temp->next;
        }
        struct AdjListNode *d = temp->next;
        temp->next = temp->next->next;
        free(d);
    }
}

void printPath(int path[], int j)
{
    if (path[j] == -1)
        return;

    printPath(path, path[j]);
    Sleep(500);
    printf("%s -> ", names[j]);
}

void ShowAdjList(struct Graph *graph)
{
    int disp[cities][cities];

    for (int i = 0; i < cities; ++i)
    {
        for (int j = 0; j < cities; ++j)
        {
            disp[i][j] = -1;
        }
    }

    for (int i = 0; i < cities; ++i)
    {
        struct AdjListNode *temp = graph->array[i].head;
        while (temp)
        {
            disp[i][temp->dest] = temp->weight;
            temp = temp->next;
        }
    }

    printf("            ");
    for (int i = 0; i < cities; ++i)
    {
        printf("%-15s ", names[i]);
    }
    for (int i = 0; i < cities; ++i)
    {
        Sleep(60);
        printf("\n\n%-15s ", names[i]);
        for (int j = 0; j < cities; ++j)
        {
            printf("%-15d ", disp[i][j]);
        }
    }
    printf("\n\n");
    Sleep(1000);
}

struct AdjListNode *newAdjListNode(int dest, int weight)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists. Size of array will be V
    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight)
{

    struct AdjListNode *newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

struct MinHeapNode *newMinHeapNode(int v, int dist)
{
    struct MinHeapNode *minHeapNode = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap *createMinHeap(int capacity)
{
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b)
{
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap *minHeap)
{
    return minHeap->size == 0;
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode *root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease dist value of a given vertex v. This function uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap *minHeap, int v, int dist)
{
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete tree is not hepified. This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

// The main function that calculates distances of shortest paths from src to all vertices. It is a O(ELogV) function
void dijkstra(struct Graph *graph, int src, int des)
{

    int path[cities];
    path[src] = -1;
    for (int i = 0; i < cities; ++i)
        path[i] = -1; 
    // Get the number of vertices in graph
    int V = graph->V;

    // dist values used to pick minimum weight edge in cut
    int dist[V];

    // minHeap represents set E
    struct MinHeap *minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode *minHeapNode = extractMin(minHeap);

        // Store the extracted vertex number
        int u = minHeapNode->v;

        // Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values
        struct AdjListNode *trav = graph->array[u].head;
        while (trav != NULL)
        {
            int v = trav->dest;

            // If shortest distance to v is not finalized yet, and distance to v through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && trav->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + trav->weight;
                path[v] = u;
                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            trav = trav->next;
        }
    }

    printf("\nCalculating");
    for (int i = 0; i < 150; i += 25)
    {
        printf(".");
        Sleep(60);
    }
    if (path[des] == -1)
        printf("\n\nNo direct route available !!!");
    else
    {
        printf("\n\nRoute: Source -> %s -> ", names[src]);
        printPath(path, des);
        printf("Destination");
        printf("\n\nTotal distance: %d km", dist[des]);
    }
    Sleep(100);
    printf("\n\nReturning to main menu");
    for (int i = 0; i < 150; i += 25)
    {
        printf(".");
        Sleep(100);
    }
}

void generateGraph(struct Graph *graph)
{
    addEdge(graph, 0, 3, 155);
    addEdge(graph, 0, 4, 140);
    addEdge(graph, 0, 5, 263);
    addEdge(graph, 0, 6, 173);
    
    addEdge(graph, 1, 0, 115);
    addEdge(graph, 1, 3, 159);
    addEdge(graph, 1, 4, 158);
    addEdge(graph, 1, 6, 113);

    addEdge(graph, 2, 3, 53);
    addEdge(graph, 2, 4, 160);

    addEdge(graph, 3, 1, 159);
    addEdge(graph, 3, 2, 160);
    addEdge(graph, 3, 4, 190);
    addEdge(graph, 3, 5, 241);
    addEdge(graph, 3, 6, 151);
    addEdge(graph, 3, 7, 239);
    addEdge(graph, 3, 8, 306);

    addEdge(graph, 4, 0, 140);
    addEdge(graph, 4, 1, 158);
    addEdge(graph, 4, 2, 190);
    addEdge(graph, 4, 3, 53);

    addEdge(graph, 5, 0, 263);
    addEdge(graph, 5, 3, 92);
    addEdge(graph, 5, 6, 241);
    addEdge(graph, 5, 7, 218);
    addEdge(graph, 5, 11, 168);
    addEdge(graph, 5, 12, 296);

    addEdge(graph, 6, 0, 173);
    addEdge(graph, 6, 1, 113);
    addEdge(graph, 6, 3, 151);
    addEdge(graph, 6, 5, 92);

    addEdge(graph, 7, 3, 239);
    addEdge(graph, 7, 5, 218);
    addEdge(graph, 7, 8, 64);
    addEdge(graph, 7, 10, 131);
    addEdge(graph, 7, 11, 73);
    addEdge(graph, 7, 12, 114);

    addEdge(graph, 8, 3, 306);
    addEdge(graph, 8, 7, 64);
    addEdge(graph, 8, 9, 73);
    addEdge(graph, 8, 10, 175);

    addEdge(graph, 9, 8, 73);
    addEdge(graph, 9, 10, 153);

    addEdge(graph, 10, 7, 131);
    addEdge(graph, 10, 8, 175);
    addEdge(graph, 10, 9, 159);
    addEdge(graph, 10, 12, 72);

    addEdge(graph, 11, 5, 168);
    addEdge(graph, 11, 7, 73);
    addEdge(graph, 11, 12, 130);

    addEdge(graph, 12, 5, 296);
    addEdge(graph, 12, 7, 114);
    addEdge(graph, 12, 10, 72);
    addEdge(graph, 12, 11, 130);

}

int main()
{

    for (int i = 0; i < 70; ++i)
    {
        printf("_");
        Sleep(30);
    }
    char title[] = "SHORTEST DISTANCE CALCULATOR.................by AYUSH BALODHI";
    printf("\n\n");
    for (int i = 0; i < sizeof title; ++i)
    {
        printf("%c", title[i]);
        Sleep(30);
    }
    printf("\n");
    for (int i = 0; i < 70; ++i)
    {
        printf("_");
        Sleep(30);
    }
    Sleep(30);
    struct Graph *graph = createGraph(cities);
    generateGraph(graph);
    char choice;
    do
    {
        printf("\n\nType the serial number you want to perform. Type 'Q' or 'q' to quit.\n\n");
        printf("1) Show available cities.\n\n2) Calculate shortest distance between two cities.\n\n3) Insert NEW city.\n\n4) Modify distance.\n\n5) Show Adjacency matrix.\n\nEnter your choice: ");
        fflush(stdin);
        scanf("%c", &choice);
        choice = tolower(choice);
        switch (choice)
        {
        case '1':
            view_cities();
            break;
        case '2':
            view_cities();
            int source, destination;
            printf("\nEnter SOURCE city number from the list: ");
            scanf("%d", &source);
            printf("%s\n", names[source - 1]);
            printf("Enter DESTINATION city number from the list: ");
            scanf("%d", &destination);
            printf("%s\n", names[destination - 1]);
            dijkstra(graph, source - 1, destination - 1);
            printf("\n");
            break;
        case '3':
            printf("\nEnter the name of the city you want to insert: ");
            char newname[25];
            scanf("%s", newname);
            cities++;
            graph = createGraph(cities);
            generateGraph(graph);
            insert_city(graph, newname);
            break;
        case '4':
            view_cities();
            printf("\nEnter the two city numbers for modification : ");
            int a, b;
            scanf("%d %d", &a, &b);
            printf("Enter new distance between city %s and %s (Type -1 if no direct connection exists): ", names[a - 1], names[b - 1]);
            int dist;
            scanf("%d", &dist);
            modify(graph, a - 1, b - 1, dist);
            break;
        case '5':
            ShowAdjList(graph);
            break;
        case 'q':
            break;
        default:
            printf("Please enter a valid input.\n");
        }

    } while (choice != 'q');
    printf("THANK YOU !!");
    return 0;
}
