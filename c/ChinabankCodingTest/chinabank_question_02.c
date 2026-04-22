#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>

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
 *
 * 解题思路：
 * 1. 本质是求数组的"全排列"（去重）
 * 2. 用递归+回溯：每次选一个没用过的数放到当前位置，递归填下一个位置
 * 3. 先排序，相同数字只选一次，避免重复排列
 * 4. 题目只要求打印，不需要真正创建链表节点，直接用数组打印即可
 */

// 链表节点结构（保留定义，实际打印时不需要用到）
typedef struct ListNode {
    int val;               // 节点存储的值
    struct ListNode* next; // 指向下一个节点的指针
} ListNode;

// 打印链表（直接从数组打印，不实际创建链表）
void printListFromArray(int* nums, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        printf("%d", nums[i]);
        if (i < size - 1) printf(",");  // 最后一个元素后面不加逗号
    }
    printf("}\n");
}

/**
 * 递归生成排列（回溯法）
 * @param nums      原始数组（已排序）
 * @param numsSize  数组长度
 * @param used      标记数组，used[i]=1 表示 nums[i] 已被使用
 * @param current   当前正在填充的排列
 * @param index     当前要填充的位置（第几个数）
 *
 * 执行流程示例：nums = {1,2,2}
 *   index=0: 选1 -> current[0]=1, used[0]=1
 *     index=1: 选2 -> current[1]=2, used[1]=1
 *       index=2: 选2 -> current[2]=2, used[2]=1 -> 打印 {1,2,2}
 *       回溯：used[2]=0
 *     回溯：used[1]=0
 *     index=1: 再选2（第二个2）-> 同理...
 */
void generatePermutations(int* nums, int numsSize, int* used, int* current, int index) {
    // 递归终止条件：所有位置都填好了
    if (index == numsSize) {
        printListFromArray(current, numsSize);  // 打印这个排列
        return;
    }
    
    int lastUsed = -1;  // 记录上一次使用的数字，用于跳过重复数字
    for (int i = 0; i < numsSize; i++) {
        // 两个条件：
        // 1. !used[i]        -> 这个位置的数字还没被用过
        // 2. nums[i] != lastUsed -> 和上一次用的数字不同（去重）
        if (!used[i] && nums[i] != lastUsed) {
            used[i] = 1;                // 标记为已使用
            current[index] = nums[i];   // 填入当前位置
            generatePermutations(nums, numsSize, used, current, index + 1);  // 递归填下一个位置
            used[i] = 0;                // 回溯：撤销标记，让后面的循环还能用这个数字
            lastUsed = nums[i];         // 记录这次用了什么，下次循环跳过相同的
        }
    }
}

/**
 * 入口函数：生成并打印所有可能的链表排列
 * @param nums      节点值数组
 * @param numsSize  数组长度
 */
void generateAllLists(int* nums, int numsSize) {
    if (numsSize == 0) return;  // 空数组直接返回
    
    // 第一步：冒泡排序（升序）
    // 为什么要排序？因为去重逻辑依赖相同数字相邻
    for (int i = 0; i < numsSize - 1; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] > nums[j]) {
                int temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
        }
    }
    
    // 第二步：分配辅助数组
    int* used = (int*)calloc(numsSize, sizeof(int));   // 全部初始化为0，表示都没用过
    int* current = (int*)malloc(numsSize * sizeof(int));  // 存放当前正在生成的排列
    
    // 第三步：开始递归生成排列
    generatePermutations(nums, numsSize, used, current, 0);
    
    // 第四步：释放内存
    free(used);
    free(current);
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
    SetConsoleOutputCP(65001);
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
