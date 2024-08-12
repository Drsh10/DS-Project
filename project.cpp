#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structure name parcel node is Define for  the Binary Search Tree
typedef struct ParcelNode
{
    char* destination;
    int weight;
    float valuation;
    struct ParcelNode* left;
    struct ParcelNode* right;
} ParcelNode;

// Structure to Name HashTable is Define for a Hash Table
typedef struct HashTable
{
    ParcelNode* buckets[127];
} HashTable;

unsigned long hash_djb2(char* str);
ParcelNode* createParcelNode(const char* destination, int weight, float valuation);
ParcelNode* insertParcelNode(ParcelNode* root, char* destination, int weight, float valuation);
void insertParcel(HashTable* hashTable, char* destination, int weight, float valuation);
ParcelNode* searchByWeight(ParcelNode* root, int weight);
ParcelNode* findMin(ParcelNode* root);
ParcelNode* findMax(ParcelNode* root);
ParcelNode* findMinValuation(ParcelNode* root);
ParcelNode* findMaxValuation(ParcelNode* root);
void displayParcels(ParcelNode* root);
void displayParcelsByWeight(ParcelNode* root, int weight, int higher);
void calculateTotals(ParcelNode* root, int* totalWeight, float* totalValuation);
void displayMenu(HashTable* hashTable);
void freeBST(ParcelNode* root);
void freeHashTable(HashTable* hashTable);

int main()
{
    // Initialize the hash table with all buckets set to NULL
    HashTable hashTable = { { NULL } };
    // Open the file "couriers.txt" for reading
    FILE* file;
    errno_t err = fopen_s(&file, "couriers.txt", "r");
    if (!file)
    {
        // Print an error message if the file could not be opened
        printf("Could not open couriers.txt file.\n");
        return 1;   // Exit the program with an error code
    }
    // Buffer to store the destination of the parcel
    char destination[20];
    // Variable to store the weight of the parcel
    int weight;
    // Variable to store the valuation of the parcel
    float valuation;
    // Read each line from the file and parse the parcel details
    while (fscanf_s(file, "%[^,], %d, %f\n", destination, (unsigned)_countof(destination), &weight, &valuation) != -1)
    {
        // Insert the parcel details into the hash table
        insertParcel(&hashTable, destination, weight, valuation);
    }
    // Close the file after reading
    fclose(file);
    // Display the menu to the user and handle their choices
    displayMenu(&hashTable);
    // Free the allocated memory for the hash table and its contents
    freeHashTable(&hashTable);
    // Exit the program successfully

    return 0;
}
//
// FUNCTION: hash_djb2
//DESCRIPTION:
// The  Function is created To assign hash value using djb2 hash function
//PARAMETERS: char* str
//
unsigned long hash_djb2(char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 127;
}
//
// FUNCTION:createParcelNode
//DESCRIPTION:
// Function is created to create a new parcel node
//PARAMETERS: const char* destination, int weight, float valuation
// Returns: the function returns a pointer to the newly created ParcelNode.
//
ParcelNode* createParcelNode(const char* destination, int weight, float valuation)
{
    // Allocate memory for a new ParcelNode structure
    ParcelNode* newNode = (ParcelNode*)malloc(sizeof(ParcelNode));
    if (newNode == NULL)
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    // Allocate memory for the destination string, including the null terminator
    newNode->destination = (char*)malloc(strlen(destination) + 1);
    if (newNode->destination == NULL)
    {
        perror("Failed to allocate memory for destination");
        free(newNode);      // Free the node memory to prevent a memory leak
        exit(EXIT_FAILURE);
    }
    // Copy the destination string to the allocated memory, ensuring it's null-terminated
    strcpy_s(newNode->destination, strlen(destination) + 1, destination);

    // Assign the weight and valuation to the new node
    newNode->weight = weight;
    newNode->valuation = valuation;
    // Initialize the left and right pointers to NULL, indicating no child nodes
    newNode->left = newNode->right = NULL;

    return newNode;
}