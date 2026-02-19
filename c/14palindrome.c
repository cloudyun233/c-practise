// 题目内容：
// 回文字符串是指一个字符串正读和反读完全一致，空格、标点符号均参与判断，区分大小写。请编写程序，读入一行字符串，判断其是否为回文字符串。若是，输出YES；否则输出NO。
//
// 输入格式:
// 一行字符串，长度不超过100，可包含字母、数字、空格、英文标点符号。
//
// 输出格式：
// 一行字符串，YES或NO，无多余输出。
//
// 输入样例1：
// abba
//
// 输出样例1：
// YES
//
// 输入样例2：
// a b c b a
//
// 输出样例2：
// YES
//
// 输入样例3：
// Abcba
//
// 输出样例3：
// NO

#include <stdio.h>
#include <string.h>

int main() {
    char s[101];
    fgets(s, 101, stdin);
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
        len--;
    }
    int flag = 1;
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) {
            flag = 0;
            break;
        }
    }
    if (flag) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    return 0;
}
