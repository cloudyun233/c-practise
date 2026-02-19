// 题目内容：
// 请编写程序，读入一个非负十进制整数n和进制基数k（2&lt;=k&lt;=16），将十进制数n转换为k进制数并输出。
// 注：10~15分别用大写字母A、B、C、D、E、F表示。
//
// 输入格式:
// 一行两个整数，第一个是十进制数n（0&lt;=n&lt;=10000），第二个是进制基数k，两个数字之间用空格分隔。
//
// 输出格式：
// 一行字符串，为转换后的k进制数，无多余输出。
//
// 输入样例1：
// 10 2
//
// 输出样例1：
// 1010
//
// 输入样例2：
// 27 16
//
// 输出样例2：
// 1B
//
// 输入样例3：
// 0 8
//
// 输出样例3：
// 0

#include &lt;stdio.h&gt;

int main() {
    int n, k, i = 0;
    char result[100];
    char digits[] = "0123456789ABCDEF";
    scanf("%d %d", &amp;n, &amp;k);
    if (n == 0) {
        printf("0\n");
        return 0;
    }
    while (n &gt; 0) {
        result[i++] = digits[n % k];
        n = n / k;
    }
    for (int j = i - 1; j &gt;= 0; j--) {
        printf("%c", result[j]);
    }
    printf("\n");
    return 0;
}
