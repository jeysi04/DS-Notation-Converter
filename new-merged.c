/* Program: Notation Converter CLI
 * Description: Converts mathematical expressions between infix, prefix, and postfix notations.
 *
 * Bloc: BSCS 2A
 * Group Members:
 *      Cadag, Jaycee D.
 *      Espinas, A Z Rain L.
 *      Lovina, John Melrick M.
*/

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines a binary node with left and right child
typedef struct Node {
    char data;
    struct Node *left, *right;
} Node;

// Points to next node; used in postfix_to_tree function
typedef struct Stack {
    Node *treeNode;
    struct Stack *next;
} Stack;

// Function prototypes
Node* newNode(char op); // Creates new binary node
void push(Stack** top, Node* node); // Pushes a tree node onto the stack
Node* pop(Stack** top); // Pops a tree node from the stack
int isOperand(char ch); // Checks if the character is an operand
int isOperator(char ch); // Checks if the character is an operator
int isASpace(char ch); // Checks if the character is a space
void skipSpaces(char* expr, int* index); // Function to skip spaces
int precedence(char op); // Determines operator precedence
void inorder_Traversal(Node* root); // Traverses expression in inorder
void preorder_Traversal(Node* root); // Traverses expression in preorder
void postorder_Traversal(Node* root); // Traverses expression in postorder
int isInfix(const char* infix); // Determines whether it is a valid infix
void infix_to_postfix(const char* infix, char* postfix); // Converts infix to postfix expression
void infix_to_prefix(const char* infix, char* prefix); // Converts infix to prefix
void reverse(char* str); // Reverses a string (used for infix to prefix conversion)
int isPrefix(const char* prefix); // Determines whether the expression is valid prefix
Node* prefix_to_tree(char* prefix, int* index); // Puts the prefix expression in a binary tree
int isPostfix(const char* postfix); // Determines whether the expression is valid postfix
Node* postfix_to_tree(char* postfix); // Puts the postfix expression in a binary tree
void printHelp(); // Prints help information
void printGuide(); // Prints detailed guide with explanations and examples

// Main function to handle command-line arguments and perform notation conversions
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printHelp();
    } else if (argc <= 3) { // If argument is one
        // Validate argument for help or guide
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            // Display help information
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "--guide") == 0) {
            // Display detailed guide
            printGuide();
            return 0;
        } else {
            printf("Error: Missing or invalid argument.\n");
            printf("Usage: %s --from <input_format> --to <output_format> \"<expression>\"\n", argv[0]);
            printf("Try '%s --help' for more information.\n", argv[0]);
            return 1;
        }
    } // If arguments are more than one
    else if (argc > 6) {
        printf("Error: Multiple expressions detected.\n");
        printf("Hint: Use double quotes for expressions with spaces.\n");
        return 1;
    } else {
        // Error Handling
        // Validate '--from' and '--to' Arguments
        if (strcmp(argv[1], "--from") != 0) {
            printf("Error: Missing '--from' argument.\n");
            return 1;
        } else if (strcmp(argv[3], "--to") != 0) {
            printf("Error: Missing '--to' argument.\n");
            return 1;
        } // Validate Format Specifiers
        else if (strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'.\n", argv[2]);
            printf("Hint: Use 'infix', 'prefix', or 'postfix'.\n");
            return 1;
        } else if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'.\n", argv[4]);
            printf("Hint: Use 'infix', 'prefix', or 'postfix'.\n");
            return 1;
        } else if (argc == 5) {
            printf("Error: Missing expression.\n");
            return 1;
        } else if (strcmp(argv[1], "--from") == 0) {
            // Check for same input and output formats
            if (strcmp(argv[2], argv[4]) == 0) {
                if (strcmp(argv[2], "infix") == 0) {
                    if (isInfix(argv[5]) == 0 || isInfix(argv[5]) == 2 || isInfix(argv[5]) == 3) {
                        printf("Error: Expression is not a valid infix expression.\n");
                        return 1;
                    }
                    printf("Note: The expression is already in infix form.\n");
                    return 0;
                }
                if (strcmp(argv[2], "prefix") == 0) {
                    if (isPrefix(argv[5]) == 0 || isPrefix(argv[5]) == 2 || isPrefix(argv[5]) == 3) {
                        printf("Error: Expression is not a valid prefix expression.\n");
                        return 1;
                    }
                    printf("Note: The expression is already in prefix form.\n");
                    return 0;
                }
                if (strcmp(argv[2], "postfix") == 0) {
                    if (isPostfix(argv[5]) == 0 || isPostfix(argv[5]) == 2 || isPostfix(argv[5]) == 3) {
                        printf("Error: Expression is not a valid postfix expression.\n");
                        return 1;
                    }
                    printf("Note: The expression is already in postfix form.\n");
                    return 0;
                }
            }

            // Perform conversions based on input and output formats
            if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)) { // Infix to postfix
                char postfix[100] = {0};
                infix_to_postfix(argv[5], postfix);
                printf("%s\n", postfix);
            } else if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)) { // Infix to prefix
                char prefix[100] = {0};
                infix_to_prefix(argv[5], prefix);
                printf("%s\n", prefix);
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)) { // Prefix to infix
                int index = 0;
                Node* root = prefix_to_tree(argv[5], &index);
                inorder_Traversal(root);
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)) { // Postfix to infix
                Node* root = postfix_to_tree(argv[5]);
                inorder_Traversal(root);
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)) { // Postfix to prefix
                Node* root = postfix_to_tree(argv[5]);
                preorder_Traversal(root);
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)) { // Prefix to postfix
                int index = 0;
                Node* root = prefix_to_tree(argv[5], &index);
                postorder_Traversal(root);
            }
        }
    }
    return 0;
}

// Creates new binary node with the given operator or operand
Node* newNode(char op) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = op;
    node->left = node->right = NULL;
    return node;
}

// Pushes a tree node onto the stack
void push(Stack** top, Node* node) {
    Stack* newStackNode = (Stack*)malloc(sizeof(Stack));
    newStackNode->treeNode = node;
    newStackNode->next = *top;
    *top = newStackNode;
}

// Pops a tree node from the stack
Node* pop(Stack** top) {
    if (*top == NULL) return NULL; // Underflow check
    Stack* temp = *top; // Store current top
    *top = (*top)->next; // Move top to next
    Node* node = temp->treeNode; // Get the tree node
    free(temp); // Free stack node
    return node; // Return node (caller must free it)
}

// Checks if the character is an operand (digit or letter)
int isOperand(char ch) {
    return ((ch >= '0' && ch <= '9') || 
            (ch >= 'A' && ch <= 'Z') || 
            (ch >= 'a' && ch <= 'z'));
}

// Checks if the character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// Checks if the character is a space
int isASpace(char ch) {
    return (ch == ' ');
}

// Function to skip spaces in the expression
void skipSpaces(char* expr, int* index) {
    while (expr[*index] == ' ') (*index)++;
}

// Determines operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to traverse in preorder (root, left, right)
void preorder_Traversal(Node* root) {
    if (root != NULL) { 
        printf("%c ", root->data); // Visit root
        preorder_Traversal(root->left); // Traverse left
        preorder_Traversal(root->right); // Traverse right
    }
}

// Function to traverse in inorder (left, root, right)
void inorder_Traversal(Node* root) {
    if (root == NULL)
        return;
    // If it's an operator, add parentheses
    if (isOperator(root->data)) {
        printf("(");
        inorder_Traversal(root->left); // Visit left
        printf(" %c ", root->data); // Visit root
        inorder_Traversal(root->right); // Visit right
        printf(")");
    } else {
        // Operand (leaf node), just print it
        printf("%c", root->data);
    }
}

// Function to traverse in postorder (left, right, root)
void postorder_Traversal(Node* root) {
    if (root == NULL) return;
    postorder_Traversal(root->left); // Visit left
    postorder_Traversal(root->right); // Visit right
    printf("%c ", root->data); // Visit root
}

// Function to determine whether the expression is in valid infix format
int isInfix(const char* infix) {
    int balance = 0; // For tracking parentheses
    int expectOperand = 1; // Start expecting an operand 
    int needOperator = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        char token = infix[i];
        if (isASpace(token)) continue; // If space, continue to next character
        if (token == '(') {
            balance++; 
        } else if (token == ')') {
            balance--;
            if (balance < 0) return 0; // Too many closing parentheses (without a matching opening parentheses)
        } else if (isOperand(token)) {
            if (!expectOperand) needOperator = 1; // Two operands in a row (expectOperand = 0)
            expectOperand = 0; // Next, expect an operator
        } else if (isOperator(token)) {
            if (expectOperand) return 2; // Operator without operand (expectOperand = 1)
            expectOperand = 1; // Next, expect an operand
        } else {
            return 0; // Invalid character
        }
    }
    if (balance == 0 && expectOperand == 0 && needOperator == 0)
        return 1; // Balanced and ends with operand
    else if (expectOperand == 1)
        return 2; // Insufficient operand
    else if (needOperator == 1)
        return 3; // Insufficient operator
    return 0;
}

// Function to convert from infix to postfix using the Shunting Yard Algorithm
void infix_to_postfix(const char* infix, char* postfix) {
    // Check for malformed input before processing
    if (isPostfix(infix) == 1) {
        printf("Error: Malformed Expression (This is a postfix expression).\n");
        return;
    } else if (isPrefix(infix) == 1) {
        printf("Error: Malformed Expression (This is a prefix expression).\n");
        return;
    }
    // Validate infix expression
    int validinfix = isInfix(infix);
    if (validinfix != 1) {
        if (validinfix == 0) {
            printf("Error: Invalid character or unbalanced parentheses.\n");
        } else if (validinfix == 2) {
            printf("Error: Malformed expression (e.g., insufficient operand).\n");
        } else if (validinfix == 3) {
            printf("Error: Malformed expression (e.g., insufficient operator).\n");
        }
        return;
    }
    Stack* opStack = NULL; // Stack to hold operators
    int j = 0; // Index for postfix output
    // Traverse the infix expression character by character
    for (int i = 0; infix[i]; i++) {
        char token = infix[i];
        // Skip spaces
        if (isASpace(token)) continue;
        // If the token is an opening parenthesis, push to operator stack
        if (token == '(') {
            push(&opStack, newNode(token));
        // If the token is an operand, add it to the output
        } else if (isOperand(token)) {
            if (j > 0) postfix[j++] = ' '; // Add space for token separation
            postfix[j++] = token;
        // If token is ')', pop from stack to output until '(' is found
        } else if (token == ')') {
            if (!opStack) {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
            while (opStack && opStack->treeNode->data != '(') {
                if (j > 0) postfix[j++] = ' '; // Add space
                Node* node = pop(&opStack);
                postfix[j++] = node->data; // Append popped operator
                free(node);
            }
            if (opStack && opStack->treeNode->data == '(') {
                Node* node = pop(&opStack); // Remove '(' from the stack
                free(node);
            } else {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
        // If the token is an operator
        } else if (isOperator(token)) {
            if (j > 0) postfix[j++] = ' '; // Add space
            // Pop operators with higher or equal precedence
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) >= precedence(token)) {
                if (j > 0) postfix[j++] = ' '; // Add space
                Node* node = pop(&opStack);
                postfix[j++] = node->data; // Append higher/equal precedence operator
                free(node);
            }
            // Push the current operator to the stack
            push(&opStack, newNode(token));
        }
    }
    // Pop any remaining operators from the stack to the output
    while (opStack) {
        Node* node = pop(&opStack);
        if (j > 0) postfix[j++] = ' '; // Add space
        postfix[j++] = node->data; // Append remaining operator
        free(node);
    }
    postfix[j] = '\0'; // Null-terminate the postfix string
}

// Function to convert from infix to prefix using the Shunting Yard Algorithm
void infix_to_prefix(const char* infix, char* prefix) {
    // Check input format before processing
    if (isPostfix(infix) == 1) {
        printf("Error: Malformed expression. Detected postfix format.\n");
        printf("Hint: Expression must be in infix form.\n");
        return;
    } else if (isPrefix(infix) == 1) {
        printf("Error: Malformed expression. Detected prefix format.\n");
        printf("Hint: Expression must be in infix form.\n");
        return;
    }

    // Validate infix expression
    int validInfix = isInfix(infix);
    if (validInfix != 1) {
        switch (validInfix) {
        case 0:
            printf("Error: Invalid character or unbalanced parentheses.\n");
            break;
        case 2:
            printf("Error: Malformed expression. Missing operand.\n");
            break;
        case 3:
            printf("Error: Malformed expression. Missing operator.\n");
            break;
        default:
            printf("Error: Invalid infix expression.\n");
        }
        return;
    }

    // Reverse the infix expression and swap '(' with ')'
    int len = strlen(infix);
    char reversed[100] = {0}; // Initialize to avoid garbage data
    strcpy(reversed, infix);
    reverse(reversed);

    // Convert reversed infix to postfix
    char postfix[100] = {0}; // Initialize to avoid garbage data
    Stack* opStack = NULL; // Stack to hold operators
    int j = 0; // Index for postfix output

    for (int i = 0; reversed[i]; i++) {
        char token = reversed[i];
        // Skip spaces
        if (isASpace(token)) continue;
        // If the token is an opening parenthesis, push to operator stack
        if (token == '(') {
            push(&opStack, newNode(token));
        // If the token is an operand, add it to the output
        } else if (isOperand(token)) {
            if (j > 0) postfix[j++] = ' '; // Add space for token separation
            postfix[j++] = token;
        // If token is ')', pop from stack to output until '(' is found
        } else if (token == ')') {
            if (!opStack) {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
            while (opStack && opStack->treeNode->data != '(') {
                if (j > 0) postfix[j++] = ' '; // Add space
                Node* node = pop(&opStack);
                postfix[j++] = node->data; // Append popped operator
                free(node);
            }
            if (opStack && opStack->treeNode->data == '(') {
                Node* node = pop(&opStack); // Remove '(' from the stack
                free(node);
            } else {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
        // If the token is an operator
        } else if (isOperator(token)) {
            if (j > 0) postfix[j++] = ' '; // Add space
            // Pop operators with strictly higher precedence
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) > precedence(token)) {
                if (j > 0) postfix[j++] = ' '; // Add space
                Node* node = pop(&opStack);
                postfix[j++] = node->data; // Append higher precedence operator
                free(node);
            }
            // Push the current operator to the stack
            push(&opStack, newNode(token));
        }
    }
    // Pop any remaining operators from the stack to the output
    while (opStack) {
        Node* node = pop(&opStack);
        if (j > 0) postfix[j++] = ' '; // Add space
        postfix[j++] = node->data; // Append remaining operator
        free(node);
    }
    postfix[j] = '\0'; // Null-terminate the postfix string
    prefix[0] = '\0'; // Initialize prefix to empty string
    strcpy(prefix, postfix);
    // Reverse the postfix to get prefix
    reverse(prefix);
}

// Function to reverse a string and swaps parentheses for infix-to-prefix conversion
void reverse(char* str) {
    // Get the length of the input string
    int len = strlen(str);
    int i = 0, j = len - 1;
    // Swap characters from start to end
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        // Swap parentheses
        if (str[i] == '(') str[i] = ')';
        else if (str[i] == ')') str[i] = '(';
        if (str[j] == '(') str[j] = ')';
        else if (str[j] == ')') str[j] = '(';
        i++;
        j--;
    }
    // Handle middle character if length is odd
    if (i == j) {
        if (str[i] == '(') str[i] = ')';
        else if (str[i] == ')') str[i] = '(';
    }
}

// Function to determine whether the expression is in valid prefix format
int isPrefix(const char* prefix) {
    int operandCount = 0;
    int operatorCount = 0;

    // Find length
    int len = 0;
    while (prefix[len] != '\0') len++;

    // Scan right to left
    for (int i = len - 1; i >= 0; i--) {
        char token = prefix[i];
        if (isASpace(token)) continue; // If space, proceed to next character

        if (isOperand(token)) {
            operandCount++; // Add operand count
        } else if (isOperator(token)) {
            if (operandCount < 2) return 2; // Not enough operands
            operatorCount++;
            operandCount--; // One operator reduces the count
        } else {
            return 0; // Invalid character
        }
    }

    if(operatorCount > operandCount || (operatorCount == 1 && operandCount == 1)) // If equal
        return 1; // The expression is valid
    else if(operatorCount + 1 > operandCount)
        return 2; // If operator is greater, insufficient operand
    else if(operatorCount < operandCount)
        return 3; // If operator is greater, insufficient operator
}

// Function to put the prefix expression in a binary tree
Node* prefix_to_tree(char* prefix, int* index) {
    // Check input format before processing
    if (isInfix(prefix) == 1) {
        printf("Error: Malformed expression. Detected infix format.\n");
        printf("Hint: Expression must be in prefix form.\n");
        return NULL;
    } else if (isPostfix(prefix) == 1) {
        printf("Error: Malformed expression. Detected postfix format.\n");
        printf("Hint: Expression must be in prefix form.\n");
        return NULL;
    }

    int validPrefix = isPrefix(prefix);

    // Validate prefix expression
    if (validPrefix == 1) {
        skipSpaces(prefix, index); // Skip spaces
        char token = prefix[*index]; // Read the current character

        if (token == '\0') return NULL; // If token is null terminator, return NULL

        Node* node = newNode(token); // Create a new node with token
        (*index)++; // Increment index
        skipSpaces(prefix, index);

        if (isOperator(token)) { // If token is an operator
            node->left = prefix_to_tree(prefix, index); // Recursively build left subtree
            node->right = prefix_to_tree(prefix, index); // Recursively build right subtree
            if (!node->left || !node->right) { // Check if left or right node is NULL
                printf("Error: Malformed expression. Incomplete subtree.\n");
                free(node);
                return NULL;
            }
        }
        return node; // Return node
    } else if (validPrefix == 2) {
        printf("Error: Malformed expression. Missing operand.\n");
    } else if (validPrefix == 3) {
        printf("Error: Malformed expression. Missing operator.\n");
    }
    return NULL;
}

// Function to determine whether the expression is in valid postfix format
int isPostfix(const char* postfix) {
    int operandCount = 0; 
    int operatorCount = 0;
    for (int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];

        if (isASpace(token)) continue; // Proceed to next character if space

        if (isOperand(token)) {
            operandCount++; // Each operand adds one
        } else if (isOperator(token)) {
            if (operandCount < 2) return 2; // Not enough operands
            operatorCount++;
            operandCount--; // An operator needs two operands, leave one operand
        } else {
            return 0; // Invalid character
        }
    }
    if (operatorCount > operandCount || (operatorCount == 1 && operandCount == 1))
        return 1; // The expression is valid
    else if (operatorCount + 1 > operandCount)
        return 2; // Insufficient operand
    else if (operatorCount < operandCount )
        return 3; // Insufficient operator
}

// Function to put the postfix expression in a binary tree
Node* postfix_to_tree(char* postfix){
    // Check input format before processing
    if (isInfix(postfix) == 1) {
        printf("Error: Malformed expression. Detected infix format.\n");
        printf("Hint: Expression must be in postfix form.\n");
        return NULL;
    } else if (isPrefix(postfix) == 1) {
        printf("Error: Malformed expression. Detected prefix format.\n");
        printf("Hint: Expression must be in postfix form.\n");
        return NULL;
    }

    int validPostfix = isPostfix(postfix);

    if(validPostfix == 1){
        Stack* stack = NULL; // Initialize an empty stack

        // Loop through each character in the string until null terminator is found
        for(int i = 0; postfix[i] != '\0'; i++){
            char token = postfix[i]; // Each character is stored in token variable
    
            if(isASpace(token)){ // If token is a space, proceed to next character
                continue;
            }
    
            if(isOperand(token)){ // If token is an operand, push to stack
                push(&stack, newNode(token));
            }
            if(isOperator(token)){ // If token is an operator, pop two nodes
                Node* right = pop(&stack);
                Node* left = pop(&stack);
                Node* opNode = newNode(token); // Create two children
                opNode->left = left;
                opNode->right = right;
                push(&stack, opNode); // Push the newnode with two children back into the stack
            }
        }
        return pop(&stack);
    }
    else if(validPostfix == 2){
        printf("Error: Malformed expression. Missing operand.\n");
    }
    else if(validPostfix == 3){
        printf("Error: Malformed expression. Missing operator.\n");
    }
}

// Function to print help information
void printHelp() {
    printf("Expression Notation Converter\n");
    printf("Description: Converts mathematical expressions between infix, prefix, and postfix notations.\n\n");
    printf("Usage: notation-converter --from <input_format> --to <output_format> \"<expression>\"\n\n");
    printf("Options:\n");
    printf("  --from <input_format>     Input format: infix, prefix, or postfix\n");
    printf("  --to <output_format>      Output format: infix, prefix, or postfix\n");
    printf("  \"<expression>\"            Space-separated expression string\n");
    printf("  -h, --help                Show this help message\n");
    printf("  --guide                   Show detailed usage guide\n\n");
    printf("Examples:\n");
    printf("  notation-converter --from prefix --to infix \"+ 1 * 2 3\"\n");
    printf("  notation-converter --from infix --to postfix \"( 1 + 2 ) * 3\"\n");
    printf("  notation-converter --from postfix --to prefix \"1 2 3 * +\"\n");
    printf("  notation-converter -h\n");
    printf("  notation-converter --help\n");
    printf("  notation-converter --guide\n");
}

// Function to print detailed guide with explanations and examples
void printGuide() {
    printf("Expression Notation Converter - Detailed Guide\n");
    printf("==============================================\n\n");
    printf("Description:\n");
    printf("  A command-line utility to convert mathematical expressions between\n");
    printf("  infix, prefix (Polish), and postfix (Reverse Polish) notations using\n");
    printf("  expression trees.\n\n");
    printf("Command Syntax:\n");
    printf("  notation-converter --from <input_format> --to <output_format> \"<expression>\"\n");
    printf("  notation-converter --h\n");
    printf("  notation-converter --help\n");
    printf("  notation-converter --guide\n\n");
    printf("Command-Line Options:\n");
    printf("  --from <input_format>        Specify input format (infix, prefix, or postfix)\n");
    printf("  --to <output_format>         Specify output format (infix, prefix, or postfix)\n");
    printf("  \"<expression>\"               Expression string enclosed in double quotes\n");
    printf("  -h, --help                   Show brief usage help message\n");
    printf("  --guide                      Show this detailed program guide\n\n");
    printf("Expression Notations:\n");
    printf("  INFIX\n");
    printf("    - Operators between operands, e.g., (1 + 2) * 3\n");
    printf("    - Parentheses required to ensure proper order\n\n");
    printf("  PREFIX (Polish)\n");
    printf("    - Operators precede operands, e.g., * + 1 2 3\n");
    printf("    - No parentheses needed\n\n");
    printf("  POSTFIX (Reverse Polish)\n");
    printf("    - Operators follow operands, e.g., 1 2 3 * +\n");
    printf("    - No parentheses needed\n\n");
    printf("Conversion Process:\n");
    printf("  1. The input expression is validated for correctness.\n");
    printf("  2. An expression tree is then built based on input notation.\n");
    printf("  3. The tree is traversed to produce output in the desired format:\n");
    printf("     - In-order Traversal    ->  Infix  Notation (with parentheses)\n");
    printf("     - Pre-order Traversal   ->  Prefix Notation\n");
    printf("     - Post-order Traversal  ->  Postfix Notation\n\n");
    printf("Input Requirements:\n");
    printf("  - Operands must be single-digit numbers (0-9) or letters of the\n");
    printf("    alphabet (A-Z and a-z).\n");
    printf("  - Operators supported are addition (+), subtraction (-),\n");
    printf("    multiplication (*), and division (/).\n");
    printf("  - Parentheses are allowed only in infix notation.\n\n");
    printf("Examples:\n");
    printf("  Convert prefix to infix:\n");
    printf("    $ notation-converter --from prefix --to infix \"* + A B C\"\n");
    printf("    Output: ( ( A + B ) * C )\n\n");
    printf("  Convert infix to postfix:\n");
    printf("    $ notation-converter --from infix --to postfix \"(A + B) * C\"\n");
    printf("    Output: A B + C *\n\n");
    printf("  Convert postfix to prefix:\n");
    printf("    $ notation-converter --from postfix --to prefix \"A B C * +\"\n");
    printf("    Output: + A * B C\n\n");
    printf("Error Handling:\n");
    printf("  The utility will detect and report errors such as:\n");
    printf("  - Missing or invalid arguments\n");
    printf("  - Invalid format specifiers\n");
    printf("  - Invalid expression formats (e.g., incorrect number of operands)\n");
    printf("  - Mismatched parentheses (infix only)\n");
    printf("  - Invalid characters or unsupported syntax\n");
    printf("  - Memory allocation failure\n\n");
    printf("Notes:\n");
    printf("  - This version supports only basic arithmetic operations (+, -, *, /).\n");
    printf("  - Only single-digit integers (0-9) and alphabetic characters (A-Z and a-z)\n");
    printf("    are supported as operands.\n");
    printf("  - Operands and operators may be separated by spaces.\n");
    printf("  - Multi-character operands or complex expressions are not supported.\n");
    printf("  - Input expressions must be enclosed in double quotes.\n");
    printf("  - Output expressions in infix form include full parentheses to preserve order.\n\n");
    printf("============================================================\n");
}
