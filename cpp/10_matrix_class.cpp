/*
================================================================================
                         C++ 编程练习题（按视频顺序进阶）
================================================================================
说明：每道题覆盖 3-5 个连续视频知识点，难度循序渐进。请在代码中
      实现需求，并通过注释标注关键知识点的应用位置。
================================================================================
*/

// ============================================================================
// 【编程题 10】运算符重载与库函数："矩阵类综合练习"
// 覆盖视频：40（explicit关键字）、41（运算符重载）、49-51（库的使用）
// ----------------------------------------------------------------------------
// 【功能需求】
// 1. 实现一个 Matrix 类，模拟二维矩阵，并结合库的使用：
//    - 构造函数接受矩阵行数和列数，使用 explicit 防止隐式转换（如禁止 Matrix m = 3; 这样的写法）。
//    - 使用 std::vector<std::vector<int>> 存储数据。
//    - 重载 operator+ 实现矩阵加法。
//    - 将"打印矩阵"功能封装为独立函数，模拟库的调用。
//    - 代码中避免使用 using namespace std，而是通过 std:: 前缀访问标准库组件。
// 2. 在 main() 中测试矩阵创建、加法与库函数调用。
//
// 【技术限制】
// - 构造函数必须使用 explicit 关键字。
// - 使用 std::vector<std::vector<int>> 存储数据。
// - operator+ 必须返回新的 Matrix 对象。
// - 禁止使用 using namespace std。
// - 模拟库的调用方式（可定义独立的打印函数）。
//
// 【预期效果示例】
// 矩阵 A:
// 1 2 3
// 4 5 6
// 矩阵 B:
// 1 0 0
// 0 1 0
// A + B:
// 2 2 3
// 4 6 6
// ============================================================================

#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    int rows;
    int cols;

public:
    explicit Matrix(int r, int c) : rows(r), cols(c) {
        data.resize(rows, std::vector<int>(cols, 0));
    }

    int GetRows() const { return rows; }
    int GetCols() const { return cols; }

    void Set(int r, int c, int value) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            data[r][c] = value;
        }
    }

    int Get(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            return data[r][c];
        }
        throw std::out_of_range("Matrix index out of range");
    }

    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have same dimensions for addition");
        }

        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
};

void PrintMatrix(const Matrix& m, const std::string& name) {
    std::cout << name << ":" << std::endl;
    for (int i = 0; i < m.GetRows(); ++i) {
        for (int j = 0; j < m.GetCols(); ++j) {
            std::cout << m.Get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    Matrix a(2, 3);
    a.Set(0, 0, 1);
    a.Set(0, 1, 2);
    a.Set(0, 2, 3);
    a.Set(1, 0, 4);
    a.Set(1, 1, 5);
    a.Set(1, 2, 6);

    Matrix b(2, 3);
    b.Set(0, 0, 1);
    b.Set(1, 1, 1);

    PrintMatrix(a, "矩阵 A");
    PrintMatrix(b, "矩阵 B");

    Matrix c = a + b;
    PrintMatrix(c, "A + B");

    return 0;
}
