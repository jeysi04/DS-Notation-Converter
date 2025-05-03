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
int isOperand(char ch){
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
           return 1;
       else
           return 0;
}

//checks if the character is an operator
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

//function to put the postfix expression in a binary tree
Node* postfix_to_tree(char* postfix){
    Stack* stack = NULL; //initializes empty stack

    //for loop that loop each character in the string until null terminator is found
    for(int i=0; postfix[i] != '\0'; i++){
        char token = postfix[i]; //each character is stored in token variable

        if(isASpace(token)){ //if space, increment index
            i++;
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

    if (token == '\0') //if token is null terminator
        return NULL; ///return NULL

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
            else if(strcmp(argv[1], "--from" ) == 0){
                if((strcmp(argv[3], "to") == 0)){
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
                    else
                        printf("Invalid specifier.");
                }
                else
                    printf("Missing --to argument.");
            }
            else
                printf("\nInvalid expression.");
           

    }
    return 0;
}