#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>

/*
 * 问题1：密码合规判断
 * 规则：
 *   1. 密码长度至少为 10
 *   2. 只能包含大小写字母和数字
 *   3. 至少出现大写字母、小写字母、数字这3种类型中的2种
 * 输入：密码字符串
 * 输出：符合规则返回 true，否则返回 false
 */
bool isValidPassword(char* password) {
    int len = strlen(password);
    if (len < 10) return false;
    int hasUpper = 0, hasLower = 0, hasDigit = 0;
    for (int i = 0; i < len; ++i) {
        char c = password[i];
        if (isupper(c)) hasUpper = 1;
        else if (islower(c)) hasLower = 1;
        else if (isdigit(c)) hasDigit = 1;
        else return false;   // 包含非法字符
    }
    return (hasUpper + hasLower + hasDigit) >= 2;
}

/*
 * 问题2：从字符串中提取最大密码
 * 规则：
 *   1. 必须是一串连续的数字
 *   2. 如果数字之间有"BAC"字符串，可以消除掉（反复消除直到没有"BAC"）
 *   3. 数字可能很大
 * 输入：原始字符串（只包含数字和大写字母）
 * 输出：符合规则的最大的数字字符串，如果没有数字返回"-1"
 */
char* findMaxPassword(char* s) {
    static char result[1000];
    char temp[1000];
    strcpy(temp, s);

    // 反复删除所有"BAC"子串
    while (1) {
        char* p = strstr(temp, "BAC");
        if (!p) break;
        char* q = p + 3;
        while (*q) *p++ = *q++;
        *p = '\0';
    }

    int len = strlen(temp);
    int best_start = -1, best_len = 0;
    int i = 0;
    while (i < len) {
        if (isdigit(temp[i])) {
            int start = i;
            while (i < len && isdigit(temp[i])) i++;
            int cur_len = i - start;

            if (best_start == -1) {
                best_start = start;
                best_len = cur_len;
            } else {
                // 比较两个数字段的大小（处理前导零）
                int cur_off = start;
                while (cur_off < start + cur_len && temp[cur_off] == '0') cur_off++;
                int cur_val_len = (start + cur_len) - cur_off;

                int best_off = best_start;
                while (best_off < best_start + best_len && temp[best_off] == '0') best_off++;
                int best_val_len = (best_start + best_len) - best_off;

                if (cur_val_len > best_val_len) {
                    best_start = start;
                    best_len = cur_len;
                } else if (cur_val_len == best_val_len && cur_val_len > 0) {
                    if (strncmp(temp + cur_off, temp + best_off, cur_val_len) > 0) {
                        best_start = start;
                        best_len = cur_len;
                    }
                } else if (cur_val_len == 0 && best_val_len == 0) {
                    // 都是0，保留原来的（数值相等）
                }
            }
        } else {
            i++;
        }
    }

    if (best_start == -1) {
        strcpy(result, "-1");
    } else {
        strncpy(result, temp + best_start, best_len);
        result[best_len] = '\0';
    }
    return result;
}

/*
 * 问题3：十进制数字转IP地址
 * 规则：
 *   1. 先把十进制数字转成十六进制
 *   2. 每2位十六进制为一段，转成十进制，加上点号，组成IP地址
 * 输入：十进制整数（可能很大）
 * 输出：对应的IP地址字符串，如果非法（超出0~2^32-1范围）则输出"invalid"
 */
char* ipAddress(long long num) {
    static char result[20];
    if (num < 0 || num > 0xFFFFFFFFLL) {
        strcpy(result, "invalid");
        return result;
    }
    char hex[9];   // 8位十六进制 + '\0'
    sprintf(hex, "%08llX", (unsigned long long)num);  // 大写，不足8位补零

    int parts[4];
    int ok = 1;
    for (int i = 0; i < 4; ++i) {
        char seg[3] = {hex[i*2], hex[i*2+1], '\0'};
        int val = (int)strtol(seg, NULL, 16);
        if (val < 0 || val > 255) {
            ok = 0;
            break;
        }
        parts[i] = val;
    }
    if (!ok) {
        strcpy(result, "invalid");
    } else {
        sprintf(result, "%d.%d.%d.%d", parts[0], parts[1], parts[2], parts[3]);
    }
    return result;
}

/*
 * 问题4：求无法购买的商品最小价格（货币使用一次，不找零）
 * 输入：货币面额数组 money，数组长度 n
 * 输出：无法凑出的最小正整数金额
 */
int cmp_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}
int minImpossibleSum(int* money, int n) {
    qsort(money, n, sizeof(int), cmp_int);
    int res = 1;   // 当前能凑出的最大连续金额+1
    for (int i = 0; i < n; ++i) {
        if (money[i] > res) break;
        res += money[i];
    }
    return res;
}

/*
 * 主函数：测试上述四个函数
 */
int main() {
    SetConsoleOutputCP(65001);
    printf("===== 问题1测试 =====\n");
    printf("密码 \"Abc1234567\": %s\n", isValidPassword("Abc1234567") ? "true" : "false");
    printf("密码 \"abc12345\": %s\n", isValidPassword("abc12345") ? "true" : "false");
    printf("密码 \"ABCabc123\": %s\n", isValidPassword("ABCabc123") ? "true" : "false");
    printf("密码 \"1234567890\": %s\n", isValidPassword("1234567890") ? "true" : "false");
    printf("密码 \"Abc@123456\": %s\n", isValidPassword("Abc@123456") ? "true" : "false");

    printf("\n===== 问题2测试 =====\n");
    char* s1 = "123BAC456";
    printf("输入: %s -> 输出: %s\n", s1, findMaxPassword(s1));
    char* s2 = "A1B2C3BAC";
    printf("输入: %s -> 输出: %s\n", s2, findMaxPassword(s2));
    char* s3 = "BAC123BAC456";
    printf("输入: %s -> 输出: %s\n", s3, findMaxPassword(s3));
    char* s4 = "ABACBACD";
    printf("输入: %s -> 输出: %s\n", s4, findMaxPassword(s4));
    char* s5 = "BACA";
    printf("输入: %s -> 输出: %s\n", s5, findMaxPassword(s5));

    printf("\n===== 问题3测试 =====\n");
    printf("输入 0 -> 输出: %s\n", ipAddress(0));
    printf("输入 2130706433 -> 输出: %s\n", ipAddress(2130706433));   // 127.0.0.1
    printf("输入 3232235521 -> 输出: %s\n", ipAddress(3232235521));   // 192.168.0.1
    printf("输入 4294967295 -> 输出: %s\n", ipAddress(4294967295));   // 255.255.255.255
    printf("输入 4294967296 -> 输出: %s\n", ipAddress(4294967296));   // 超出范围
    printf("输入 -1 -> 输出: %s\n", ipAddress(-1));

    printf("\n===== 问题4测试 =====\n");
    int money1[] = {1, 2, 5};
    int n1 = sizeof(money1)/sizeof(money1[0]);
    printf("货币 {1,2,5} -> 最小不可购买价格: %d\n", minImpossibleSum(money1, n1));
    int money2[] = {1, 3, 4};
    int n2 = sizeof(money2)/sizeof(money2[0]);
    printf("货币 {1,3,4} -> 最小不可购买价格: %d\n", minImpossibleSum(money2, n2));
    int money3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(money3)/sizeof(money3[0]);
    printf("货币 {1,2,3,4,5} -> 最小不可购买价格: %d\n", minImpossibleSum(money3, n3));
    int money4[] = {2};
    int n4 = sizeof(money4)/sizeof(money4[0]);
    printf("货币 {2} -> 最小不可购买价格: %d\n", minImpossibleSum(money4, n4));
    int money5[] = {1, 1, 1, 1};
    int n5 = sizeof(money5)/sizeof(money5[0]);
    printf("货币 {1,1,1,1} -> 最小不可购买价格: %d\n", minImpossibleSum(money5, n5));

    return 0;
}