#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義 Linked List 節點結構 [cite: 6, 20]
typedef struct Node {
    char character;
    int count;
    struct Node* next;
} Node;

// 建立新節點的函式 [cite: 7, 38]
Node* createNode(char c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->character = c;
        newNode->count = 1;
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
    int c; // 使用 int 來接收 fgetc 以正確判斷 EOF

    // 逐一讀取字元 [cite: 5, 20]
    while ((c = fgetc(fp)) != EOF) {
        // 過濾機制：只紀錄 ASCII 可視字元 (32 到 126) 
        // 這樣就不會出現你截圖中的問號或亂碼符號了
        if (c < 32 || c > 126) continue;

        Node *current = head;
        Node *prev = NULL;
        int found = 0;

        // 在 link-list 中找尋是否已有該字元 [cite: 8, 56, 57]
        while (current != NULL) {
            if (current->character == (char)c) {
                current->count++; // 若已存在，個數加一 [cite: 8, 20]
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        // 若是新字元，新增節點進 link-list 裡 [cite: 7, 38]
        if (!found) {
            Node *newNode = createNode((char)c);
            if (head == NULL) {
                head = newNode;
            } else {
                prev->next = newNode;
            }
        }
    }

    // 依照老師範例格式輸出 [cite: 41, 42, 43]
    Node *current = head;
    while (current != NULL) {
        printf("%c: %d\n", current->character, current->count);
        Node *temp = current;
        current = current->next;
        free(temp); // 釋放記憶體
    }

    fclose(fp);
    return 0;
}
