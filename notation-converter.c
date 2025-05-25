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

Node* newNode(char op); // Creates new binary node
void push(Stack** top, Node* node); // Pushes a tree node onto the stack
Node* pop(Stack** top); // Pops a tree node from the stack
int isOperand(char ch); // Checks if the character is an operand
int isOperator(char ch); // Checks if the character is an operator
int isASpace(char ch); // Checks if the character is a space
int isPostfix(const char* postfix); // Determines whether the expression is valid postfix
Node* postfix_to_tree(char* postfix); // Puts the postfix expression in a binary tree
void skipSpaces(char* expr, int* index); // Skips spaces
int isPrefix(const char* prefix); // Determines whether the expression is valid prefix
Node* prefix_to_tree(char* prefix, int* index); // Puts the prefix expression in a binary tree
void preorder_Traversal(Node* root); // Traverses in preorder
void inorder_Traversal(Node* root); // Traverses in inorder
void postorder_Traversal(Node* root); // Traverses in postorder
int precedence(char op); // Determines operator precedence
int isInfix(const char* infix); // Determines whether it is a valid infix
void infix_to_postfix(const char* infix, char* postfix); // Converts infix to postfix expression
void reverse(char* str); // Reverses a string (used for infix to prefix conversion)
void infix_to_prefix(const char* infix, char* prefix); // Converts infix to prefix
void printHelp(); // Prints help information
void printGuide(); // Prints detailed guide with explanations and examples

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printHelp();
    } else if (argc <= 3) { // If argument is one
        // Validate argument for help or guide
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            // Handle help flag
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "--guide") == 0) {
            // Handle guide flag
            printGuide();
            return 0;
        } else {
            printf("Error: Missing or invalid argument.\n");
            return 1;
        }
    } // If arguments are more than one
    else if (argc > 6) {
        printf("Error: Multiple expressions provided. Mathematical expressions containing spaces must be enclosed in quotation marks.\n");
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
            return 1;
        } else if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'.\n", argv[4]);
            return 1;
        } else if (argc == 5) {
            printf("Error: Missing expression.\n");
            return 1;
        } else if (strcmp(argv[1], "--from") == 0) {
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

// Creates new binary node
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

// Checks if the character is an operand
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

// Determines whether the expression is valid postfix
int isPostfix(const char* postfix) {
    int operandCount = 0; 
    int operatorCount = 0;
    for (int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];
        if (isASpace(token)) continue;
        if (isOperand(token)) {
            operandCount++;
        } else if (isOperator(token)) {
            if (operandCount < 2) return 2;
            operatorCount++;
            operandCount--;
        } else {
            return 0;
        }
    }
    if (operatorCount == operandCount)
        return 1;
    else if (operatorCount > operandCount)
        return 2;
    else if (operatorCount < operandCount)
        return 3;
    return 0;
}

// Puts the postfix expression in a binary tree
Node* postfix_to_tree(char* postfix) {
    if (isInfix(postfix)) {
        printf("Error: Malformed Expression (This is an infix expression).\n");
        return NULL;
    } else if (isPrefix(postfix)) {
        printf("Error: Malformed Expression (This is a prefix expression).\n");
        return NULL;
    }
    int validpostfix = isPostfix(postfix);
    if (validpostfix == 1) {
        Stack* stack = NULL;
        for (int i = 0; postfix[i] != '\0'; i++) {
            char token = postfix[i];
            if (isASpace(token)) {
                continue;
            }
            if (isOperand(token)) {
                push(&stack, newNode(token));
            }
            if (isOperator(token)) {
                Node* right = pop(&stack);
                Node* left = pop(&stack);
                Node* opNode = newNode(token);
                opNode->left = left;
                opNode->right = right;
                push(&stack, opNode);
                // Free popped nodes
                if (left) free(left);
                if (right) free(right);
            }
        }
        Node* result = pop(&stack);
        // Clean up stack
        while (stack) {
            Node* node = pop(&stack);
            if (node) free(node);
        }
        return result;
    } else if (validpostfix == 2) {
        printf("Error: Malformed expression (e.g., insufficient operand).\n");
    } else if (validpostfix == 3) {
        printf("Error: Malformed expression (e.g., insufficient operator).\n");
    }
    return NULL;
}

// Skips spaces
void skipSpaces(char* expr, int* index) {
    while (expr[*index] == ' ') (*index)++;
}

// Determines whether the expression is valid prefix
int isPrefix(const char* prefix) {
    int operandCount = 0;
    int operatorCount = 0;
    int len = 0;
    while (prefix[len] != '\0') len++;
    for (int i = len - 1; i >= 0; i--) {
        char token = prefix[i];
        if (isASpace(token)) continue;
        if (isOperand(token)) {
            operandCount++;
        } else if (isOperator(token)) {
            if (operandCount < 2) return 2;
            operatorCount++;
            operandCount--;
        } else {
            return 0;
        }
    }
    if (operatorCount == operandCount)
        return 1;
    else if (operatorCount > operandCount)
        return 2;
    else if (operatorCount < operandCount)
        return 3;
    return 0;
}

// Puts the prefix expression in a binary tree
Node* prefix_to_tree(char* prefix, int* index) {
    if (isInfix(prefix) == 1) {
        printf("Error: Malformed Expression (This is an infix expression).\n");
        return NULL;
    } else if (isPostfix(prefix) == 1) {
        printf("Error: Malformed Expression (This is a postfix expression).\n");
        return NULL;
    }
    int validprefix = isPrefix(prefix);
    if (validprefix == 1) {
        skipSpaces(prefix, index);
        char token = prefix[*index];
        if (token == '\0') return NULL;
        Node* node = newNode(token);
        (*index)++;
        skipSpaces(prefix, index);
        if (isOperator(token)) {
            node->left = prefix_to_tree(prefix, index);
            node->right = prefix_to_tree(prefix, index);
            if (!node->left || !node->right) {
                printf("Error: Malformed expression.\n");
                free(node);
                return NULL;
            }
        }
        return node;
    } else if (validprefix == 2) {
        printf("Error: Malformed expression (e.g., insufficient operand).\n");
    } else if (validprefix == 3) {
        printf("Error: Malformed expression (e.g., insufficient operator).\n");
    }
    return NULL;
}

// Traverses in preorder
void preorder_Traversal(Node* root) {
    if (root != NULL) { 
        printf("%c ", root->data);
        preorder_Traversal(root->left);
        preorder_Traversal(root->right);
    }
}

// Traverses in inorder
void inorder_Traversal(Node* root) {
    if (root == NULL)
        return;
    if (isOperator(root->data)) {
        printf("(");
        inorder_Traversal(root->left);
        printf(" %c ", root->data);
        inorder_Traversal(root->right);
        printf(")");
    } else {
        printf("%c", root->data);
    }
}

// Traverses in postorder
void postorder_Traversal(Node* root) {
    if (root == NULL) return;
    postorder_Traversal(root->left);
    postorder_Traversal(root->right);
    printf("%c ", root->data);
}

// Determines operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Determines whether it is a valid infix
int isInfix(const char* infix) {
    int balance = 0;
    int expectOperand = 1;
    int needOperator = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        char token = infix[i];
        if (isASpace(token)) continue;
        if (token == '(') {
            balance++;
        } else if (token == ')') {
            balance--;
            if (balance < 0) return 0;
        } else if (isOperand(token)) {
            if (!expectOperand) needOperator = 1;
            expectOperand = 0;
        } else if (isOperator(token)) {
            if (expectOperand) return 2;
            expectOperand = 1;
        } else {
            return 0;
        }
    }
    if (balance == 0 && expectOperand == 0 && needOperator == 0)
        return 1;
    else if (expectOperand == 1)
        return 2;
    else if (needOperator == 1)
        return 3;
    return 0;
}

// Converts infix to postfix expression
void infix_to_postfix(const char* infix, char* postfix) {
    if (isPostfix(infix) == 1) {
        printf("Error: Malformed Expression (This is a postfix expression).\n");
        return;
    } else if (isPrefix(infix) == 1) {
        printf("Error: Malformed Expression (This is a prefix expression).\n");
        return;
    }
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
    Stack* opStack = NULL;
    push(&opStack, newNode('('));
    int j = 0;
    for (int i = 0; infix[i]; i++) {
        char token = infix[i];
        if (isASpace(token)) continue;
        if (token == '(') {
            push(&opStack, newNode(token));
        } else if (isOperand(token)) {
            if (j > 0) postfix[j++] = ' ';
            postfix[j++] = token;
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
                if (j > 0) postfix[j++] = ' ';
                Node* node = pop(&opStack);
                postfix[j++] = node->data;
                free(node);
            }
            if (opStack && opStack->treeNode->data == '(') {
                Node* node = pop(&opStack);
                free(node);
            } else {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
        } else if (isOperator(token)) {
            if (j > 0) postfix[j++] = ' ';
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) >= precedence(token)) {
                if (j > 0) postfix[j++] = ' ';
                Node* node = pop(&opStack);
                postfix[j++] = node->data;
                free(node);
            }
            push(&opStack, newNode(token));
        }
    }
    while (opStack) {
        Node* node = pop(&opStack);
        if (node->data == '(') {
            printf("Error: Mismatched opening parenthesis.\n");
            free(node);
            return;
        }
        if (j > 0) postfix[j++] = ' ';
        postfix[j++] = node->data;
        free(node);
    }
    postfix[j] = '\0';
}

// Reverses a string and swaps parentheses
void reverse(char* str) {
    int len = strlen(str);
    int i = 0, j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        if (str[i] == '(') str[i] = ')';
        else if (str[i] == ')') str[i] = '(';
        if (str[j] == '(') str[j] = ')';
        else if (str[j] == ')') str[j] = '(';
        i++;
        j--;
    }
    if (i == j) {
        if (str[i] == '(') str[i] = ')';
        else if (str[i] == ')') str[i] = '(';
    }
}

// Converts infix to prefix
void infix_to_prefix(const char* infix, char* prefix) {
    if (isPostfix(infix) == 1) {
        printf("Error: Malformed Expression (This is a postfix expression).\n");
        return;
    } else if (isPrefix(infix) == 1) {
        printf("Error: Malformed Expression (This is a prefix expression).\n");
        return;
    }
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
    char reversed[100] = {0};
    strcpy(reversed, infix);
    reverse(reversed);
    char postfix[100] = {0};
    Stack* opStack = NULL;
    int j = 0;
    for (int i = 0; reversed[i]; i++) {
        char token = reversed[i];
        if (isASpace(token)) continue;
        if (token == '(') {
            push(&opStack, newNode(token));
        } else if (isOperand(token)) {
            if (j > 0) postfix[j++] = ' ';
            postfix[j++] = token;
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
                if (j > 0) postfix[j++] = ' ';
                Node* node = pop(&opStack);
                postfix[j++] = node->data;
                free(node);
            }
            if (opStack && opStack->treeNode->data == '(') {
                Node* node = pop(&opStack);
                free(node);
            } else {
                printf("Error: Mismatched closing parenthesis.\n");
                while (opStack) {
                    Node* node = pop(&opStack);
                    free(node);
                }
                return;
            }
        } else if (isOperator(token)) {
            if (j > 0) postfix[j++] = ' ';
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) > precedence(token)) {
                if (j > 0) postfix[j++] = ' ';
                Node* node = pop(&opStack);
                postfix[j++] = node->data;
                free(node);
            }
            push(&opStack, newNode(token));
        }
    }
    while (opStack) {
        Node* node = pop(&opStack);
        if (j > 0) postfix[j++] = ' ';
        postfix[j++] = node->data;
        free(node);
    }
    postfix[j] = '\0';
    prefix[0] = '\0'; // Initialize prefix to empty string
    strcpy(prefix, postfix);
    reverse(prefix);
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
