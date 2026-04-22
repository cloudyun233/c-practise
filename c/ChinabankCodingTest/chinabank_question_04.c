#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * 问题1：字符串公共字符替换
 * 规则：把另一个字符串中所有字符丢进set，然后判断是否存在，替换公共字符即可
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
 * 规则：把链表节点丢到queue里偷鸡（毕竟操作容器比动指针容易多了），最后最多剩两个节点，连起来返回即可
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

ListNode* modifyList(ListNode* head) {
    // 使用数组模拟队列
    ListNode* queue[1000];
    int front = 0, rear = 0;
    
    // 将所有节点入队
    ListNode* current = head;
    while (current != NULL) {
        queue[rear++] = current;
        current = current->next;
    }
    
    int length = rear - front;
    if (length < 3) {
        return head;  // 链表长度小于3，直接返回
    }
    
    // 保存第二个节点的值
    int secondVal = queue[1]->val;
    
    // 跳过前三个节点
    front += 3;
    
    // 创建新链表
    ListNode* newHead = NULL;
    ListNode* tail = NULL;
    
    // 将剩余节点连接起来
    while (front < rear) {
        ListNode* node = queue[front++];
        if (newHead == NULL) {
            newHead = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    
    // 将第二个节点的值添加到链表末尾
    if (tail != NULL) {
        tail->next = createNode(secondVal);
        tail->next->next = NULL;
    } else {
        // 如果没有剩余节点，直接创建新节点
        newHead = createNode(secondVal);
    }
    
    // 释放前三个节点的内存
    for (int i = 0; i < 3; ++i) {
        free(queue[i]);
    }
    
    return newHead;
}

/*
 * 问题3：字符串重新排序
 * 规则：统计一下字符串中0-9对应的个数，计数以后分正负号讨论，然后从小到大/从大到小输出。特殊处理一下第一位不为0的情况
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
 * 规则：构建一个状态机即可。当遇到不合法状态时直接返回上一个完整的合法长度
 * 输入：字符串
 * 输出：最大合法长度
 */
int maxValidExpressionLength(char* s) {
    int maxLen = 0;
    int n = strlen(s);
    
    for (int i = 0; i < n; ++i) {
        // 状态机：0-初始状态，1-数字状态，2-操作符状态
        int state = 0;
        int len = 0;
        bool valid = true;
        
        for (int j = i; j < n; ++j) {
            char c = s[j];
            
            if (state == 0) {
                if (isdigit(c)) {
                    if (c == '0' && j + 1 < n && isdigit(s[j + 1])) {
                        // 避免"01"这样的情况
                        valid = false;
                        break;
                    }
                    state = 1;
                    len++;
                } else {
                    // 操作符不能作为开头
                    valid = false;
                    break;
                }
            } else if (state == 1) {
                if (isdigit(c)) {
                    len++;
                } else if (c == '+' || c == '-') {
                    state = 2;
                    len++;
                } else {
                    valid = false;
                    break;
                }
            } else if (state == 2) {
                if (isdigit(c)) {
                    if (c == '0' && j + 1 < n && isdigit(s[j + 1])) {
                        // 避免"+01"这样的情况
                        valid = false;
                        break;
                    }
                    state = 1;
                    len++;
                } else {
                    // 操作符不能连续
                    valid = false;
                    break;
                }
            }
        }
        
        // 操作符不能作为结尾
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
