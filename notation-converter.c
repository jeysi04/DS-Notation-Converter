#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* 
 * TreeNode structure represents a node in the expression tree
 * - data: stores either an operator (+, -, *, /) or operand (0-9)
 * - left: pointer to left child node
 * - right: pointer to right child node
 */
typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/*
 * Stack structure used during tree construction
 * - items: array of TreeNode pointers
 * - top: index of top element
 * - capacity: maximum capacity of the stack
 */
typedef struct Stack {
    TreeNode **items;
    int top;
    int capacity;
} Stack;

// Function prototypes
TreeNode* createNode(char data);
Stack* createStack(int capacity);
void push(Stack *stack, TreeNode *node);
TreeNode* pop(Stack *stack);
bool isEmpty(Stack *stack);
bool isOperator(char c);
int precedence(char op);
int countTokens(const char *str);
const char* getNextToken(const char *str, char *token);
TreeNode* buildTreeFromPostfix(char *postfix);
TreeNode* buildTreeFromPrefix(char *prefix);
TreeNode* buildTreeFromInfix(char *infix);
void inorderTraversal(TreeNode *root);
void preorderTraversal(TreeNode *root);
void postorderTraversal(TreeNode *root);
void freeTree(TreeNode *root);
bool validateExpression(char *expr, const char *format);
char* infixToPostfix(char *infix);
void printHelp();
void printGuide();

int main(int argc, char *argv[]) {
    // Initialize variables to store command line arguments
    char *inputExpr = NULL;
    char *fromFormat = NULL;
    char *toFormat = NULL;
    bool showHelp = false;
    bool showGuide = false;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--from") == 0) {
            // Handle --from format argument
            if (i + 1 < argc) {
                fromFormat = argv[++i];
            }
        } else if (strcmp(argv[i], "--to") == 0) {
            // Handle --to format argument
            if (i + 1 < argc) {
                toFormat = argv[++i];
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            // Handle help flag
            showHelp = true;
        } else if (strcmp(argv[i], "--guide") == 0) {
            // Handle guide flag
            showGuide = true;
        } else {
            // Handle expression argument
            // Check for expressions not enclosed in double quotes - treated as multiple arguments
            if (inputExpr != NULL) {
                printf("Error: Multiple expressions provided.\n");
                return 1;
            }

            // Assign the expression to inputExpr
            inputExpr = argv[i];
        }
    }

    // Show help or guide if requested and exit
    if (showHelp) {
        printHelp();
        return 0;
    }

    if (showGuide) {
        printGuide();
        return 0;
    }

    // Validate required arguments
    if (!inputExpr || !fromFormat || !toFormat) {
        printf("Error: Missing required arguments.\n");
        printf("Usage: %s --from <input_format> --to <output_format> \"<expression>\"\n", argv[0]);
        printf("Use -h or --help for more information.\nFormat: %s -h\n", argv[0]);
        return 1;
    }

    // Validate input format specifier
    if (strcmp(fromFormat, "infix") != 0 && strcmp(fromFormat, "prefix") != 0 && strcmp(fromFormat, "postfix") != 0) {
        printf("Error: Invalid input format '%s'. Must be infix, prefix, or postfix.\n", fromFormat);
        return 1;
    }

    // Validate output format specifier
    if (strcmp(toFormat, "infix") != 0 && strcmp(toFormat, "prefix") != 0 && strcmp(toFormat, "postfix") != 0) {
        printf("Error: Invalid output format '%s'. Must be infix, prefix, or postfix.\n", toFormat);
        return 1;
    }

    // Validate the expression syntax
    if (!validateExpression(inputExpr, fromFormat)) {
        return 1;
    }

    // Build the expression tree based on input format
    TreeNode *root = NULL;
    if (strcmp(fromFormat, "postfix") == 0) {
        root = buildTreeFromPostfix(inputExpr);
    } else if (strcmp(fromFormat, "prefix") == 0) {
        root = buildTreeFromPrefix(inputExpr);
    } else if (strcmp(fromFormat, "infix") == 0) {
        root = buildTreeFromInfix(inputExpr);
    }

    // Check if tree construction was successful
    if (!root) {
        printf("Error: Failed to build expression tree.\n");
        return 1;
    }

    // Generate output based on requested format
    if (strcmp(toFormat, "infix") == 0) {
        printf("Converted expression: ");
        inorderTraversal(root);
        printf("\n");
    } else if (strcmp(toFormat, "prefix") == 0) {
        printf("Converted expression: ");
        preorderTraversal(root);
        printf("\n");
    } else if (strcmp(toFormat, "postfix") == 0) {
        printf("Converted expression: ");
        postorderTraversal(root);
        printf("\n");
    }

    // Free the allocated memory for the expression tree
    freeTree(root);

    return 0;
}

/*
 * Creates a new tree node with given data
 * Parameters:
 *   data - character to store in the node (operator or operand)
 * Returns:
 *   Pointer to newly created node, or NULL if memory allocation fails
 */
TreeNode* createNode(char data) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        node->data = data;
        node->left = node->right = NULL;
    }
    return node;
}

/*
 * Creates a new stack with given capacity
 * Parameters:
 *   capacity - maximum number of items the stack can hold
 * Returns:
 *   Pointer to newly created stack, or NULL if memory allocation fails
 */
Stack* createStack(int capacity) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack) {
        stack->items = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
        stack->top = -1;
        stack->capacity = capacity;
    }
    return stack;
}

/*
 * Pushes a node onto the stack
 * Parameters:
 *   stack - pointer to the stack
 *   node - pointer to tree node to push
 */
void push(Stack *stack, TreeNode *node) {
    if (stack->top < stack->capacity - 1) {
        stack->items[++stack->top] = node;
    }
}

/*
 * Pops a node from the stack
 * Parameters:
 *   stack - pointer to the stack
 * Returns:
 *   Pointer to popped tree node, or NULL if stack is empty
 */
TreeNode* pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top--];
    }
    return NULL;
}

/*
 * Checks if stack is empty
 * Parameters:
 *   stack - pointer to the stack
 * Returns:
 *   true if stack is empty, false otherwise
 */
bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

/*
 * Checks if character is an operator
 * Parameters:
 *   c - character to check
 * Returns:
 *   true if character is +, -, *, or /, false otherwise
 */
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

/*
 * Gets operator precedence
 * Parameters:
 *   op - operator character
 * Returns:
 *   2 for * and /, 1 for + and -, 0 otherwise
 */
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Helper function to count tokens
int countTokens(const char *str) {
    int count = 0;
    bool inToken = false;
    
    while (*str) {
        if (isspace(*str)) {
            inToken = false;
        } else if (!inToken) {
            count++;
            inToken = true;
        }
        str++;
    }
    return count;
}

// Helper function to get next token
const char* getNextToken(const char *str, char *token) {
    while (*str && isspace(*str)) str++;
    
    if (!*str) return NULL;
    
    *token = *str;
    return str + 1;
}

/*
 * Builds expression tree from postfix notation
 * Parameters:
 *   postfix - string containing postfix expression
 * Returns:
 *   Pointer to root of constructed tree, or NULL on failure
 */
TreeNode* buildTreeFromPostfix(char *postfix) {
    // Create stack with capacity for all tokens
    Stack *stack = createStack(strlen(postfix));
    if (!stack) return NULL;

    // Tokenize the postfix expression
    char token;
    const char *ptr = postfix;
    while (ptr = getNextToken(ptr, &token)) {
        TreeNode *node = createNode(token);

        if (isOperator(token)) {
            node->right = pop(stack);
            node->left = pop(stack);
            if (!node->left || !node->right) {
                free(node);
                free(stack->items);
                free(stack);
                return NULL;
            }
        }
        push(stack, node);
    }

    TreeNode *root = pop(stack);
    free(stack->items);
    free(stack);
    
    if (!isEmpty(stack)) {
        freeTree(root);
        return NULL;
    }
    return root;
}

/*
 * Builds expression tree from prefix notation (iterative approach)
 * Parameters:
 *   prefix - string containing prefix expression
 * Returns:
 *   Pointer to root of constructed tree, or NULL on failure
 */
TreeNode* buildTreeFromPrefix(char *prefix) {
    // Create stack with capacity for all tokens
    Stack *stack = createStack(strlen(prefix));
    if (!stack) return NULL;

    // Temporary storage for tokens
    char tokens[100][2];            // Assume max 100 tokens, each max 1 char + null terminator
    int tokenCount = 0;
    
    // Manual tokenization
    const char *ptr = prefix;
    while (*ptr) {
        // Skip whitespace
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        // Store token
        tokens[tokenCount][0] = *ptr;
        tokens[tokenCount][1] = '\0';
        tokenCount++;
        ptr++;
    }

    // Process tokens in reverse order
    for (int i = tokenCount - 1; i >= 0; i--) {
        char c = tokens[i][0];
        TreeNode *node = createNode(c);

        if (isOperator(c)) {
            // For operators: pop two operands and make them children
            node->left = pop(stack);
            node->right = pop(stack);
            if (!node->left || !node->right) {
                // Not enough operands for operator
                free(node);
                free(stack->items);
                free(stack);
                return NULL;
            }
        }

        // Push node onto stack (operator with children or operand)
        push(stack, node);
    }

    // Final tree is the only node left on stack
    TreeNode *root = pop(stack);
    free(stack->items);
    free(stack);

    // If stack isn't empty, expression was malformed
    if (!isEmpty(stack)) {
        freeTree(root);
        return NULL;
    }

    return root;
}

/*
 * Builds expression tree from infix notation (converts to postfix first)
 * Parameters:
 *   infix - string containing infix expression
 * Returns:
 *   Pointer to root of constructed tree, or NULL on failure
 */
TreeNode* buildTreeFromInfix(char *infix) {
    // First convert infix to postfix notation
    char *postfix = infixToPostfix(infix);
    if (!postfix) return NULL;
    
    // Then build tree from postfix notation
    TreeNode *root = buildTreeFromPostfix(postfix);
    free(postfix);
    return root;
}

/*
 * Converts infix expression to postfix notation using Shunting-yard algorithm
 * Parameters:
 *   infix - string containing infix expression
 * Returns:
 *   String containing postfix expression, or NULL on failure
 */
char* infixToPostfix(char *infix) {
    // Create operator stack
    Stack *opStack = createStack(strlen(infix));
    if (!opStack) return NULL;

    // Allocate memory for postfix expression
    char *postfix = (char*)malloc(2 * strlen(infix) + 1);   // Enough space
    if (!postfix) {
        free(opStack->items);
        free(opStack);
        return NULL;
    }
    postfix[0] = '\0';
    size_t postfix_len = 0;

    // Manual tokenization
    const char *ptr = infix;
    while (*ptr) {
        // Skip whitespace
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        char c = *ptr;
        ptr++;

        if (isdigit(c)) {
            // Operand - add to output
            postfix[postfix_len++] = c;
            postfix[postfix_len++] = ' ';
        } else if (c == '(') {
            // Left parenthesis - push to stack
            TreeNode *node = createNode(c);
            push(opStack, node);
        } else if (c == ')') {
            // Right parenthesis - pop until matching '('
            while (!isEmpty(opStack) && opStack->items[opStack->top]->data != '(') {
                postfix[postfix_len++] = pop(opStack)->data;
                postfix[postfix_len++] = ' ';
            }
            
            if (isEmpty(opStack)) {
                // Mismatched parentheses
                free(postfix);
                free(opStack->items);
                free(opStack);
                return NULL;
            }
            
            // Pop the '('
            free(pop(opStack));
        } else if (isOperator(c)) {
            // Operator - pop higher or equal precedence operators from stack
            while (!isEmpty(opStack) && opStack->items[opStack->top]->data != '(' && 
                   precedence(opStack->items[opStack->top]->data) >= precedence(c)) {
                postfix[postfix_len++] = pop(opStack)->data;
                postfix[postfix_len++] = ' ';
            }
            TreeNode *node = createNode(c);
            push(opStack, node);
        }
    }

    // Pop remaining operators from stack
    while (!isEmpty(opStack)) {
        if (opStack->items[opStack->top]->data == '(') {
            // Mismatched parentheses
            free(postfix);
            free(opStack->items);
            free(opStack);
            return NULL;
        }
        postfix[postfix_len++] = pop(opStack)->data;
        postfix[postfix_len++] = ' ';
    }

    // Remove trailing space if any
    if (postfix_len > 0 && postfix[postfix_len-1] == ' ') {
        postfix_len--;
    }
    postfix[postfix_len] = '\0';

    // Clean up stack
    free(opStack->items);
    free(opStack);
    return postfix;
}

/*
 * Performs in-order traversal of expression tree with EXACT output formatting
 * Parameters:
 *   root - current node in traversal
 */
void inorderTraversal(TreeNode *root) {
    if (!root) return;

    if (isOperator(root->data)) {
        printf("(");
        inorderTraversal(root->left);
        printf(" %c ", root->data);  // Space before and after operator
        inorderTraversal(root->right);
        printf(")");
    } else {
        printf("%c", root->data);    // Operand with no space
    }
}

/*
 * Performs pre-order traversal of expression tree (for prefix notation)
 * Parameters:
 *   root - pointer to root of tree
 */
void preorderTraversal(TreeNode *root) {
    if (!root) return;
    printf("%c ", root->data);          // Visit node
    preorderTraversal(root->left);      // Traverse left subtree
    preorderTraversal(root->right);     // Traverse right subtree
}

/*
 * Performs post-order traversal of expression tree (for postfix notation)
 * Parameters:
 *   root - pointer to root of tree
 */
void postorderTraversal(TreeNode *root) {
    if (!root) return;
    postorderTraversal(root->left);     // Traverse left subtree
    postorderTraversal(root->right);    // Traverse right subtree
    printf("%c ", root->data);          // Visit node
}

/*
 * Frees memory allocated for expression tree
 * Parameters:
 *   root - pointer to root of tree
 */
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/*
 * Validates expression syntax based on format
 * Parameters:
 *   expr - expression string to validate
 *   format - format to validate against (infix, prefix, postfix)
 * Returns:
 *   true if expression is valid, false otherwise
 */
bool validateExpression(char *expr, const char *format) {
    if (!expr || strlen(expr) == 0) {
        printf("Error: Empty expression.\n");
        return false;
    }

    int operatorCount = 0;
    int operandCount = 0;
    int parenCount = 0;
    bool lastWasSpace = true;   // Track if current character is preceded by space
    
    while (*expr) {
        if (isspace(*expr)) {
            lastWasSpace = true;
            expr++;
            continue;
        }
        
        // Check for space between tokens
        if (!lastWasSpace) {
            printf("Error: Missing space between tokens at '%c'.\n", *expr);
            return false;
        }
        lastWasSpace = false;

        char c = *expr;
        expr++;

        // Validate the character
        if (isdigit(c)) {
            operandCount++;
        } else if (isOperator(c)) {
            operatorCount++;
        } else if (c == '(' || c == ')') {
            parenCount++;
        } else {
            printf("Error: Invalid character '%c' in expression.\n", c);
            return false;
        }
    }
    
    // Validate structure based on format
    if (strcmp(format, "infix") == 0) {
        if (operandCount != operatorCount + 1) {
            printf("Error: Malformed %s expression.\n", format);
            return false;
        }
        if (parenCount % 2 != 0) {
            printf("Error: Mismatched parentheses in infix expression.\n");
            return false;
        }
    } 
    else if (strcmp(format, "postfix") == 0 || strcmp(format, "prefix") == 0) {
        if (operandCount != operatorCount + 1) {
            printf("Error: Invalid operator/operand balance in %s expression.\n", format);
            return false;
        }
        if (parenCount > 0) {
            printf("Error: Parentheses not allowed in %s notation.\n", format);
            return false;
        }
    }
    
    return true;
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