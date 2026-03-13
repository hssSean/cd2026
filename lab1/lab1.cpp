#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義 Linked List 節點結構
typedef struct Node {
    char character;
    struct Node* next;
} Node;

// 建立新節點的函式
Node* createNode(char c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->character = c;
        newNode->next = NULL;
    }
    return newNode;
}

int main() {
    // 讀取自己本身的程式碼檔案
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        return 1;
    }

    Node *head = NULL;
    Node *tail = NULL;
    int c;

    // 1. 讀取字元並串進 Link-list
    while ((c = fgetc(fp)) != EOF) {
        Node *newNode = createNode((char)c);
        
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            // 依序加到尾端，保持原始程式碼的順序
            tail->next = newNode; 
            tail = newNode;
        }
    }
    fclose(fp);

    // 2. 從 Link-list 走訪並輸出
    Node *current = head;
    while (current != NULL) {
        // 針對特殊字元進行格式化輸出，避免破壞單引號格式
        if (current->character == '\n') {
            printf("'\\n'");
        } else if (current->character == '\r') {
            printf("'\\r'");
        } else if (current->character == '\t') {
            printf("'\\t'");
        } else if (current->character == ' ') {
            printf("' '");
        } else {
            // 一般字元正常印出
            printf("'%c'", current->character);
        }

        // 加上逗號分隔
        if (current->next != NULL) {
            printf(", ");
        }

        // 釋放記憶體並往下一個節點前進
        Node *temp = current;
        current = current->next;
        free(temp); 
    }
    
    printf("\n");
    return 0;
}