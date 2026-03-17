/*
================================================================================
                         C++ 编程练习题（按视频顺序进阶）
================================================================================
说明：每道题覆盖 3-5 个连续视频知识点，难度循序渐进。请在代码中
      实现需求，并通过注释标注关键知识点的应用位置。
================================================================================
*/

// ============================================================================
// 【编程题 9】继承与多态："图形继承体系"
// 覆盖视频：26（Inheritance）、27（Virtual Functions）、28（Interfaces）、69（类型转换）
// ----------------------------------------------------------------------------
// 【功能需求】
// 1. 设计一个简单的图形类继承体系：
//    - 基类 Shape：包含纯虚函数 virtual double Area() const = 0，以及虚析构函数。
//    - 派生类 Circle（成员变量 radius）和 Rectangle（成员变量 width、height），重写 Area() 计算面积。
// 2. 在 main() 中：
//    - 使用 Shape* 数组存储多个图形对象（展示多态）。
//    - 遍历数组，通过指针调用 Area() 计算总面积。
//    - 使用 dynamic_cast 检查对象是否为 Circle 类型，若是则输出其半径。
//
// 【技术限制】
// - Shape 必须是抽象类（包含纯虚函数）。
// - 必须使用虚析构函数。
// - 使用原始指针管理派生类对象。
// - 通过指针调用虚函数。
// - 使用 dynamic_cast 进行向下转型。
//
// 【预期效果示例】
// Circle Area: 78.5398
// Rectangle Area: 24
// Total Area: 102.5398
// Circle with radius: 5
// ============================================================================

#include <iostream>
#include <vector>

class Shape {
public:
    virtual ~Shape() {}
    virtual double Area() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double Area() const override {
        return 3.14159 * radius * radius;
    }

    double GetRadius() const {
        return radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double Area() const override {
        return width * height;
    }
};

int main() {
    const int numShapes = 2;
    Shape* shapes[numShapes];
    shapes[0] = new Circle(5);
    shapes[1] = new Rectangle(4, 6);

    double totalArea = 0;
    for (int i = 0; i < numShapes; ++i) {
        double area = shapes[i]->Area();
        std::cout << (dynamic_cast<Circle*>(shapes[i]) ? "Circle" : "Rectangle")
                  << " Area: " << area << std::endl;
        totalArea += area;
    }
    std::cout << "Total Area: " << totalArea << std::endl;

    for (int i = 0; i < numShapes; ++i) {
        Circle* circle = dynamic_cast<Circle*>(shapes[i]);
        if (circle != nullptr) {
            std::cout << "Circle with radius: " << circle->GetRadius() << std::endl;
        }
    }

    for (int i = 0; i < numShapes; ++i) {
        delete shapes[i];
    }

    return 0;
}
