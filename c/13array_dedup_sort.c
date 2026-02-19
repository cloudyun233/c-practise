// 题目内容：
// 程序读入n个正整数，先删除数组中重复出现的数字（重复数字仅保留第一次出现的那一个），再将去重后的数组按照升序排列，最终输出处理后的数组。
//
// 输入格式:
// 第一行是一个正整数n，表示数字的个数，n&lt;=50；
// 第二行是n个正整数，数字之间用空格分隔，每个数字范围是[0,1000]。
//
// 输出格式：
// 一行数字，为处理后的数组，数字之间用一个空格分隔，行末无多余空格。
//
// 输入样例：
// 7
// 5 2 8 2 5 9 8
//
// 输出样例：
// 2 5 8 9

#include <stdio.h>

int main() {
    int n, i, j, k, temp;
    int arr[50], dedup[50];
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    k = 0;
    for (i = 0; i < n; i++) {
        int flag = 0;
        for (j = 0; j < k; j++) {
            if (arr[i] == dedup[j]) {
                flag = 1;
                break;
            }
        }
        if (!flag) {
            dedup[k++] = arr[i];
        }
    }
    for (i = 0; i < k - 1; i++) {
        for (j = 0; j < k - 1 - i; j++) {
            if (dedup[j] > dedup[j + 1]) {
                temp = dedup[j];
                dedup[j] = dedup[j + 1];
                dedup[j + 1] = temp;
            }
        }
    }
    for (i = 0; i < k; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", dedup[i]);
    }
    printf("\n");
    return 0;
}
