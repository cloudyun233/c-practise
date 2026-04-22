/*******************************************************************************
 * 题目描述：
 * 
 * 编写一个程序，实现以下功能：
 *   1. 从文件 data.txt 中读取一段文本内容
 *   2. 从键盘输入一个待统计的子串
 *   3. 统计该子串在文件文本中出现的次数（区分大小写）
 *   4. 将统计结果写入文件 result.txt 中
 * 
 * 输入格式：
 *   - 文件 data.txt 中包含一段长度不超过 1000 的字符串（可能包含空格和换行）
 *   - 程序运行时，从键盘输入要查找的子串（长度不超过 100）
 * 
 * 输出格式：
 *   - 将统计结果以"子串出现次数：X"的格式写入 result.txt 文件
 *   - 同时在屏幕上显示统计结果
 * 
 * 示例：
 *   假设 data.txt 内容为：
 *     Hello world, hello everyone. This is a hello test.
 *   
 *   键盘输入：hello
 *   
 *   则程序输出（屏幕显示）：
 *     子串出现次数：2
 *   
 *   同时 result.txt 文件中写入：
 *     子串出现次数：2
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp;
    char text[1001];
    char sub[101];
    int count = 0;
    char *p;

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        return 1;
    }
    fgets(text, sizeof(text), fp);
    fclose(fp);

    scanf("%s", sub);

    p = text;
    while ((p = strstr(p, sub)) != NULL) {
        count++;
        p += strlen(sub);
    }

    fp = fopen("result.txt", "w");
    fprintf(fp, "子串出现次数：%d", count);
    fclose(fp);

    printf("子串出现次数：%d", count);

    return 0;
}
