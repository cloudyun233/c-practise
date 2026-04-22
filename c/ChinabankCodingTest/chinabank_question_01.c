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

    // 第一步：反复删除所有"BAC"子串，直到没有"BAC"为止
    char* p;
    while ((p = strstr(temp, "BAC")) != NULL) {
        char* q = p + 3;          // q指向"BAC"后面的字符
        while (*q != '\0') {
            *p = *q;               // 向前移动字符
            p++;
            q++;
        }
        *p = '\0';                 // 添加字符串结束符
    }

    // 第二步：找最长且最大的数字串
    int best_start = -1;           // 最佳数字串的起始位置（-1表示还没找到）
    int best_len = 0;              // 最佳数字串的长度

    int i = 0;
    while (temp[i] != '\0') {
        // 如果遇到数字字符，开始提取数字串
        if (isdigit(temp[i])) {
            int start = i;         // 记录数字串起始位置

            // 连续取出所有连续数字
            while (temp[i] != '\0' && isdigit(temp[i])) {
                i++;
            }
            int cur_len = i - start;   // 当前数字串长度

            // 第一次找到数字串，直接记录
            if (best_start == -1) {
                best_start = start;
                best_len = cur_len;
            }
            // 比较当前数字串和之前的最佳数字串
            else {
                // 比较长度，长度大的更大
                if (cur_len > best_len) {
                    best_start = start;
                    best_len = cur_len;
                }
                // 长度相同时，用字符串比较（字典序）
                else if (cur_len == best_len) {
                    if (strncmp(temp + start, temp + best_start, cur_len) > 0) {
                        best_start = start;
                        best_len = cur_len;
                    }
                }
            }
        } else {
            i++;                    // 非数字字符，跳过
        }
    }

    // 第三步：返回结果
    if (best_start == -1) {
        strcpy(result, "-1");      // 没有找到数字，返回"-1"
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
    static char result[20];  // 用static保存返回值，函数返回后仍有效

    // ========== 第一步：检查输入是否合法 ==========
    // IP地址本质是一个32位的二进制数
    // 最小值是0，最大值是2^32-1 = 4294967295（十六进制0xFFFFFFFF）
    // 0xFFFFFFFFLL 是long long类型的最大值（末尾LL表示long long）
    if (num < 0 || num > 0xFFFFFFFFLL) {
        strcpy(result, "invalid");
        return result;
    }

    // ========== 第二步：提取IP地址的4个部分 ==========
    // IP地址格式是 a.b.c.d，每部分占1字节（8位），范围0~255
    // 使用位运算直接从32位数中提取每一段，比字符串转换更直观

    // 把输入转成无符号32位整数，方便位运算
    unsigned int ip = (unsigned int)num;
    
    // 提取第一段（最高8位）：右移24位，再用0xFF掩码取低8位
    int part1 = (ip >> 24) & 0xFF;
    // 提取第二段：右移16位，取低8位
    int part2 = (ip >> 16) & 0xFF;
    // 提取第三段：右移8位，取低8位
    int part3 = (ip >> 8) & 0xFF;
    // 提取第四段（最低8位）：直接取低8位
    int part4 = ip & 0xFF;

    // ========== 第三步：组装IP地址字符串 ==========
    // 格式化输出为 "a.b.c.d" 的形式
    sprintf(result, "%d.%d.%d.%d", part1, part2, part3, part4);
    return result;
}

/*
 * 问题4：求无法购买的商品最小价格（货币使用一次，不找零）
 * 输入：货币面额数组 money，数组长度 n
 * 输出：无法凑出的最小正整数金额
 */

// qsort排序需要的比较函数：升序排列
int cmp_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;  // 返回负数表示a<b，正数表示a>b，0表示相等
}

int minImpossibleSum(int* money, int n) {
    // ========== 第一步：排序 ==========
    // 贪心算法的关键：必须从小到大排序，才能逐步覆盖连续金额
    qsort(money, n, sizeof(int), cmp_int);

    // ========== 第二步：贪心遍历 ==========
    // res 的含义：当前能凑出的「最大连续金额」+ 1
    // 初始值为1，因为我们要找的是「最小正整数」
    int res = 1;

    for (int i = 0; i < n; ++i) {
        // 如果当前面额 > res，说明 res 无法凑出，直接返回
        // 例如：当前能凑出1~4，下一个面额是6，那么5就无法凑出
        if (money[i] > res) {
            break;
        }
        // 否则，把当前面额加入，更新能凑出的最大连续金额
        // 例如：能凑1~4，加上面额5，就能凑1~9
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