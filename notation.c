#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defines a binary code with left and right child
typedef struct Node {
    char data;
    struct Node *left, *right;
} Node;

//points to next node; used in Postfix_to_tree function
typedef struct Stack {
    Node *treeNode;
    struct Stack *next;
} Stack;

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
Node* postfix_to_tree(char* postfix) {
    Stack* stack = NULL; //initializes empty stack
    
    //for loop that loop each character in the string until null terminator is found
    for (int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i]; //each character is stored in token variable

        if (isASpace(token)) {  //if space, increment index
            i++;
        }

        if (isOperand(token)) { // if char is operand, push to stack
            push(&stack, newNode(token));
        }

        if (isOperator(token)) { //if char is operator, pop two nodes
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
Node* prefix_to_tree(char* expr, int* index) {
    skipSpaces(expr, index);  //skip spaces
    char ch = expr[*index];  //read the current character

    if (ch == '\0') return NULL; //if token is null terminator return NULL

    Node* node = newNode(ch); //create a newnode with token
    (*index)++; // increment index
    skipSpaces(expr, index);

    if (isOperator(ch)) { //if token is an operator
        node->left = prefix_to_tree(expr, index); //recursively build left subtree
        node->right = prefix_to_tree(expr, index); //recursively build right subtree
    }

    return node; //return node
}

//function to traverse in preorder
void preorder_Traversal(Node* root) {
    if (root != NULL) {
        printf("%c ", root->data);  // Visit root
        preorder_Traversal(root->left);        // Traverse left
        preorder_Traversal(root->right);       // Traverse right
    }
}

//function to traverse in inorder
void inorder_Traversal(Node* root) {
    if (root == NULL) return;


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

int main(int argc, char *argv[]) {
    // Error Handling
    // Validate correct number of arguments
    if (argc < 6) {
        printf("Error: Missing or incomplete arguments.\n");
        printf("Correct Syntax: notation-converter --from <format> --to <format> \"<expression>\"\n");
        return 1;
    }

    // Validate '--from' and '--to' Arguments
    if (strcmp(argv[1], "--from") != 0 || strcmp(argv[3], "--to") != 0) {
        printf("Error: Missing or invalid '--from' or '--to' argument.\n");
        return 1;
    }

    // Validate Format Specifiers
    if (strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0) {
        printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[2]);
        return 1;
    }

    if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
        printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[4]);
        return 1;
    }

    // Expression conversion handling
    // Display Help instructions
    if (strcmp(argv[1], "--h") == 0) {
        printf("\nhelp...");
        // help function

    // Display user's guide
    } else if (strcmp(argv[1], "--guide") == 0) {
        printf("\nguide...");
        // guide function
    } else if (strcmp(argv[1], "--from") == 0) {
        //Infix to postfix
        if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)) {
            printf("%s\n", argv[5]);

            char postfix[100];
            infix_to_postfix(argv[5], postfix);

            printf("Postfix expression: %s\n", postfix);

        //Infix to prefix
        } else if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)) {
            printf("Infix expression: %s\n", argv[5]);

            char prefix[100];
            infix_to_prefix(argv[5], prefix);

            printf("Prefix expression: %s\n", prefix);
        } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)) {
            int index = 0;
            printf("%s\n", argv[5]);
            Node* root = prefix_to_tree(argv[5], &index);

            printf("Infix expression: ");
            inorder_Traversal(root);
            printf("\n");
        } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)) {
            printf("%s\n", argv[5]);
            Node* root = postfix_to_tree(argv[5]);

            printf("Infix expression: ");
            inorder_Traversal(root);
            printf("\n");
        } else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)) {
            printf("%s\n", argv[5]);
            Node* root = postfix_to_tree(argv[5]);

            printf("Prefix expression: ");
            preorder_Traversal(root);
            printf("\n");
        } else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)) {
            int index = 0;
            printf("%s\n", argv[5]);
            Node* root = prefix_to_tree(argv[5], &index);

            printf("Postfix expression: ");
            postorder_Traversal(root);
            printf("\n");
        }
    }

    return 0;
}
