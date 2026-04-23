/*
 * 2026.4中国银行软件开发中心（成都）编程测试题
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>

/*
 * 问题1：字符位移
 * 规则：给定一个小写字母和一个数字，将字母位移指定数字位后返回新字母
 * 输入：字符c，位移数字n
 * 输出：位移后的字符
 */
char shiftChar(char c, int n) {
    if (c >= 'a' && c <= 'z') {
        int result = c + n;
        if (result > 'z') {
            return 'z';
        } else if (result < 'a') {
            return 'a';
        }
        return result;
    }
    return c;
}

/*
 * 问题2：数字分裂
 * 规则：给定一个正整数a，每次可以除以2并向上或向下取整
 *      问最多能出现多少个不同的数字
 * 输入：正整数a
 * 输出：不同数字的最大个数
 *
 * 算法：使用集合记录所有出现过的数字
 *       每次分裂后数字递减，最多分裂log2(a)次
 */
int maxDifferentNumbers(int a, int* visited, int* visitedSize) {
    if (a <= 0) return 0;

    for (int i = 0; i < *visitedSize; i++) {
        if (visited[i] == a) return 0;
    }
    visited[(*visitedSize)++] = a;

    int up = (a + 1) / 2;
    int down = a / 2;

    if (down != a) {
        maxDifferentNumbers(down, visited, visitedSize);
    }
    if (up != a) {
        maxDifferentNumbers(up, visited, visitedSize);
    }

    return *visitedSize;
}

int getMaxDifferentNumbers(int a) {
    int visited[10000];
    int visitedSize = 0;
    return maxDifferentNumbers(a, visited, &visitedSize);
}

/*
 * 问题3：倒水问题
 * 规则：有两个杯子容量分别为a升和b升，可以执行以下操作：
 *       1. 装满任意一个杯子
 *       2. 清空任意一个杯子
 *       3. 将一个杯子中的水倒入另一个杯子，直到源杯子空或目标杯子满
 *       问能否测量出恰好c升的水
 * 输入：两个杯子容量a,b，和目标水量c
 * 输出：能否测量出c（true/false）
 *
 * 算法：使用BFS模拟所有可能的倒水状态
 *       状态表示为(杯1水量, 杯2水量)
 *       当任一杯水量恰好等于c时成功
 *       使用扩展欧几里得算法判断可行性：c必须是a和b最大公约数的倍数
 */
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

bool canMeasureWater(int a, int b, int c) {
    if (c == 0) return true;
    if (c > a + b) return false;
    if (c == a || c == b) return true;
    if (c == a + b) return true;

    int g = gcd(a, b);
    if (c % g != 0) return false;

    typedef struct {
        int x;
        int y;
    } State;

    State queue[10000];
    int front = 0, rear = 0;
    bool visited[10000][10000];

    memset(visited, 0, sizeof(visited));

    queue[rear].x = 0;
    queue[rear++].y = 0;
    visited[0][0] = true;

    while (front < rear) {
        State cur = queue[front++];
        int curX = cur.x;
        int curY = cur.y;

        if (curX == c || curY == c || curX + curY == c) {
            return true;
        }

        State next;
        if (!visited[a][curY]) {
            visited[a][curY] = true;
            queue[rear].x = a;
            queue[rear++].y = curY;
        }
        if (!visited[curX][b]) {
            visited[curX][b] = true;
            queue[rear].x = curX;
            queue[rear++].y = b;
        }
        if (!visited[0][curY]) {
            visited[0][curY] = true;
            queue[rear].x = 0;
            queue[rear++].y = curY;
        }
        if (!visited[curX][0]) {
            visited[curX][0] = true;
            queue[rear].x = curX;
            queue[rear++].y = 0;
        }
        int pour1to2 = curX < (b - curY) ? curX : (b - curY);
        next.x = curX - pour1to2;
        next.y = curY + pour1to2;
        if (!visited[next.x][next.y]) {
            visited[next.x][next.y] = true;
            queue[rear++] = next;
        }
        int pour2to1 = curY < (a - curX) ? curY : (a - curX);
        next.x = curX + pour2to1;
        next.y = curY - pour2to1;
        if (!visited[next.x][next.y]) {
            visited[next.x][next.y] = true;
            queue[rear++] = next;
        }
    }

    return false;
}

/*
 * 问题4：字符串最小代价相等
 * 规则：给定两个字符串s1和s2，以及两次操作的代价a和b
 *       操作1：交换s1中任意两个字符，消耗a
 *       操作2：交换s2中任意两个字符，消耗b
 *       问最少消耗多少使两个字符串相等
 * 输入：字符串s1, s2，代价a, b
 * 输出：最小消耗，无法相等返回-1
 *
 * 算法：贪心 + 计数排序
 *       1. 首先检查两个字符串的字符集合是否相同
 *       2. 对于每种字符，统计在两个字符串中的出现次数差
 *       3. 计算需要通过交换来平衡的字符对数
 *       4. 每次交换可以解决一个字符的差值
 */
int minCostToEqual(char* s1, char* s2, int cost1, int cost2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (len1 != len2) return -1;

    int count1[256] = {0};
    int count2[256] = {0};

    for (int i = 0; i < len1; i++) {
        count1[(unsigned char)s1[i]]++;
        count2[(unsigned char)s2[i]]++;
    }

    for (int i = 0; i < 256; i++) {
        if ((count1[i] + count2[i]) % 2 != 0) {
            return -1;
        }
    }

    int diff[256];
    int diffSize = 0;
    for (int i = 0; i < 256; i++) {
        if (count1[i] > count2[i]) {
            diff[diffSize++] = i;
        }
    }

    int pairs = diffSize / 2;
    int costA = pairs * cost1;
    int costB = pairs * cost2;

    return costA < costB ? costA : costB;
}

/*
 * 问题5：red子串构成
 * 规则：给定多个只包含字符'r'、'e'、'd'的字符串
 *       问这些字符串中的字符可以构成多少个不连续的"red"子串
 * 输入：字符串数组strs，字符串个数n
 * 输出：最多能构成的"red"子串个数
 *
 * 算法：统计r、e、d的总字符数，每个red需要各一个
 *       但子串不能连续，所以答案是min(总r数, 总e数, 总d数)
 */
int canFormRedSubstrings(char** strs, int n) {
    int totalR = 0, totalE = 0, totalD = 0;

    for (int i = 0; i < n; i++) {
        char* s = strs[i];
        int len = strlen(s);
        for (int j = 0; j < len; j++) {
            if (s[j] == 'r') totalR++;
            else if (s[j] == 'e') totalE++;
            else if (s[j] == 'd') totalD++;
        }
    }

    int min1 = totalR < totalE ? totalR : totalE;
    return min1 < totalD ? min1 : totalD;
}

/*
 * 主函数：测试上述五个函数
 */
int main() {
    SetConsoleOutputCP(65001);

    printf("===== 问题1测试 =====\n");
    printf("'a' + 3 = %c\n", shiftChar('a', 3));
    printf("'x' + 27 = %c\n", shiftChar('x', 27));

    printf("\n===== 问题2测试 =====\n");
    printf("数字 14 能出现的不同数字个数: %d\n", getMaxDifferentNumbers(14));
    printf("数字 15 能出现的不同数字个数: %d\n", getMaxDifferentNumbers(15));
    printf("数字 8 能出现的不同数字个数: %d\n", getMaxDifferentNumbers(8));
    printf("数字 1 能出现的不同数字个数: %d\n", getMaxDifferentNumbers(1));

    printf("\n===== 问题3测试 =====\n");
    printf("杯子容量 3,7，目标 4: %s\n", canMeasureWater(3, 7, 4) ? "可以" : "不可以");
    printf("杯子容量 2,6，目标 1: %s\n", canMeasureWater(2, 6, 1) ? "可以" : "不可以");
    printf("杯子容量 5,10，目标 8: %s\n", canMeasureWater(5, 10, 8) ? "可以" : "不可以");

    printf("\n===== 问题4测试 =====\n");
    printf("s1=\"abc\", s2=\"bac\", a=1, b=2: %d\n", minCostToEqual("abc", "bac", 1, 2));
    printf("s1=\"abc\", s2=\"def\", a=3, b=3: %d\n", minCostToEqual("abc", "def", 3, 3));
    printf("s1=\"aab\", s2=\"aba\", a=5, b=3: %d\n", minCostToEqual("aab", "aba", 5, 3));

    printf("\n===== 问题5测试 =====\n");
    char* strs1[] = {"red", "der", "abc"};
    char* strs2[] = {"r", "e", "d", "r", "e", "d"};
    char* strs3[] = {"rrr", "eee", "ddd"};
    printf("strs={\"red\",\"der\",\"abc\"}: %d\n", canFormRedSubstrings(strs1, 3));
    printf("strs={\"r\",\"e\",\"d\",\"r\",\"e\",\"d\"}: %d\n", canFormRedSubstrings(strs2, 6));
    printf("strs={\"rrr\",\"eee\",\"ddd\"}: %d\n", canFormRedSubstrings(strs3, 3));

    return 0;
}