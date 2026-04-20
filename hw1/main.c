#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 定義 Linked List 節點結構 ---
typedef struct Node {
    char lexeme[100];
    char token_name[50];
    struct Node *next;
} Node;

// 將 Token 加入 Linked List 尾端 (比照 HW#0 邏輯)
void add_node(Node **head, Node **tail, char *lexeme, char *token) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL) {
        strcpy(newNode->lexeme, lexeme);
        strcpy(newNode->token_name, token);
        newNode->next = NULL;
        if (*head == NULL) {
            *head = newNode;
            *tail = newNode;
        } else {
            (*tail)->next = newNode;
            *tail = newNode;
        }
    }
}

// 輔助函式：判斷字元類型 (只使用規定的 header)
int is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
int is_digit(char c) { return (c >= '0' && c <= '9'); }

int main() {
    // 💡 比照 HW#0：讀取自己本身的程式碼檔案
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) return 1;

    Node *head = NULL, *tail = NULL;
    int c;

    while ((c = fgetc(fp)) != EOF) {
        // 略過空白、換行
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;

        // 1. 辨識 Identifier (ID_TOKEN) 或 關鍵字
        if (is_alpha(c)) {
            char buf[100] = {0};
            int i = 0;
            buf[i++] = (char)c;
            while (is_alpha(c = fgetc(fp)) || is_digit(c)) {
                if (i < 99) buf[i++] = (char)c;
            }
            ungetc(c, fp); // 將多讀的字元退回
            
            //Token 名稱
            if (strcmp(buf, "int") == 0) add_node(&head, &tail, buf, "TYPE_TOKEN");
            else if (strcmp(buf, "main") == 0) add_node(&head, &tail, buf, "MAIN_TOKEN");
            else if (strcmp(buf, "if") == 0) add_node(&head, &tail, buf, "IF_TOKEN");
            else if (strcmp(buf, "else") == 0) add_node(&head, &tail, buf, "ELSE_TOKEN");
            else if (strcmp(buf, "while") == 0) add_node(&head, &tail, buf, "WHILE_TOKEN");
            else add_node(&head, &tail, buf, "ID_TOKEN");
        } 
        // 2. 辨識數字 (INTLITERAL)
        else if (is_digit(c)) {
            char buf[100] = {0};
            int i = 0;
            buf[i++] = (char)c;
            while (is_digit(c = fgetc(fp))) {
                if (i < 99) buf[i++] = (char)c;
            }
            ungetc(c, fp);
            add_node(&head, &tail, buf, "INTLITERAL");
        }
        // 3. 辨識符號 (Symbols)
        else if (c == '=') {
            if ((c = fgetc(fp)) == '=') add_node(&head, &tail, "==", "EQUAL_TOKEN");
            else { ungetc(c, fp); add_node(&head, &tail, "=", "ASSIGN_TOKEN"); }
        }
        else if (c == '>') {
            if ((c = fgetc(fp)) == '=') add_node(&head, &tail, ">=", "GREATEREQUAL_TOKEN");
            else ungetc(c, fp);
        }
        else if (c == '(') add_node(&head, &tail, "(", "LEFTPAREN_TOKEN");
        else if (c == ')') add_node(&head, &tail, ")", "REFTPAREN_TOKEN"); // 照投影片拼寫
        else if (c == '{') add_node(&head, &tail, "{", "LEFTBRACE_TOKEN");
        else if (c == '}') add_node(&head, &tail, "}", "RIGHTBRACE_TOKEN");
        else if (c == '+') add_node(&head, &tail, "+", "PLUS_TOKEN");
        else if (c == ';') add_node(&head, &tail, ";", "SEMICOLON_TOKEN");
    }
    fclose(fp);

    Node *curr = head;
    while (curr) {
        printf("%s: %s\n", curr->lexeme, curr->token_name);
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    return 0;
}