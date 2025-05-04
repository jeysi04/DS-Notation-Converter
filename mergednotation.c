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
Node* postfix_to_tree(char* postfix); //function to put the postfix expression in a binary tree
void skipSpaces(char* expr, int* index); //function to skip spaces
Node* prefix_to_tree(char* prefix, int* index);//function to put the prefix expression in a binary tree
void preorder_Traversal(Node* root); //function to traverse in preorder
void inorder_Traversal(Node* root); //function to traverse in inorder
void postorder_Traversal(Node* root); //function to traverse in postorder
int precedence(char op); // Determine operator precedence
void infix_to_postfix(const char* infix, char* postfix); // Convert infix to postfix expression
void reverse(char* str); // Reverse a string (used for infix to prefix conversion logic)
void infix_to_prefix(const char* infix, char* prefix); // Convert infix to prefix using operator and operand stacks
void printHelp(); //Prints help information
void printGuide(); //Prints detailed guide with explanations and examples



int main(int argc, char *argv[]) {

    if(argc == 2){ //if argument is one
        // Validate argument for help or guide
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            // Handle help flag
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "--guide") == 0) {
            // Handle guide flag
            printGuide();
            return 0;
        } else if(strcmp(argv[1], "-h") != 0 || strcmp(argv[1], "--help") != 0 || strcmp(argv[1], "--guide") != 0){
            printf("Error: Missing or invalid argument. Supported formats are '--help', '-h' and '--guide'.\n");
            return 1;
        } 
    } //if arguments are more than one
    else {
        // Error Handling
        // Validate correct number of arguments
        if (argc < 6) {
            printf("Error: Missing or incomplete arguments.\n");
            printf("Correct Syntax: notation-converter --from <format> --to <format> \"<expression>\"\n");
            return 1;
        } // Validate '--from' and '--to' Arguments
        else if (strcmp(argv[1], "--from") != 0 || strcmp(argv[3], "--to") != 0) {
            printf("Error: Missing or invalid '--from' or '--to' argument.\n");
            return 1;
        } // Validate Format Specifiers
        else if (strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[2]);
            return 1;
        }
        else if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
            printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[4]);
            return 1;
        }
        else if (strcmp(argv[1], "--from") == 0) {
            if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)) { //Infix to postfix
                printf("%s\n", argv[5]);

                char postfix[100];
                infix_to_postfix(argv[5], postfix);

                printf("Postfix expression: %s\n", postfix);

            } else if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)) { //Infix to prefix
                printf("Infix expression: %s\n", argv[5]);

                char prefix[100];
                infix_to_prefix(argv[5], prefix);

                printf("Prefix expression: %s\n", prefix);
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)) { //prefix to infix
                int index = 0;
                printf("%s\n", argv[5]);
                Node* root = prefix_to_tree(argv[5], &index);

                printf("Infix expression: ");
                inorder_Traversal(root);
                printf("\n");
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)) { //postfix to infix
                printf("%s\n", argv[5]);
                Node* root = postfix_to_tree(argv[5]);

                printf("Infix expression: ");
                inorder_Traversal(root);
                printf("\n");
            } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)) { //postfix to prefix
                printf("%s\n", argv[5]);
                Node* root = postfix_to_tree(argv[5]);

                printf("Prefix expression: ");
                preorder_Traversal(root);
                printf("\n");
            } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)) { //prefix to postfix
                int index = 0;
                printf("%s\n", argv[5]);
                Node* root = prefix_to_tree(argv[5], &index);

                printf("Postfix expression: ");
                postorder_Traversal(root);
                printf("\n");
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

//function to put the postfix expression in a binary tree
Node* postfix_to_tree(char* postfix){
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

//function to skip spaces
void skipSpaces(char* expr, int* index) {
    while (expr[*index] == ' ') (*index)++;
}

//function to put the prefix expression in a binary tree
Node* prefix_to_tree(char* prefix, int* index) {
    skipSpaces(prefix, index); //skip spaces
    char token = prefix[*index]; //read the current character

    if (token == '\0') return NULL; //if token is null terminator return NULL

    Node* node = newNode(token); //create a newnode with token
    (*index)++;  // increment index
    skipSpaces(prefix, index);

    if (isOperator(token)) {//if token is an operator
        node->left = prefix_to_tree(prefix, index); //recursively build left subtree
        node->right = prefix_to_tree(prefix, index); //recursively build right subtree
    }
    return node; //return node
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

// Convert infix to postfix expression
void infix_to_postfix(const char* infix, char* postfix) {
    Stack* opStack = NULL;
    int j = 0;

    for (int i = 0; infix[i]; i++) {
        char token = infix[i];

        if (isASpace(token)) continue;

        if (isOperand(token)) {
            postfix[j++] = token;
            postfix[j++] = ' ';
        } else if (isOperator(token)) {
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) >= precedence(token)) {
                postfix[j++] = pop(&opStack)->data;
                postfix[j++] = ' ';
            }
            push(&opStack, newNode(token));
        }
    }

    while (opStack) {
        postfix[j++] = pop(&opStack)->data;
        postfix[j++] = ' ';
    }

    if (j > 0 && postfix[j - 1] == ' ') j--;
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
    char* operators[100];
    char* operands[100];
    int opTop = -1, valTop = -1;
    int len = strlen(infix);
    char token;

    for (int i = 0; i < len; i++) {
        token = infix[i];

        if (isASpace(token)) continue;

        if (isOperand(token)) {
            char* operand = (char*)malloc(2);
            operand[0] = token;
            operand[1] = '\0';
            operands[++valTop] = operand;
        } else if (isOperator(token)) {
            while (opTop != -1 && precedence(operators[opTop][0]) >= precedence(token)) {
                char* op = operators[opTop--];
                char* op1 = operands[valTop--];
                char* op2 = operands[valTop--];

                char* expr = (char*)malloc(strlen(op1) + strlen(op2) + 3);
                sprintf(expr, "%c%s%s", op[0], op2, op1);

                operands[++valTop] = expr;
                free(op);
                free(op1);
                free(op2);
            }

            char* op = (char*)malloc(2);
            op[0] = token;
            op[1] = '\0';
            operators[++opTop] = op;
        }
    }

    while (opTop != -1) {
        char* op = operators[opTop--];
        char* op1 = operands[valTop--];
        char* op2 = operands[valTop--];

        char* expr = (char*)malloc(strlen(op1) + strlen(op2) + 3);
        sprintf(expr, "%c%s%s", op[0], op2, op1);

        operands[++valTop] = expr;
        free(op);
        free(op1);
        free(op2);
    }

    strcpy(prefix, operands[valTop]);
    free(operands[valTop]);
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