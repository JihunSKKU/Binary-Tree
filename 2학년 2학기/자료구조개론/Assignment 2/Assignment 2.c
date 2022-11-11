#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LEN 200
#define MAX_TREE_SIZE 53

typedef struct Node* treePointer;
typedef struct Node {
    char data;
    int level; // 해당 node의 level variable
    treePointer leftChild, rightChild;
}Node;

char *input; // char pointer array of inputed string for user
char *inorder; // char pointer array of inorder traversal
char *postorder; // char pointer array of postorder traversal
treePointer binary_tree; // char pointer array of corresponding binary tree
// two-dimensional char pointer array to store the binary_tree in the given format
char **display_binary_tree; 
int tree_size; // number of alphabet in binary_tree
int tree_depth; // depth of binary_tree

/* 새로운 node를 만들고 return하는 함수 */
treePointer newNode(char data, int level) {
    treePointer node = (treePointer)malloc(sizeof(Node));
    node->data = data;
    node->level = level;
    node->leftChild = node->rightChild = NULL;
    return node;
} // end function newNode

/* inorder에서 해당 data의 index를 찾고 return하는 함수 */
int find_idx(char data) {
    int i, idx;
    // inorder의 앞에서부터 data가 나올 때 까지 idx를 증가시키며 해당 idx를 찾는다.
    for(idx = 0 ; inorder[idx] != data ; idx++);

    // 해당 data를 inorder array에서 뺀다.
    for(i = idx ; inorder[i] != '\0' ; i++)
        inorder[i] = inorder[i+1];

    return idx;
} // end function find_idx

/* inorder와 postorder를 이용하여 binary tree를 만드는 함수 */
treePointer buildTree(int front_idx, int last_idx, int level) {
    if(front_idx > last_idx)
        /* Case of NULL */
        return NULL;
        
    char data = postorder[last_idx]; // postorder의 마지막 data가 현재 tree의 head node
    treePointer node = newNode(data, level); // 현재 data와 level로 새로운 node를 만든다.
    /* Update tree_depth. */
    if(node->level > tree_depth)
        tree_depth = node->level;
    
    /* Case of this node has no child */
    if(front_idx == last_idx)
        return node;
    
    // 현재 node의 data가 inorder에서 몇 번째 index에 있는지 찾는다.
    int inorder_idx = find_idx(node->data); 
    level++; // child의 level은 현재 level보다 1 높게 setting한다.
    // 앞에서 구한 inorder_idx를 기준으로 왼쪽은 leftChild, 오른쪽은 rightChild로 subtree를 만든다.
    node->rightChild = buildTree(inorder_idx, last_idx - 1, level);
    node->leftChild = buildTree(front_idx, inorder_idx - 1, level);
    return node;
} // end function buildTree

/* 2^n을 return하는 함수 */
int pow2(int n) {
    int i, value = 1;
    for(i = 0 ; i < n ; i++)
        value *= 2;
    return value;
} // end function pow2

/* 해당 node에서 child node로 갈 때 필요한 '/' or '\'의 개수를 return하는 함수 */
int number_of_slash(treePointer node) {
    if(tree_depth - node->level == 1)
        return 1;
    return 3 * pow2(tree_depth - node->level - 2) - 1;
} // end function number_of_slash

/* binary tree를 출력할 display_binary_tree 2nd array를 setting하는 함수 */
void set_display(treePointer node, int row, int col) {
    if(!node)
        /* Case of NULL */
        return;
    // 현재 좌표에 data를 넣는다.
    display_binary_tree[row][col] = node->data;
    
    int i, now_row, now_col; // for문에 이용할 variables
    
    if(node->leftChild) {
        /* Case of leftChild of the current node exists */
        // 현재 좌표에서 왼쪽 아래로 number_of_slash(node)만큼 내려가며 '/'를 넣는다.
        for(i = 0, now_row = row, now_col = col ; i < number_of_slash(node) ; i++)
            display_binary_tree[++now_row][--now_col] = '/';
        // leftChild를 기준으로 다시 setting한다. (좌표도 함께 이동)
        set_display(node->leftChild, ++now_row, --now_col);
    }
    
    if(node->rightChild) {
        /* Case of rightChild of the current node exists */
        // 현재 좌표에서 오른쪽 아래로 number_of_slash(node)만큼 내려가며 '\'를 넣는다.
        for(i = 0, now_row = row, now_col = col ; i < number_of_slash(node) ; i++)
            display_binary_tree[++now_row][++now_col] = '\\';
        // rightChild를 기준으로 다시 setting한다. (좌표도 함께 이동)
        set_display(node->rightChild, ++now_row, ++now_col);
    }
} // end function set_display

int main(void) {
    int i, j, idx; // for문에 사용할 variables
    input = (char *)malloc(sizeof(char)*MAX_STRING_LEN);
    int input_len; // 입력된 input의 길이

    /* inorder traversal results를 input에 받고 inorder array에 alphabet만 저장한다. */
    gets(input);
    input_len = strlen(input);
    inorder = (char *)malloc(sizeof(char)*MAX_TREE_SIZE);
    for(i = 0, idx = 0 ; i < input_len ; i += 3)
        inorder[idx++] = input[i];
    inorder[idx] = '\0';

    /* postorder traversal results를 input에 받고 postorder array에 alphabet만 저장한다. */
    gets(input);
    postorder = (char *)malloc(sizeof(char)*MAX_TREE_SIZE);
    for(i = 0, idx = 0 ; i < input_len ; i += 3)
        postorder[idx++] = input[i];
    postorder[idx] = '\0';

    /* binary_tree를 입력받은 inorder, postorder에 맞게 만든다. */
    tree_size = idx; // tree의 node 개수
    binary_tree = buildTree(0, tree_size-1, 1); 
    
    /* binary_tree를 주어진 형식에 맞게 display한다. */
    int row, col; // display_binary_tree 배열의 row, col 개수
    if(tree_depth == 1)
        row = col = 1;
    else {
        row = 3 * pow2(tree_depth - 2);
        col = 3 * pow2(tree_depth - 1) - 1;
    }
    
    // display_binary_tree를 위에서 계산한 row, col에 맞게 memory allocation한다.
    display_binary_tree = (char **)malloc(sizeof(char *)*row);
    for(i = 0 ; i < row ; i++)
        display_binary_tree[i] = (char *)malloc(sizeof(char)*col);
    
    // display_binary_tree의 모든 element를 ' '로 initialization한다.
    for(i = 0 ; i < row ; i++)
        for(j = 0 ; j < col ; j++)
            display_binary_tree[i][j] = ' ';  

    set_display(binary_tree, 0, row-1); // Make display_binary_tree

    puts("");
    /* Display the result */
    for(i = 0 ; i < row ; i++) {
        for(j = 0 ; j < col ; j++)
            printf("%c", display_binary_tree[i][j]);
        puts("");
    }

    puts("");
    // Frees the allocated memory.
    free(input);
    free(inorder);
    free(postorder);
    free(binary_tree);
    for(i = 0 ; i < row ; i++)
        free(display_binary_tree[i]);
    free(display_binary_tree);
} // end main function