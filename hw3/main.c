#include <stdio.h>
#include <string.h>

// 輔助函式：只保留判斷數字，因為這次的 Grammar 只有 num, +, (, )
int is_digit(char c) { return (c >= '0' && c <= '9'); }

// --- Token 種類定義 ---
#define NUM 256
#define PLUS '+'
#define LPAREN '('
#define RPAREN ')'
#define END_OF_FILE EOF

int token;         // 全域變數：儲存當前的 Token
int has_error = 0; // 錯誤標記：因為不能用 stdlib.h 的 exit()，改用標記來中斷

// ==========================================
// 1. Scanner (源自你的 HW1，改為即時回傳)
// ==========================================
void next_token() {
    int c;
    while ((c = getchar()) != EOF) {
        // 略過空白與換行
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;

        // 辨識數字 (對應 Grammar 的 num)
        if (is_digit(c)) {
            while (is_digit(c = getchar())) {
                // 吃掉後續的數字字元
            }
            ungetc(c, stdin); // 將多讀的一個非數字字元退回
            token = NUM;
            return;
        }
        // 辨識符號
        else if (c == '+') { token = PLUS; return; }
        else if (c == '(') { token = LPAREN; return; }
        else if (c == ')') { token = RPAREN; return; }
        else {
            // 遇到不認識的字元
            has_error = 1;
            return;
        }
    }
    token = END_OF_FILE;
}

// 錯誤處理
void error() {
    if (!has_error) {
        printf("ParseError\n");
        has_error = 1; // 標記錯誤，讓遞迴快速退出
    }
}

// 預先宣告 Parser 函式
void parse_E();
void parse_S_prime();

// ==========================================
// 2. Parser (HW3 核心邏輯)
// ==========================================

// S -> E S'
void parse_S() {
    if (has_error) return;
    
    if (token == NUM || token == LPAREN) {
        printf("S -> E S'\n");
        parse_E();
        parse_S_prime();
    } else {
        error();
    }
}

// S' -> + S | epsilon
void parse_S_prime() {
    if (has_error) return;

    if (token == PLUS) {
        printf("S' -> + S\n");
        next_token(); // 吃掉 '+'，讀取下一個 token
        parse_S();
    } 
    else if (token == RPAREN || token == END_OF_FILE) {
        // S' -> epsilon (空字串)，什麼都不用印，直接 return
        return;
    } else {
        error();
    }
}

// E -> num | ( S )
void parse_E() {
    if (has_error) return;

    if (token == NUM) {
        printf("E -> num\n");
        next_token(); // 吃掉 num
    } 
    else if (token == LPAREN) {
        printf("E -> ( S )\n");
        next_token(); // 吃掉 '('
        parse_S();
        
        // 檢查是否有對應的右括號
        if (token != RPAREN) {
            error();
            return;
        }
        next_token(); // 吃掉 ')'
    } else {
        error();
    }
}

// ==========================================
// 3. Main
// ==========================================
int main() {
    // 1. 啟動 Scanner，抓取第一個 Token
    next_token();

    // 2. 呼叫起始規則 S
    parse_S();

    // 3. 檢查最後結果
    if (!has_error && token == END_OF_FILE) {
        // 成功解析到結尾
        printf("Parsing Complete.\n");
    } else if (!has_error) {
        // 如果沒有觸發內部錯誤，但解析完還有剩餘字元 (例如輸入了 "1 + 2 * 3")
        printf("ParseError\n");
    }

    return 0;
}