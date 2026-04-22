#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * 问题1：判断数字的大小
 * 规则：小于500000输出个位，否则输出十位
 * 输入：一个整数
 * 输出：对应的位数字
 */
int getDigit(int num) {
    if (num < 500000) {
        return num % 10;  // 个位
    } else {
        return (num / 10) % 10;  // 十位
    }
}

/*
 * 问题2：判断字符串是否能构成31415
 * 规则：判断一个字符串经过换位能不能构成31415
 * 输入：字符串
 * 输出：能构成返回true，否则返回false
 */
bool canForm31415(char* s) {
    // 统计目标字符串31415中各字符的出现次数
    int targetCount[10] = {0};  // 0-9的计数
    targetCount[3] = 1;
    targetCount[1] = 2;
    targetCount[4] = 1;
    targetCount[5] = 1;
    
    // 统计输入字符串中各字符的出现次数
    int inputCount[10] = {0};
    int len = strlen(s);
    for (int i = 0; i < len; ++i) {
        char c = s[i];
        if (isdigit(c)) {
            inputCount[c - '0']++;
        } else {
            return false;  // 包含非数字字符
        }
    }
    
    // 比较两个计数数组
    for (int i = 0; i < 10; ++i) {
        if (inputCount[i] != targetCount[i]) {
            return false;
        }
    }
    
    return true;
}

/*
 * 问题3：二维数组倒置与选择
 * 规则：将二维数组中的某一行倒置后（倒置：32,45,9->23,54,9），选择一行和一列使这一行与这一列中的数相加和最大
 * 输入：二维数组，行数，列数，要倒置的行号
 * 输出：最大和
 */
int reverseNumber(int num) {
    int reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    return reversed;
}

int maxSumAfterReverse(int** matrix, int rows, int cols, int reverseRow) {
    // 倒置指定行
    for (int j = 0; j < cols; ++j) {
        matrix[reverseRow][j] = reverseNumber(matrix[reverseRow][j]);
    }
    
    // 计算每一行的和
    int rowSums[rows];
    for (int i = 0; i < rows; ++i) {
        rowSums[i] = 0;
        for (int j = 0; j < cols; ++j) {
            rowSums[i] += matrix[i][j];
        }
    }
    
    // 计算每一列的和
    int colSums[cols];
    for (int j = 0; j < cols; ++j) {
        colSums[j] = 0;
        for (int i = 0; i < rows; ++i) {
            colSums[j] += matrix[i][j];
        }
    }
    
    // 找到最大的行和列组合
    int maxSum = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int currentSum = rowSums[i] + colSums[j] - matrix[i][j];  // 减去重复计算的交点
            if (currentSum > maxSum) {
                maxSum = currentSum;
            }
        }
    }
    
    return maxSum;
}

/*
 * 问题4：生成所有可能的链表
 * 规则：有x个节点，返回所有可能的链表（eg：{1},{2},{2}，所有可能为{1,2,2}，{2,1,2}，{2,2,1}）
 * 输入：节点值数组，数组长度
 * 输出：打印所有可能的链表
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

// 生成所有排列
void generatePermutations(int* nums, int numsSize, int* used, int* current, int index, ListNode** lists, int* listCount) {
    if (index == numsSize) {
        // 创建链表
        ListNode* head = NULL;
        ListNode* tail = NULL;
        for (int i = 0; i < numsSize; ++i) {
            ListNode* node = createNode(current[i]);
            if (head == NULL) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        }
        lists[*listCount] = head;
        (*listCount)++;
        return;
    }
    
    // 避免重复排列
    int lastUsed = -1;
    for (int i = 0; i < numsSize; ++i) {
        if (!used[i] && nums[i] != lastUsed) {
            used[i] = 1;
            current[index] = nums[i];
            generatePermutations(nums, numsSize, used, current, index + 1, lists, listCount);
            used[i] = 0;
            lastUsed = nums[i];
        }
    }
}

void generateAllLists(int* nums, int numsSize) {
    if (numsSize == 0) {
        return;
    }
    
    // 排序数组，方便去重
    for (int i = 0; i < numsSize - 1; ++i) {
        for (int j = i + 1; j < numsSize; ++j) {
            if (nums[i] > nums[j]) {
                int temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
        }
    }
    
    int* used = (int*)calloc(numsSize, sizeof(int));
    int* current = (int*)malloc(numsSize * sizeof(int));
    ListNode** lists = (ListNode**)malloc(1000 * sizeof(ListNode*));  // 假设最多1000种排列
    int listCount = 0;
    
    generatePermutations(nums, numsSize, used, current, 0, lists, &listCount);
    
    // 打印所有链表
    for (int i = 0; i < listCount; ++i) {
        printList(lists[i]);
        freeList(lists[i]);
    }
    
    free(used);
    free(current);
    free(lists);
}

/*
 * 问题5：新增边保持最短路径不变
 * 规则：新增一条边，使无向图的两个节点间最短路径长度不变，有多少种可能
 * 输入：图的邻接矩阵，节点数，起点，终点
 * 输出：可能的边数
 */
#define INF 999999

// 计算最短路径（Dijkstra算法）
void dijkstra(int** graph, int n, int start, int* dist) {
    bool visited[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[start] = 0;
    
    for (int count = 0; count < n - 1; ++count) {
        // 找到当前距离最小的节点
        int minDist = INF;
        int u = -1;
        for (int i = 0; i < n; ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        //  relax操作
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

int countPossibleEdges(int** graph, int n, int start, int end) {
    // 计算原始最短路径
    int distStart[n];
    int distEnd[n];
    dijkstra(graph, n, start, distStart);
    dijkstra(graph, n, end, distEnd);
    
    int originalShortestPath = distStart[end];
    int count = 0;
    
    // 遍历所有可能的边 (u, v)
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {  // 无向图，只考虑u < v
            // 如果边已经存在，跳过
            if (graph[u][v] != INF) {
                continue;
            }
            
            // 检查添加边u-v后，最短路径是否不变
            // 新的可能路径：start->u->v->end 或 start->v->u->end
            int newPath1 = distStart[u] + 1 + distEnd[v];
            int newPath2 = distStart[v] + 1 + distEnd[u];
            int newPath = (newPath1 < newPath2) ? newPath1 : newPath2;
            
            if (newPath >= originalShortestPath) {
                count++;
            }
        }
    }
    
    return count;
}

/*
 * 主函数：测试上述五个函数
 */
int main() {
    printf("===== 问题1测试 =====\n");
    printf("数字 12345: %d\n", getDigit(12345));
    printf("数字 678900: %d\n", getDigit(678900));
    
    printf("\n===== 问题2测试 =====\n");
    printf("字符串 \"31415\": %s\n", canForm31415("31415") ? "true" : "false");
    printf("字符串 \"13415\": %s\n", canForm31415("13415") ? "true" : "false");
    printf("字符串 \"314156\": %s\n", canForm31415("314156") ? "true" : "false");
    
    printf("\n===== 问题3测试 =====\n");
    // 创建一个3x3的二维数组
    int matrix[3][3] = {
        {32, 45, 9},
        {12, 34, 56},
        {78, 90, 12}
    };
    // 转换为二维指针
    int* matrixPtr[3];
    for (int i = 0; i < 3; ++i) {
        matrixPtr[i] = matrix[i];
    }
    printf("最大和: %d\n", maxSumAfterReverse(matrixPtr, 3, 3, 0));
    
    printf("\n===== 问题4测试 =====\n");
    int nodes[] = {1, 2, 2};
    generateAllLists(nodes, 3);
    
    printf("\n===== 问题5测试 =====\n");
    // 创建一个4节点的图
    int n = 4;
    int graph[4][4] = {
        {0, 1, INF, 4},
        {1, 0, 2, INF},
        {INF, 2, 0, 3},
        {4, INF, 3, 0}
    };
    // 转换为二维指针
    int* graphPtr[4];
    for (int i = 0; i < 4; ++i) {
        graphPtr[i] = graph[i];
    }
    printf("可能的边数: %d\n", countPossibleEdges(graphPtr, 4, 0, 3));
    
    return 0;
}
