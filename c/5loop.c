/*
 * 题目1：素数求和
 * 题目内容：
 * 我们认为2是第一个素数，3是第二个素数，5是第三个素数，依次类推。
 * 现在，给定两个整数n和m，0<n<=m<=200，你的程序要计算第n个素数到第m个素数之间所有的素数的和，包括第n个素数和第m个素数。
 * 
 * 输入格式:
 * 两个整数，第一个表示n，第二个表示m。
 * 
 * 输出格式：
 * 一个整数，表示第n个素数到第m个素数之间所有的素数的和，包括第n个素数和第m个素数。
 * 
 * 输入样例：
 * 2 4
 * 
 * 输出样例：
 * 15
 */

// 代码空间：
#include "stdio.h"
// #include <stddef.h>

int isit(int a){
    if (a == 2) {
        return 1;
    }
    for (int i=2; i<a; i++) {
        if (a%i==0) {
        return 0;
        }
    }
    return 1;
}

// int main(){
//     int n,m,count=0,sum=0;
//     scanf("%d %d",&n,&m);
//     for (int i=2; count<m; i++) {
//         if (isit(i)) {
//             count++;
//             if (count>=n && count<=m) {
//                     sum = sum + i;
//             }
//         }
//     }
//     printf("%d",sum);
// }

// 参考答案：
// #include <math.h>

// int isPrime(int num) {
//     if (num < 2) return 0;
//     if (num == 2) return 1;
//     if (num % 2 == 0) return 0;
//     for (int i = 3; i <= sqrt(num); i += 2) {
//         if (num % i == 0) return 0;
//     }
//     return 1;
// }

// int main_reference() {
//     int n, m;
//     scanf("%d %d", &n, &m);
    
//     int count = 0, sum = 0;
//     for (int i = 2; count <= m; i++) {
//         if (isPrime(i)) {
//             count++;
//             if (count >= n && count <= m) {
//                 sum += i;
//             }
//         }
//     }
    
//     printf("%d\n", sum);
//     return 0;
// }

/*
 * 题目2：整数转汉语拼音
 * 题目内容：
 * 你的程序要读入一个整数，范围是[-100000,100000]。然后，用汉语拼音将这个整数的每一位输出出来。
 * 如输入1234，则输出：yi er san si
 * 注意，每个字的拼音之间有一个空格，但是最后的字后面没有空格。当遇到负数时，在输出的开头加上"fu"，如-2341输出为：fu er san si yi
 * 
 * 输入格式:
 * 一个整数，范围是[-100000,100000]。
 * 
 * 输出格式：
 * 表示这个整数的每一位数字的汉语拼音，每一位数字的拼音之间以空格分隔，末尾没有空格。
 * 
 * 输入样例：
 * -30
 * 
 * 输出样例：
 * fu san ling
 */

// 代码空间：
void pinyin(int i){
    switch (i) {
        case 0: printf("ling"); break;
        case 1: printf("yi"); break;
        case 2: printf("er"); break;
        case 3: printf("san"); break;
        case 4: printf("si"); break;
        case 5: printf("wu"); break;
        case 6: printf("liu"); break;
        case 7: printf("qi"); break;
        case 8: printf("ba"); break;
        case 9: printf("jiu"); break;
    }
}
int main(){
    int num;
    int a =100000;
    scanf("%d",&num);
    if (num < 0) {
        printf("fu ");
        num = -num;
    }
    
    // 跳过前导的零
    while (a > 0 && num / a == 0) {
        a = a / 10;
    }
    
    // 处理所有数字位
    while (a > 0) {
        pinyin(num / a % 10);
        a = a / 10;
        if (a > 0) {
            printf(" ");
        }
    }
    
    // 处理数字0的特殊情况
    if (num == 0) {
        pinyin(0);
    }
return 0;
}
/*
 * 更优解决方案：使用数组反转
 * 优点：
 * 1. 避免使用sprintf和字符串处理
 * 2. 使用基础的数学运算
 * 3. 更适合初学者理解
 */

// void pinyin_optimized(int num) {
//     // 处理负数
//     if (num < 0) {
//         printf("fu ");
//         num = -num;
//     }
    
//     // 特殊情况：数字0
//     if (num == 0) {
//         printf("ling\n");
//         return;
//     }
    
//     // 定义数字对应的拼音
//     char *pinyin_map[] = {
//         "ling", "yi", "er", "san", "si", 
//         "wu", "liu", "qi", "ba", "jiu"
//     };
    
//     // 将数字的各位存入数组（从低位到高位）
//     int digits[6];  // 最多6位数字
//     int count = 0;
//     int temp = num;
    
//     while (temp > 0) {
//         digits[count] = temp % 10;  // 取个位
//         temp = temp / 10;           // 去掉个位
//         count++;
//     }
    
//     // 从高位到低位输出（数组倒序）
//     for (int i = count - 1; i >= 0; i--) {
//         printf("%s", pinyin_map[digits[i]]);
        
//         // 如果不是最后一位，输出空格
//         if (i > 0) {
//             printf(" ");
//         }
//     }
//     printf("\n");
// }

