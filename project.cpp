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