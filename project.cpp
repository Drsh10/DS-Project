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

//
// FUNCTION:insertParcelNode
//DESCRIPTION:
// Function  created to  insert a parcel node into the Binary Search Tree
//PARAMETERS: ParcelNode* root, char* destination, int weight, float valuation
// Returns: the function returns the root of the tree, which represents the top of the binary search tree
//
ParcelNode* insertParcelNode(ParcelNode* root, char* destination, int weight, float valuation)
{
    // If the tree is empty, create a new node as the root
    if (root == NULL)
    {
        return createParcelNode(destination, weight, valuation);
    }
    // If the weight of the new parcel is less than the current node's weight,
    // insert the new node in the left subtree
    if (weight < root->weight)
    {
        root->left = insertParcelNode(root->left, destination, weight, valuation);
    }
    // If the weight of the new parcel is greater than or equal to the current node's weight,
    // insert the new node in the right subtree
    else
    {
        root->right = insertParcelNode(root->right, destination, weight, valuation);
    }
    return root;
}

//
// FUNCTION:insertParcel
//DESCRIPTION:
// Function is created to insert the parcel into a hash table
//PARAMETERS: HashTable* hashTable, char* destination, int weight, float valuation
//
void insertParcel(HashTable* hashTable, char* destination, int weight, float valuation)
{
    // Calculate the index in the hash table using a hash function on the destination string
    unsigned long index = hash_djb2(destination);

    // Insert the parcel node into the binary search tree located at  index
    hashTable->buckets[index] = insertParcelNode(hashTable->buckets[index], destination, weight, valuation);
}
//
// FUNCTION: searchByWeight
// DESCRIPTION:
//   Function to find a parcel node by its weight in a binary search tree
// PARAMETERS: ParcelNode* root, int weight
// RETURNS: ParcelNode* (the found node or NULL if not found)
//
ParcelNode* searchByWeight(ParcelNode* root, int weight)
{
    // If the current node is NULL or its weight matches the searched weight, return the node
    if (root == NULL || root->weight == weight)
    {
        return root;
    }
    // If the searched weight is less than the current node's weight, search in the left subtree
    if (weight < root->weight)
    {
        return searchByWeight(root->left, weight);
    }
    // If the searched weight is greater than the current node's weight, search in the right subtree
    else
    {
        return searchByWeight(root->right, weight);
    }
}


//
// FUNCTION:FindMin
//DESCRIPTION:
// Function  is created to find Minimum value of Weight node in BST
//PARAMETERS: ParcelNode* root
// RETURNS: ParcelNode* (the node with the minimum weight)
//
ParcelNode* findMin(ParcelNode* root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

//
// FUNCTION:FindMax
//DESCRIPTION:
// Function  is created to find Minimum value of Weight node in BST
//PARAMETERS: ParcelNode* root
//
ParcelNode* findMax(ParcelNode* root)
{
    // Traverse the tree to the leftmost node, which has the minimum weight
    while (root->right != NULL)
    {
        root = root->right;
    }
    // Return the leftmost node, which contains the minimum weight
    return root;
}
//
// FUNCTION:findMinValuation
//DESCRIPTION:
// Function  is created to find Minimum value of Valuation node in BST
//PARAMETERS: ParcelNode* root
// RETURNS: ParcelNode* (the node with the minimum valuation)
//
ParcelNode* findMinValuation(ParcelNode* root)
{
    // If the tree is empty, return NULL
    if (root == NULL)
    {
        return NULL;
    }
    // Assume the current node is the minimum
    ParcelNode* minNode = root;

    // Recursively find the minimum valuation in the left subtree
    ParcelNode* leftMin = findMinValuation(root->left);
    // Update minNode if a smaller valuation is found in the left subtree
    if (leftMin != NULL && leftMin->valuation < minNode->valuation) {
        minNode = leftMin;
    }
    // Recursively find the minimum valuation in the right subtree
    ParcelNode* rightMin = findMinValuation(root->right);
    // Update minNode if a smaller valuation is found in the right subtree
    if (rightMin != NULL && rightMin->valuation < minNode->valuation) {
        minNode = rightMin;
    }

    return minNode;
}
//
// FUNCTION:findMaxValuation
//DESCRIPTION:
// Function  is created to find Minimum value of Valuation node in BST
//PARAMETERS: ParcelNode* root
// RETURNS: ParcelNode* (the node with the maximum valuation)
//
ParcelNode* findMaxValuation(ParcelNode* root) {
    // If the tree is empty, return NULL
    if (root == NULL) {
        return NULL;
    }

    ParcelNode* maxNode = root;

    // Recursively find the maximum valuation in the left subtree
    ParcelNode* leftMax = findMaxValuation(root->left);
    // Update maxNode if a larger valuation is found in the left subtree
    if (leftMax != NULL && leftMax->valuation > maxNode->valuation) {
        maxNode = leftMax;
    }

    // Recursively find the maximum valuation in the right subtree
    ParcelNode* rightMax = findMaxValuation(root->right);
    // Update maxNode if a larger valuation is found in the right subtree
    if (rightMax != NULL && rightMax->valuation > maxNode->valuation) {
        maxNode = rightMax;
    }

    return maxNode;
}
//
// FUNCTION:displayParcels
//DESCRIPTION:
// Function  is created to display all the parcles of particular company
//PARAMETERS: ParcelNode* root
//
void displayParcels(ParcelNode* root)
{
    // If the current node is not NULL, perform an in-order traversal

    if (root != NULL)
    {
        // Recursively display parcels in the left subtree
        displayParcels(root->left);
        // Print the current node's parcel information
        printf("Destination: %s, Weight: %d g, Valuation: $%.2f\n", root->destination, root->weight, root->valuation);
        // Recursively display parcels in the right subtree
        displayParcels(root->right);
    }
}
//
// FUNCTION:displayParcelsByWeight
//DESCRIPTION:
// Function is created To display the Parcel by it Weight 
//PARAMETERS: ParcelNode* root, int weight, int higher
void displayParcelsByWeight(ParcelNode* root, int weight, int higher)
{
    if (root == NULL)
    {
        return;
    }
    displayParcelsByWeight(higher ? root->right : root->left, weight, higher);
    if ((higher && root->weight > weight) || (!higher && root->weight < weight))
    {
        printf("Destination: %s, Weight: %d g, Valuation: $%.2f\n", root->destination, root->weight, root->valuation);
    }
    displayParcelsByWeight(higher ? root->left : root->right, weight, higher);
}
//
// FUNCTION:calculateTotals
//DESCRIPTION:
// Function is created to calculate the total amount of the parcels of the Particular Company
//PARAMETERS: ParcelNode* root, int* totalWeight, float* totalValuation
//
void calculateTotals(ParcelNode* root, int* totalWeight, float* totalValuation)
{
    // If the current node is not NULL, calculate totals
    if (root != NULL)
    {
        // Add the current node's weight to the total weight
        *totalWeight += root->weight;
        // Add the current node's valuation to the total valuation
        *totalValuation += root->valuation;
        // Recursively calculate totals for the left subtree
        calculateTotals(root->left, totalWeight, totalValuation);
        // Recursively calculate totals for the right subtree
        calculateTotals(root->right, totalWeight, totalValuation);
    }
}

//
// FUNCTION:displayMenu
//DESCRIPTION:
// Function  is created to to display the menu for the the user to Select the Choice ans for various opeartion
//PARAMETERS: HashTable* hashTable
//
void displayMenu(HashTable* hashTable)
{
    int choice;
    int weight;
    int higher;
    char country[20];
    ParcelNode* node;
    do
    {
        printf("\nMenu:\n");
        printf("1. Enter country name and display all parcels\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display total parcel load and valuation for the country\n");
        printf("4. Display cheapest and most expensive parcel details\n");
        printf("5. Display lightest and heaviest parcel for the country\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        // Read the user's choice
        scanf_s("%d", &choice);
        // Process the user's choice using a switch statement
        switch (choice)
        {
        case 1:
            // Option 1: Display all parcels for a given country
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            // Get the ParcelNode list for the country using a hash function
            node = hashTable->buckets[hash_djb2(country)];
            // Check if there are parcels for the entered country
            if (node != NULL && strcmp(node->destination, country) == 0)
            {
                displayParcels(node);
            }
            else
            {
                printf("No parcels found for %s.\n", country);
            }
            break;
        case 2:
            // Option 2: Enter country and weight pair, then filter parcels by weight
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            printf("Enter weight: ");
            scanf_s("%d", &weight);
            if (weight < 100 || weight > 50000)
            {
                printf("Error: Invalid weight value.\n");
                break;
            }
            // Display options to filter parcels based on weight
            printf("1. Display parcels heavier than %d g\n", weight);
            printf("2. Display parcels lighter than %d g\n", weight);
            printf("Enter your choice: ");
            scanf_s("%d", &higher);
            // Get the ParcelNode list for the country using a hash function
            node = hashTable->buckets[hash_djb2(country)];
            // Check if there are parcels for the entered country
            if (node != NULL && strcmp(node->destination, country) == 0)
            {
                displayParcelsByWeight(node, weight, higher == 1);
            }
            else
            {
                // Display parcels based on the weight condition
                printf("No parcels found for %s.\n", country);
            }
            break;
        case 3:
            // Option 3: Display total weight and valuation for a given country
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            // Get the ParcelNode list for the country using a hash function
            node = hashTable->buckets[hash_djb2(country)];
            // Check if there are parcels for the entered country
            if (node != NULL && strcmp(node->destination, country) == 0)
            {
                int totalWeight = 0;
                float totalValuation = 0.0;
                // Calculate totals for the parcels
                calculateTotals(node, &totalWeight, &totalValuation);
                printf("Total weight: %d g, Total valuation: $%.2f\n", totalWeight, totalValuation);
            }
            else
            {
                printf("No parcels found for %s.\n", country);
            }
            break;
        case 4:
            // Option 4: Display cheapest and most expensive parcel details
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            // Get the ParcelNode list for the country using a hash function
            node = hashTable->buckets[hash_djb2(country)];
            // Check if there are parcels for the entered country
            if (node != NULL && strcmp(node->destination, country) == 0)
            {
                // Find the cheapest and most expensive parcels
                ParcelNode* minNode = findMinValuation(node);
                ParcelNode* maxNode = findMaxValuation(node);
                // Display the details of the cheapest parcel
                printf("Cheapest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", minNode->destination, minNode->weight, minNode->valuation);
                // Display the details of the most expensive parcel
                printf("Most Expensive Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", maxNode->destination, maxNode->weight, maxNode->valuation);
            }
            else
            {
                printf("No parcels found for %s.\n", country);
            }
            break;
        case 5:
            // Option 5: Display lightest and heaviest parcel details
            printf("Enter country name: ");
            scanf_s("%s", country, 20);
            // Get the ParcelNode list for the country using a hash function
            node = hashTable->buckets[hash_djb2(country)];
            // Check if there are parcels for the entered country
            if (node != NULL && strcmp(node->destination, country) == 0)
            {
                // Find the lightest and heaviest parcels
                ParcelNode* minNode = findMin(node);
                ParcelNode* maxNode = findMax(node);
                // Display the details of the lightest parcel
                printf("Lightest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", minNode->destination, minNode->weight, minNode->valuation);
                // Display the details of the heaviest parcel
                printf("Heaviest Parcel: Destination: %s, Weight: %d g, Valuation: $%.2f\n", maxNode->destination, maxNode->weight, maxNode->valuation);
                printf("No parcels found for %s.\n", country);
            }
            else
            {
                printf("No parcels found for %s.\n", country);
            }
            break;
        case 6:
            // Option 6: Exit the menu
            printf("Exiting...\n");
            break;
        default:
            // Handle invalid menu choices
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);  // Continue the loop until the user chooses to exit
}