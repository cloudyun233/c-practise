/*
================================================================================
                         C++ 编程练习题（按视频顺序进阶）
================================================================================
说明：每道题覆盖 3-5 个连续视频知识点，难度循序渐进。请在代码中
      实现需求，并通过注释标注关键知识点的应用位置。
================================================================================
*/

// ============================================================================
// 【编程题 7】深拷贝与运算符重载："动态整数数组类"
// 覆盖视频：39（new关键字）、40（explicit关键字）、41（运算符重载）、45（拷贝构造函数）
// ----------------------------------------------------------------------------
// 【功能需求】
// 1. 实现一个 DynamicArray 类，模拟动态整数数组：
//    - 构造函数使用 new 在堆内存分配数组，接受数组大小作为参数（使用 explicit 防止隐式转换）。
//    - 实现深拷贝的拷贝构造函数和拷贝赋值运算符（重载 operator=）。
//    - 提供 Get(int index) 和 Set(int index, int value) 函数访问数组元素。
//    - 在 main() 中测试对象的拷贝、赋值与生存期。
//
// 【技术限制】
// - 构造函数必须使用 explicit 关键字。
// - 必须实现深拷贝（而非浅拷贝）。
// - 使用原始指针管理内存。
//
// 【预期效果示例】
// 创建数组: size = 5
// arr[0] = 10
// 拷贝构造: size = 5
// 赋值操作: size = 5
// ============================================================================

#include <iostream>
#include <algorithm>

class DynamicArray {
private:
    int* data;
    int size;

public:
    explicit DynamicArray(int n) : size(n) {
        data = new int[size];
        for (int i = 0; i < size; ++i) {
            data[i] = 0;
        }
        std::cout << "创建数组: size = " << size << std::endl;
    }

    DynamicArray(const DynamicArray& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        std::cout << "拷贝构造: size = " << size << std::endl;
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (int i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
            std::cout << "赋值操作: size = " << size << std::endl;
        }
        return *this;
    }

    int Get(int index) const {
        return data[index];
    }

    void Set(int index, int value) {
        data[index] = value;
    }
};

int main() {
    DynamicArray arr1(5);
    arr1.Set(0, 10);
    std::cout << "arr[0] = " << arr1.Get(0) << std::endl;

    DynamicArray arr2 = arr1;
    DynamicArray arr3(3);
    arr3 = arr1;

    return 0;
}
