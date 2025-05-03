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
            continue;
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

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("No arguments passed.\n");
        return 0;
    } else {
            if(strcmp(argv[1], "--h") == 0)
                printf("\nhelp...");
                //help function
            else if(strcmp(argv[1], "--guide") == 0)
                printf("\nguide...");
                //guide function
            else if(strcmp(argv[1], "--from") == 0){
                 if((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "postfix") == 0)){
                        //infix_to_tree
                        //infix_to_postfix
                 }
                 else if((strcmp(argv[2], "infix") == 0) && (strcmp(argv[4], "prefix") == 0)){
                        //infix_to_tree
                        //infix_to_prefix
                 }
                 else if((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "infix") == 0)){
                        //prefix_to_tree
                        int index = 0;
                        printf("%s\n", argv[5]);
                        Node* root = prefix_to_tree(argv[5], &index);
                        //prefix_to_infix
                        printf("Infix expression: ");
                        inorder_Traversal(root); // Should print: * + 2 3 + 4 5
                        printf("\n");
                 }
                 else if((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "infix") == 0)){
                        //postfix_to_infix

                        printf("%s\n", argv[5]);
                        Node* root = postfix_to_tree(argv[5]);

                        printf("Infix expression: ");
                        inorder_Traversal(root); // Should print: * + 2 3 + 4 5
                        printf("\n");
                 }
                 else if((strcmp(argv[2], "postfix") == 0) && (strcmp(argv[4], "prefix") == 0)){
                        printf("%s\n", argv[5]);
                        Node* root = postfix_to_tree(argv[5]);

                        printf("Prefix expression: ");
                        preorder_Traversal(root); 
                        printf("\n");
                 }
                 else if((strcmp(argv[2], "prefix") == 0) && (strcmp(argv[4], "postfix") == 0)){
                        //prefix_to_tree
                        int index = 0;
                        printf("%s\n", argv[5]);
                        Node* root = prefix_to_tree(argv[5], &index);
                        //prefix_to_postfix
                        printf("Postfix expression: ");
                        postorder_Traversal(root); 
                        printf("\n");
                 }
            }

    }
    return 0;
}