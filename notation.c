#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defines a binary code with left and right child
typedef struct Node{
    char data;
    struct Node *left, *right;
} Node;

//points to next node; used in Postfix_to_tree function
typedef struct Stack {
    Node *treeNode;
    struct Stack *next;
} Stack;

Node* newNode(char op); //creates new binary node
void push(Stack** top, Node* node); //pushes a tree node in the stack
Node* pop(Stack** top); //pops a tree node in the stack
int isOperand(char ch); //checks if the character is an operand
int isOperator(char ch); //checks if the character is an operator
int isASpace(char ch); //checks if the character is a space
int isPostfix(const char* postfix); //function to determine whether the expression is valid postfix   gcc notation-converter.c -o notation-converter.exe
Node* postfix_to_tree(char* postfix); //function to put the postfix expression in a binary tree
void skipSpaces(char* expr, int* index); //function to skip spaces
int isPrefix(const char* prefix); //function to determine whether the expression is valid prefix
Node* prefix_to_tree(char* prefix, int* index);//function to put the prefix expression in a binary tree
void preorder_Traversal(Node* root); //function to traverse in preorder
void inorder_Traversal(Node* root); //function to traverse in inorder
void postorder_Traversal(Node* root); //function to traverse in postorder
int precedence(char op); // Determine operator precedence
int isInfix(const char* infix); //function to determine whether it is a valid infix
void infix_to_postfix(const char* infix, char* postfix); // Convert infix to postfix expression
void reverse(char* str); // Reverse a string (used for infix to prefix conversion logic)
void infix_to_prefix(const char* infix, char* prefix); // Convert infix to prefix using operator and operand stacks
void printHelp(); //Prints help information
void printGuide(); //Prints detailed guide with explanations and examples



int main(int argc, char *argv[]) {
    if(argc == 1){
        printHelp();
    } else if(argc <= 3){ //if argument is one
        // Validate argument for help or guide
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            // Handle help flag
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "--guide") == 0) {
            // Handle guide flag
            printGuide();
            return 0;
        } else if(strcmp(argv[1], "-h") != 0 || strcmp(argv[1], "--help") != 0 || strcmp(argv[1], "--guide") != 0 || argc <= 3){
            printf("Error: Missing or invalid argument.\n");
            return 1;
        } 
    } //if arguments are more than one
    else if(argc > 6) {
        printf("Error: Multiple expressions provided. Mathematical expressions containing spaces must be enclosed in quotation marks.\n");
    }
    else {
        // Error Handling
        // Validate '--from' and '--to' Arguments
        if (strcmp(argv[1], "--from") != 0) {
            printf("Error: Missing '--from' argument.\n");
            return 1;
        } else if(strcmp(argv[3], "--to") != 0){
            printf("Error: Missing '--to' argument.\n");
            return 1;
        }// Validate Format Specifiers
        else if(strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0 && 
                strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0){
            printf("Error: Invalid format specifier '%s' or '%s'.\n", argv[2], argv[4]);
            return 1;
        }
        else if (strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s.\n", argv[2]);
            return 1;
        }
        else if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'.\n", argv[4]);
            return 1;
        }
        else if(argc == 5)
            printf("Error: Missing expression.");
        else if (strcmp(argv[1], "--from") == 0) {
            if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)) { //Infix to postfix
                char postfix[100];
                infix_to_postfix(argv[5], postfix);
                printf(postfix);
            } else if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)) { //Infix to prefix
                char prefix[100];
                infix_to_prefix(argv[5], prefix);
                printf(prefix);
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)) { //prefix to infix
                int index = 0;
                Node* root = prefix_to_tree(argv[5], &index);
                inorder_Traversal(root);
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)) { //postfix to infix
                Node* root = postfix_to_tree(argv[5]);
                inorder_Traversal(root);
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)) { //postfix to prefix
                Node* root = postfix_to_tree(argv[5]);
                preorder_Traversal(root);
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)) { //prefix to postfix
                int index = 0;
                Node* root = prefix_to_tree(argv[5], &index);
                postorder_Traversal(root);
            }
        }
    }

return 0;

}

//creates new binary node
Node* newNode(char op) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = op;
    node->left = node->right = NULL;
    return node;
}

//pushes a tree node in the stack
void push(Stack** top, Node* node) {
    Stack* newStackNode = (Stack*)malloc(sizeof(Stack));
    newStackNode->treeNode = node;
    newStackNode->next = *top;
    *top = newStackNode;
}

//pops a tree node in the stack
Node* pop(Stack** top) {
    if (*top == NULL) return NULL; //underflow check
    
    Stack* temp = *top; //store current top to temp
    *top = (*top)->next; //move top to next
    Node* node = temp->treeNode; //get the new tree node without the previous top
    free(temp); //free node
    return node;
}

//checks if the character is an operand
int isOperand(char ch) {
    return ((ch >= '0' && ch <= '9') || 
            (ch >= 'A' && ch <= 'Z') || 
            (ch >= 'a' && ch <= 'z'));
}

//checks if the character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

//checks if the character is a space
int isASpace(char ch) {
    return (ch == ' ');
}


//function to determine whether the expression is valid postfix
int isPostfix(const char* postfix) {
    int operandCount = 0; 
    int operatorCount = 0;

    for(int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];
        if (isASpace(token)) continue; //proceed to next character if space

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

    if(operatorCount == operandCount) //if equal
        return 1; //the expression is valid
    else if(operatorCount > operandCount)
                return 2; //if operator is greater, insufficient operand
    else if(operatorCount < operandCount)
                return 3; //if operator is greater, insufficient operator
}

//function to put the postfix expression in a binary tree
Node* postfix_to_tree(char* postfix){
    //checks whether the given expression is infix or prefix; if they are skip the process
    if(isInfix(postfix)){
        printf("Error: Malformed Expression (This is an infix expression.).");
        return NULL;
    }
    else if(isPrefix(postfix)){
        printf("Error: Malformed Expression (This is an prefix expression.).");
        return NULL;
    }

    int validpostfix = isPostfix(postfix);

    if(validpostfix == 1){
        Stack* stack = NULL; //initializes empty stack

        //for loop that loop each character in the string until null terminator is found
        for(int i=0; postfix[i] != '\0'; i++){
            char token = postfix[i]; //each character is stored in token variable
    
            if(isASpace(token)){ //if space, proceed to next character
                continue;
            }
    
            if(isOperand(token)){ // if char is operand, push to stack
                push(&stack, newNode(token));
            }
            if(isOperator(token)){ //if char is operator, pop two nodes
                Node* right = pop(&stack);
                Node* left = pop(&stack);
                Node* opNode = newNode(token); //create two children
                opNode->left = left;
                opNode->right = right;
                push(&stack, opNode); //push the newnode with two children back into the stack
            }
        }
        return pop(&stack);
    }
    else if(validpostfix == 2){
        printf("Error: Malformed expression  (e.g., insufficient operand).\n");
    }
    else if(validpostfix == 3){
        printf("Error: Malformed expression  (e.g., insufficient operator).\n");
    }
}

//function to skip spaces
void skipSpaces(char* expr, int* index) {
    while (expr[*index] == ' ') (*index)++;
}

//function to determine whether the expression is valid prefix
int isPrefix(const char* prefix) {
    int operandCount = 0;
    int operatorCount = 0;

    // Find length
    int len = 0;
    while (prefix[len] != '\0') len++;

    // Scan right to left
    for (int i = len - 1; i >= 0; i--) {
        char token = prefix[i];
        if (isASpace(token)) continue; //if space, proceed to next character

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

    if(operatorCount == operandCount) //if equal
        return 1; //the expression is valid
    else if(operatorCount > operandCount)
        return 2; //if operator is greater, insufficient operand
    else if(operatorCount < operandCount)
        return 3; //if operator is greater, insufficient operator
}

//function to put the prefix expression in a binary tree
Node* prefix_to_tree(char* prefix, int* index) {
    //checks whether the given expression is infix or postfix; if they are skip the process
    if(isInfix(prefix) == 1){
        printf("Error: Malformed Expression (This is an infix expression.).");
        return NULL;
    }
    else if(isPostfix(prefix) == 1){
        printf("Error: Malformed Expression (This is an postfix expression.).");
        return NULL;
    }

    int validprefix = isPrefix(prefix);

    if(validprefix == 1){
        skipSpaces(prefix, index); //skip spaces
        char token = prefix[*index]; //read the current character
    
        if (token == '\0') return NULL; //if token is null terminator return NULL
    
        Node* node = newNode(token); //create a newnode with token
        (*index)++;  // increment index
        skipSpaces(prefix, index);
    
        if (isOperator(token)) {//if token is an operator
            node->left = prefix_to_tree(prefix, index); //recursively build left subtree
            node->right = prefix_to_tree(prefix, index); //recursively build right subtree
    
            if (!node->left || !node->right) { //checks if left or right node is NULL
                printf("Error: Malformed expression.\n");
                return NULL;
            }
        }
    
        return node; //return node
    }
    else if(validprefix == 2){
        printf("Error: Malformed expression  (e.g, insufficient operand).\n");
    }
    else if(validprefix == 3){
        printf("Error: Malformed expression  (e.g., insufficient operator).\n");
    }
}

//function to traverse in preorder
void preorder_Traversal(Node* root) {
    if (root != NULL) { 
        printf("%c ", root->data);   // Visit root
        preorder_Traversal(root->left);        // Traverse left
        preorder_Traversal(root->right);       // Traverse right
    }
}

//function to traverse in inorder
void inorder_Traversal(Node* root) {
    if (root == NULL)
        return;

    // If it's an operator, add parentheses
    if (isOperator(root->data)) {
        printf("(");
        inorder_Traversal(root->left); //visit left
        printf(" %c ", root->data);
        inorder_Traversal(root->right); //visit right
        printf(")");
    } else {
        // Operand (leaf node), just print it
        printf("%c", root->data);
    }
}

//function to traverse in postorder
void postorder_Traversal(Node* root) {
    if (root == NULL) return;
    postorder_Traversal(root->left); //visit left
    postorder_Traversal(root->right); //visit right
    printf("%c ", root->data);
}

// Determine operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

//function to determine whether it is a valid infix
int isInfix(const char* infix) {
    int balance = 0;               // For tracking parentheses
    int expectOperand = 1;         // Start expecting an operand 
    int needOperator = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char token = infix[i];
        if (isASpace(token)) continue; //if is space, continue to next character

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

    if(balance == 0 && expectOperand == 0 && needOperator == 0)
        return 1; //if balanced and ends with operand
    else if(expectOperand == 1)
        return 2; //insufficient operand
    else if(needOperator == 1)
        return 3; //insufficient operator
}

//Infix to postfix 
//Converts infix to postfix expresssion using shunting yard algorithm
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
        if (validinfix == 2) {
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

        // If the token is an operand, add it directly to the output
        if (isOperand(token)) {
            postfix[j++] = token;
            postfix[j++] = ' ';
        
        // If token is '(', push to operator stack
        } else if (token == '(') {
            push(&opStack, newNode(token));
        
        // If token is ')', pop from stack to output until '(' is found
        } else if (token == ')') {
            while (opStack && opStack->treeNode->data != '(') {
                postfix[j++] = pop(&opStack)->data;
                postfix[j++] = ' ';
            }
            if (opStack && opStack->treeNode->data == '(') {
                pop(&opStack); // Remove '(' from the stack
            }
        // If the token is an operator
        } else if (isOperator(token)) {
            // Pop operators from the stack that have higher or equal precedence
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) >= precedence(token)) {
                postfix[j++] = pop(&opStack)->data;
                postfix[j++] = ' ';
            }
            // Push the current operator to the stack
            push(&opStack, newNode(token));
        }
    }

    // Pop any remaining operators from the stack to the output
    while (opStack) {
        postfix[j++] = pop(&opStack)->data;
        postfix[j++] = ' ';
    }

    if (j > 0 && postfix[j - 1] == ' ') j--; // Remove trailing space
    postfix[j] = '\0';
}

// Reverse a string (used for infix to prefix conversion logic)
void reverse(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Convert infix to prefix using operator and operand stacks
void infix_to_prefix(const char* infix, char* prefix) {
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
        if (validinfix == 2) {
            printf("Error: Malformed expression (e.g., insufficient operand).\n");
        } else if (validinfix == 3) {
            printf("Error: Malformed expression (e.g., insufficient operator).\n");
        }
        return;
    }

    // Reverse the infix expression and swap '(' with ')'
    int len = strlen(infix);
    char reversed[len + 1];
    int j = 0;

    for (int i = len - 1; i >= 0; i--) {
        if (infix[i] == '(') {
            reversed[j++] = ')';
        } else if (infix[i] == ')') {
            reversed[j++] = '(';
        } else {
            reversed[j++] = infix[i];
        }
    }
    reversed[j] = '\0';

    // Convert reversed infix to postfix 
    char tempPostfix[100];
    infix_to_postfix(reversed, tempPostfix);

    // Reverse the postfix to get prefix
    reverse(tempPostfix);
    strcpy(prefix, tempPostfix);
}

/*
 * Prints help information
 */
void printHelp() {
    printf("Expression Notation Converter\n");
    printf("Description: Converts mathematical expressions between infix, prefix, and postfix notations.\n\n");
    printf("Usage: notation-converter --from <input_format> --to <output_format> \"<expression>\"\n\n");
    printf("Options:\n");
    printf("  --from <input_format>     Input format: infix, prefix, or postfix\n");
    printf("  --to <output_format>      Ouput format: infix, prefix, or postfix\n");
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

/*
 * Prints detailed guide with explanations and examples
 */
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
    printf("  --from <format>        Specify input format (infix, prefix, or postfix)\n");
    printf("  --to <format>          Specify output format (infix, prefix, or postfix)\n");
    printf("  \"<expression>\"         Space-separated expression string in double quotes\n");
    printf("  -h, --help             Show brief usage help message\n");
    printf("  --guide                Show this detailed program guide\n\n");

    printf("Expression Notations:\n");
    printf("  INFIX\n");
    printf("    - Operators between operands, e.g., ( 1 + 2 ) * 3\n");
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
    printf("  - Operands must be single-digit numbers (0-9).\n");
    printf("  - Operators supported are addition (+), subtraction (-),\n");
    printf("    multiplication (*), and division (/).\n");
    printf("  - Tokens (operands/operators) must be space-separated.\n");
    printf("  - Parentheses are allowed only in infix notation.\n\n");

    printf("Examples:\n");
    printf("  Convert prefix to infix:\n");
    printf("    $ notation-converter --from prefix --to infix \"* + 1 2 3\"\n");
    printf("    Output: ( ( 1 + 2 ) * 3 )\n\n");

    printf("  Convert infix to postfix:\n");
    printf("    $ notation-converter --from infix --to postfix \"( 1 + 2 ) * 3\"\n");
    printf("    Output: 1 2 + 3 *\n\n");

    printf("  Convert postfix to prefix:\n");
    printf("    $ notation-converter --from postfix --to prefix \"1 2 3 * +\"\n");
    printf("    Output: + 1 * 2 3\n\n");

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
    printf("  - Only single-digit integers (0-9) are supported as operands.\n");
    printf("  - Output expressions in infix form include full parentheses to preserve order.\n");
    printf("  - Input expressions must be space-separated and enclosed in double quotes.\n\n");

    printf("============================================================\n");
}
