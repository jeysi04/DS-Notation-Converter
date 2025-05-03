#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node{
    char data;
    struct Node *left, *right;
} Node;

typedef struct Stack {
    Node *treeNode;
    struct Stack *next;
} Stack;

Node* newNode(char op) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = op;
    node->left = node->right = NULL;
    return node;
}

void push(Stack** top, Node* node) {
    Stack* newStackNode = (Stack*)malloc(sizeof(Stack));
    newStackNode->treeNode = node;
    newStackNode->next = *top;
    *top = newStackNode;
}

Node* pop(Stack** top) {
    if (*top == NULL) return NULL;
    Stack* temp = *top;
    *top = (*top)->next;
    Node* node = temp->treeNode;
    free(temp);
    return node;
}

int isOperand(char ch){
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
           return 1;
       else
           return 0;
}

int isOperator(char ch){
    if(ch == '+' || ch == '/' || ch == '-' || ch == '*')
        return 1;
    else
        return 0;
}

//checks if the character is a space
int isASpace(char ch){
    if(ch == ' ')
        return 1;
    else
        return 0;
}


Node* postfix_to_tree(char* postfix){
    Stack* stack = NULL;
    for(int i=0; postfix[i] != '\0'; i++){
        char token = postfix[i];

        if(isASpace(token)){
            i++;
        }

        if(isOperand(token)){
            push(&stack, newNode(token));
        }
        if(isOperator(token)){
            Node* right = pop(&stack);
            Node* left = pop(&stack);
            Node* opNode = newNode(token);
            opNode->left = left;
            opNode->right = right;
            push(&stack, opNode);
        }
    }
    return pop(&stack);

}

//function to skip spaces
void skipSpaces(char* expr, int* index) {
    while (expr[*index] == ' ') (*index)++;
}

Node* prefix_to_tree(char* expr, int* index) {
    skipSpaces(expr, index);
    char ch = expr[*index];

    if (ch == '\0') return NULL;

    Node* node = newNode(ch);
    (*index)++;  // Move past current char
    skipSpaces(expr, index);

    if (isOperator(ch)) {
        node->left = prefix_to_tree(expr, index);
        node->right = prefix_to_tree(expr, index);
    }

    return node;
}

void preorder_Traversal(Node* root) {
    if (root != NULL) {
        printf("%c ", root->data);   // Visit root
        preorder_Traversal(root->left);        // Traverse left
        preorder_Traversal(root->right);       // Traverse right
    }
}

void inorder_Traversal(Node* root) {
    if (root == NULL)
        return;

    // If it's an operator, add parentheses
    if (isOperator(root->data)) {
        printf("(");
        inorder_Traversal(root->left);
        printf(" %c ", root->data);
        inorder_Traversal(root->right);
        printf(")");
    } else {
        // Operand (leaf node), just print it
        printf("%c", root->data);
    }
}

void postorder_Traversal(Node* root) {
    if (root == NULL) return;

    postorder_Traversal(root->left);
    postorder_Traversal(root->right);
    printf("%c ", root->data);
}

//Infix to postfix
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void infix_to_postfix(const char* infix, char* postfix) {
    Stack* opStack = NULL;  // Stack to store operators
    int j = 0;              // Index for postfix output

    for (int i = 0; infix[i]; i++) {
        char token = infix[i];

        if (isASpace(token)) continue;  //Skips spaces

        // Append operand followed by space
        if (isOperand(token)) {
            postfix[j++] = token;
            postfix[j++] = ' ';
        }
        
        else if (isOperator(token)) {
            while (opStack && isOperator(opStack->treeNode->data) &&
                   precedence(opStack->treeNode->data) >= precedence(token)) {
                postfix[j++] = pop(&opStack)->data;
                postfix[j++] = ' ';
            }

            // Push current operator to stack
            push(&opStack, newNode(token));
        }
        
    }

    // Pop any remaining operators from the stack to postfix
    while (opStack) {
        postfix[j++] = pop(&opStack)->data;
        postfix[j++] = ' ';
    }

    if (j > 0 && postfix[j - 1] == ' ') j--;  // Remove trailing space
    postfix[j] = '\0';
}

//Infix to Prefix

// Utility to reverse a string
void reverse(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void infix_to_prefix(const char* infix, char* prefix) {
    char* operators[100];          // Stack for operators
    char* operands[100];           // Stack for operand
    int opTop = -1, valTop = -1;
    int len = strlen(infix);
    char token;

    for (int i = 0; i < len; i++) {
        token = infix[i];

        if (isASpace(token)) continue;   // Skip whitespace

        if (isOperand(token)) {
            // Push operand  to operand stack
            char* operand = (char*)malloc(2);
            operand[0] = token;
            operand[1] = '\0';
            operands[++valTop] = operand;
        }
        else if (isOperator(token)) {
            while (opTop != -1 && precedence(operators[opTop][0]) >= precedence(token)) {
                // Pop operator and two operands
                char* op = operators[opTop--];
                char* op1 = operands[valTop--];
                char* op2 = operands[valTop--];

                // Combine them: operator + operand1 + operand2
                char* expr = (char*)malloc(strlen(op1) + strlen(op2) + 2 + 1);
                sprintf(expr, "%c%s%s", op[0], op2, op1);

                operands[++valTop] = expr;
                free(op);
                free(op1);
                free(op2);
            }
            // Push current operator
            char* op = (char*)malloc(2);
            op[0] = token;
            op[1] = '\0';
            operators[++opTop] = op;
        }
    }

    // Final pop
    while (opTop != -1) {
        char* op = operators[opTop--];
        char* op1 = operands[valTop--];
        char* op2 = operands[valTop--];

        char* expr = (char*)malloc(strlen(op1) + strlen(op2) + 2 + 1);
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

    // Check for enough arguments first
    if (argc < 6) {
        printf("Error: Missing arguments.\nUsage: notation-converter --from <format> --to <format> <expression>\n");
        return 1;
    }
    
    // Validate "--from" and "--to"
    if (strcmp(argv[1], "--from") != 0 || strcmp(argv[3], "--to") != 0) {
        printf("Error: Missing or invalid '--from' or '--to' argument.\n");
        return 1;
    }

    // Validate format specifiers
    if (strcmp(argv[2], "infix") != 0 && strcmp(argv[2], "prefix") != 0 && strcmp(argv[2], "postfix") != 0) {
        printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[2]);
        return 1;
    }

    if (strcmp(argv[4], "infix") != 0 && strcmp(argv[4], "prefix") != 0 && strcmp(argv[4], "postfix") != 0) {
        printf("Error: Invalid format specifier '%s'. Supported formats are 'infix', 'prefix', and 'postfix'.\n", argv[4]);
        return 1;
    }
    
    if (argc < 1) {
        printf("No arguments passed.\n");
        return 0;
    } else {
        if (strcmp(argv[1], "--h") == 0)
            printf("\nhelp...");
            //help function
        else if (strcmp(argv[1], "--guide") == 0)
            printf("\nguide...");
            //guide function
        else if (strcmp(argv[1], "--from") == 0) {
            if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)) {
                printf("%s\n", argv[5]);

                char postfix[100];
                infix_to_postfix(argv[5], postfix);

                printf("Postfix expression: %s\n", postfix);
            }
            else if ((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)) {
                printf("Infix expression: %s\n", argv[5]);

                char prefix[100];
                infix_to_prefix(argv[5], prefix);

                printf("Prefix expression: %s\n", prefix);
            }
            else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)) {
                int index = 0;
                printf("%s\n", argv[5]);
                Node* root = prefix_to_tree(argv[5], &index);
                printf("Infix expression: ");
                inorder_Traversal(root);
                printf("\n");
            }
            else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)) {
                printf("%s\n", argv[5]);
                Node* root = postfix_to_tree(argv[5]);
                printf("Infix expression: ");
                inorder_Traversal(root);
                printf("\n");
            }
            else if ((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)) {
                printf("%s\n", argv[5]);
                Node* root = postfix_to_tree(argv[5]);
                printf("Prefix expression: ");
                preorder_Traversal(root);
                printf("\n");
            }
            else if ((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)) {
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
