#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char character;
    int frequency;
    struct Node *left, *right;
} Node;

typedef struct MinHeap
{
    int size;
    int capacity;
    Node **array;
} MinHeap;

Node* createNode(char character, int frequency);
MinHeap* createMinHeap(int capacity);
void insertMinHeap(MinHeap *minHeap, Node *node);
Node* extractMin(MinHeap *minHeap);
void buildMinHeap(MinHeap *minHeap);
void printCodes(Node *root, int arr[], int top);
void HuffmanCoding(char *text);
void calculateFrequency(char *text, int *frequency);

int main()
{
    char text[1000];

    printf("Enter word to compress: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    HuffmanCoding(text);

    return 0;
}

Node* createNode(char character, int frequency)
{
    Node *node = (Node*)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// A min heap
MinHeap* createMinHeap(int capacity)
{
    MinHeap *minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (Node**)malloc(minHeap->capacity * sizeof(Node*));
    return minHeap;
}

// Adding a node into the min-heap
void insertMinHeap(MinHeap *minHeap, Node *node)
{
    minHeap->array[minHeap->size] = node;
    int i = minHeap->size;
    while (i > 0 && minHeap->array[(i - 1) / 2]->frequency > minHeap->array[i]->frequency) {
        Node *temp = minHeap->array[i];
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        minHeap->array[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
    minHeap->size++;
}

Node* extractMin(MinHeap *minHeap)
{
    Node *minNode = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    buildMinHeap(minHeap);
    return minNode;
}

void buildMinHeap(MinHeap *minHeap)
{
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; i--) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
            smallest = left;
        if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
            smallest = right;

        if (smallest != i) {
            Node *temp = minHeap->array[i];
            minHeap->array[i] = minHeap->array[smallest];
            minHeap->array[smallest] = temp;
            i = smallest;
        }
    }
}

void calculateFrequency(char *text, int *frequency)
{
    for (int i = 0; text[i] != '\0'; i++)
	{
        frequency[(unsigned char)text[i]]++;
    }
}

// Printing the Huffman
void printCodes(Node *root, int arr[], int top)
{
    if (root->left)
	{
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right)
	{
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!root->left && !root->right)
	{
        printf("%c: ", root->character);
        for (int i = 0; i < top; i++)
		{
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

// Huffman function
void HuffmanCoding(char *text)
{
    int frequency[256] = {0};
    calculateFrequency(text, frequency);

    MinHeap *minHeap = createMinHeap(strlen(text));
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            insertMinHeap(minHeap, createNode(i, frequency[i]));
        }
    }

    while (minHeap->size > 1) {
        Node *left = extractMin(minHeap);
        Node *right = extractMin(minHeap);
        Node *top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    int arr[100], top = 0;
    printf("\nHuffman Codes:\n");
    printCodes(minHeap->array[0], arr, top);
}

