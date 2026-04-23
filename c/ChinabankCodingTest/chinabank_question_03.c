#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>

/*
 * 问题1：字符串公共字符替换
 * 规则：给两个字符串，将字符串一中的公共字符换成"*"后返回字符串一
 * 输入：两个字符串
 * 输出：替换后的字符串
 */
char* replaceCommonChars(char* s1, char* s2) {
    static char result[1000];
    bool common[256] = {false};  // 标记哪些字符是公共的
    
    // 遍历s2，标记所有出现的字符
    int len2 = strlen(s2);
    for (int i = 0; i < len2; ++i) {
        common[(unsigned char)s2[i]] = true;
    }
    
    // 遍历s1，替换公共字符
    int len1 = strlen(s1);
    for (int i = 0; i < len1; ++i) {
        if (common[(unsigned char)s1[i]]) {
            result[i] = '*';
        } else {
            result[i] = s1[i];
        }
    }
    result[len1] = '\0';
    
    return result;
}

/*
 * 问题2：链表操作
 * 规则：给一个链表，将第二个的值放在链表末尾并将链表前三个元素删除，链表个数小于三的时候返回
 * 思路：
 *   1. 检查链表长度，若少于3个节点则直接返回原链表
 *   2. 保存第二个节点的值（secondVal）
 *   3. 将head指向第四个节点，释放前三个节点的内存
 *   4. 遍历新链表找到末尾节点，将保存的secondVal作为新节点接入
 *   5. 返回新的链表头
 * 输入：链表头
 * 输出：操作后的链表头
 */
// 链表节点结构
typedef struct ListNode {
    int val;
    struct ListNode* next;
} ListNode;

// 创建新节点
ListNode* createNode(int val) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 打印链表
void printList(ListNode* head) {
    printf("{");
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next != NULL) {
            printf(",");
        }
        head = head->next;
    }
    printf("}\n");
}

// 释放链表内存
void freeList(ListNode* head) {
    while (head != NULL) {
        ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// 计算链表长度
int getListLength(ListNode* head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

ListNode* modifyList(ListNode* head) {
    int length = getListLength(head);
    if (length < 3) {
        return head;  // 链表长度小于3，直接返回
    }
    
    // 保存第二个节点的值
    int secondVal = head->next->val;
    
    // 删除前三个节点
    ListNode* newHead = head->next->next->next;
    free(head->next->next);
    free(head->next);
    free(head);
    
    // 将第二个节点的值添加到链表末尾
    if (newHead == NULL) {
        // 如果删除后链表为空，直接创建新节点
        return createNode(secondVal);
    } else {
        // 找到链表末尾
        ListNode* tail = newHead;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = createNode(secondVal);
        return newHead;
    }
}

/*
 * 问题3：字符串重新排序
 * 规则：给一个字符串，里面只包含"-"和数字，返回由这个字符串重新排序后的最小整数
 * 输入：字符串
 * 输出：最小整数
 */
char* smallestNumber(char* s) {
    static char result[1000];
    int digitCount[10] = {0};  // 0-9的计数
    int minusCount = 0;
    
    // 统计数字和负号的个数
    int len = strlen(s);
    for (int i = 0; i < len; ++i) {
        char c = s[i];
        if (isdigit(c)) {
            digitCount[c - '0']++;
        } else if (c == '-') {
            minusCount++;
        }
    }
    
    // 构建结果
    int index = 0;
    
    // 如果有负号，结果为负数，数字从大到小排列
    if (minusCount % 2 != 0) {
        result[index++] = '-';
        // 从9到1排列
        for (int i = 9; i >= 1; --i) {
            for (int j = 0; j < digitCount[i]; ++j) {
                result[index++] = '0' + i;
            }
        }
        // 最后添加0
        for (int j = 0; j < digitCount[0]; ++j) {
            result[index++] = '0';
        }
    } else {
        // 没有负号，结果为正数，数字从小到大排列
        // 找到第一个非0的数字作为最高位
        int firstNonZero = -1;
        for (int i = 1; i < 10; ++i) {
            if (digitCount[i] > 0) {
                firstNonZero = i;
                break;
            }
        }
        
        if (firstNonZero != -1) {
            result[index++] = '0' + firstNonZero;
            digitCount[firstNonZero]--;
        }
        
        // 添加所有0
        for (int j = 0; j < digitCount[0]; ++j) {
            result[index++] = '0';
        }
        
        // 添加剩余数字
        for (int i = 1; i < 10; ++i) {
            for (int j = 0; j < digitCount[i]; ++j) {
                result[index++] = '0' + i;
            }
        }
    }
    
    result[index] = '\0';
    
    // 处理特殊情况：全是0
    if (index == 0) {
        strcpy(result, "0");
    }
    
    return result;
}

/*
 * 问题4：合法算式最大长度
 * 规则：给一个字符串，里面只包含"+"，"-"和数字，返回合法算式的最大长度，"01"、"+12""3-"不合法
 * 输入：字符串
 * 输出：最大合法长度
 *
 * 算法思路：枚举所有可能的子串，对每个子串检查是否构成合法算式
 * 什么是合法算式？
 *   1. 必须以数字开头（不能以操作符开头）
 *   2. 数字不能有前导零（如"01"不合法，但单独的"0"合法）
 *   3. 操作符不能连续出现（如"+-"不合法）
 *   4. 不能以操作符结尾（如"3-"不合法）
 *
 * 我们使用"状态机"来跟踪解析进度：
 *   状态0（初始状态）：期待一个数字作为开头
 *   状态1（数字状态）：正在读取一个数字，期待数字或操作符
 *   状态2（操作符状态）：刚读完一个操作符，期待下一个数字
 */
int maxValidExpressionLength(char* s) {
    int maxLen = 0;      // 记录找到的最长合法算式长度
    int n = strlen(s);   // 字符串总长度

    /* 外层循环：从每个位置开始尝试作为算式的起点 */
    for (int i = 0; i < n; ++i) {
        int state = 0;   // 当前状态：0=初始，1=数字，2=操作符
        int len = 0;      // 当前子串的长度
        bool valid = true; // 当前子串是否仍然合法

        /* 内层循环：从起点i向后扩展，尝试构建合法算式 */
        for (int j = i; j < n; ++j) {
            char c = s[j];

            /* 状态0：期待数字作为开头
             * 只有数字才能作为算式的第一个字符 */
            if (state == 0) {
                if (isdigit(c)) {
                    /* 检查前导零：如果当前是'0'且后面还有数字，说明有前导零 */
                    if (c == '0' && j + 1 < n && isdigit(s[j + 1])) {
                        valid = false;  // "01"这种形式不合法
                        break;
                    }
                    state = 1;  // 切换到数字状态
                    len++;      // 长度加1
                } else {
                    valid = false;  // 操作符不能作为开头
                    break;
                }

            /* 状态1：正在读数字，可以继续读数字或遇到操作符结束数字 */
            } else if (state == 1) {
                if (isdigit(c)) {
                    len++;  // 继续是数字，长度加1
                } else if (c == '+' || c == '-') {
                    state = 2;  // 遇到操作符，切换到操作符状态
                    len++;
                } else {
                    valid = false;  // 出现其他字符（如空格）不合法
                    break;
                }

            /* 状态2：刚读完操作符，后面必须跟数字，不能再跟操作符 */
            } else if (state == 2) {
                if (isdigit(c)) {
                    /* 同样要检查前导零："+01"或"-02"不合法 */
                    if (c == '0' && j + 1 < n && isdigit(s[j + 1])) {
                        valid = false;
                        break;
                    }
                    state = 1;  // 读到了数字，切换回数字状态
                    len++;
                } else {
                    valid = false;  // 操作符后面不能跟操作符
                    break;
                }
            }
        }

        /* 循环结束后检查：
         * 1. 整个过程中没有出现invalid情况
         * 2. 最后不能在操作符状态（算式不能以操作符结尾）
         * 只有都满足才算一个合法算式 */
        if (valid && state != 2) {
            if (len > maxLen) {
                maxLen = len;
            }
        }
    }

    return maxLen;
}

/*
 * 主函数：测试上述四个函数
 */
int main() {
    SetConsoleOutputCP(65001);
    printf("===== 问题1测试 =====\n");
    printf("字符串1: \"hello\", 字符串2: \"world\" -> %s\n", replaceCommonChars("hello", "world"));
    printf("字符串1: \"abcde\", 字符串2: \"ace\" -> %s\n", replaceCommonChars("abcde", "ace"));
    
    printf("\n===== 问题2测试 =====\n");
    // 创建一个链表：1->2->3->4->5
    ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    printf("原链表: ");
    printList(head);
    ListNode* modifiedHead = modifyList(head);
    printf("修改后: ");
    printList(modifiedHead);
    freeList(modifiedHead);
    
    printf("\n===== 问题3测试 =====\n");
    printf("字符串 \"123-45-6\": %s\n", smallestNumber("123-45-6"));
    printf("字符串 \"-1-2-3\": %s\n", smallestNumber("-1-2-3"));
    printf("字符串 \"000\": %s\n", smallestNumber("000"));
    
    printf("\n===== 问题4测试 =====\n");
    printf("字符串 \"123+45-6\": %d\n", maxValidExpressionLength("123+45-6"));
    printf("字符串 \"+12-3\": %d\n", maxValidExpressionLength("+12-3"));
    printf("字符串 \"123-\": %d\n", maxValidExpressionLength("123-"));
    printf("字符串 \"1201+3\": %d\n", maxValidExpressionLength("1201+3"));
    
    return 0;
}
