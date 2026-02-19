// 题目内容：
// 请编写程序，从prime.in文件中逐行读取正整数，直到读到数字0为止（0不作为统计数据）。统计其中所有素数的个数，并将结果写入prime.out文件中。
// 注：素数是指大于1的自然数，除了1和它本身，不能被其他自然数整除的数。
//
// 输入格式:
// prime.in文件中每行一个正整数，最后一行是数字0。
//
// 输出格式：
// prime.out文件中仅输出一个整数，即统计得到的素数个数。
//
// 输入样例（prime.in文件内容）：
// 2
// 9
// 13
// 27
// 31
// 0
//
// 输出样例（prime.out文件内容）：
// 3

#include <stdio.h>

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    FILE *in = fopen("prime.in", "r");
    FILE *out = fopen("prime.out", "w");
    int num, count = 0;
    while (fscanf(in, "%d", &num) == 1 && num != 0) {
        if (is_prime(num)) {
            count++;
        }
    }
    fprintf(out, "%d\n", count);
    fclose(in);
    fclose(out);
    return 0;
}
