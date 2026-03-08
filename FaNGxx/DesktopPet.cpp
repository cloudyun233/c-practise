#include <tchar.h>
#include <time.h>
#include <windows.h>

// 宠物的全局变量：坐标、速度、大小
int petX = 100, petY = 100;
int petSpeedX = 0, petSpeedY = 0; // 如果你想让它静止，把速度改成0
int petWidth = 150, petHeight = 150;

// 窗口过程函数，处理各种消息
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_TIMER: {
    // 定时器触发，更新宠物位置
    petX += petSpeedX;
    petY += petSpeedY;

    // 获取屏幕尺寸，做简单的边缘碰撞反弹
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (petX <= 0 || petX + petWidth >= screenWidth) {
      petSpeedX = -petSpeedX; // 碰到左右边缘掉头
    }
    if (petY <= 0 || petY + petHeight >= screenHeight) {
      petSpeedY = -petSpeedY; // 碰到上下边缘掉头
    }

    // 移动窗口到新位置
    SetWindowPos(hwnd, HWND_TOPMOST, petX, petY, petWidth, petHeight,
                 SWP_NOZORDER | SWP_NOACTIVATE);
    // 请求重绘窗口，产生动画效果
    InvalidateRect(hwnd, NULL, TRUE);
    break;
  }
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // 1. 填充背景色为洋红色 (这一刻的颜色会在后面被设为透明)
    HBRUSH bgBrush = CreateSolidBrush(RGB(255, 0, 255));
    FillRect(hdc, &ps.rcPaint, bgBrush);
    DeleteObject(bgBrush);

    // 2. 画宠物的脸 (黄色圆圈)
    HBRUSH faceBrush = CreateSolidBrush(RGB(255, 200, 0));
    SelectObject(hdc, faceBrush);
    // 消除边框线
    HPEN nullPen = (HPEN)GetStockObject(NULL_PEN);
    SelectObject(hdc, nullPen);
    Ellipse(hdc, 20, 20, 130, 130);

    // 3. 画眼睛 (黑色)
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);
    Ellipse(hdc, 45, 50, 65, 75);
    Ellipse(hdc, 85, 50, 105, 75);

    // 4. 画嘴巴 (红色)
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 80, 80));
    SelectObject(hdc, mouthBrush);
    Ellipse(hdc, 55, 90, 95, 110);

    // 清理画刷资源
    DeleteObject(faceBrush);
    DeleteObject(eyeBrush);
    DeleteObject(mouthBrush);

    EndPaint(hwnd, &ps);
    break;
  }
  case WM_LBUTTONDOWN: {
    // 按下左键时，欺骗系统以为点在了标题栏，从而实现拖拽宠物的功能
    SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    break;
  }
  case WM_RBUTTONUP: {
    // 右键点击时退出程序
    MessageBox(NULL, _T("拜拜！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
    DestroyWindow(hwnd);
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

// 使用标准的 main 函数，方便学习环境一键编译
int main() {
  // 隐藏掉黑色的控制台窗口
  HWND hConsole = GetConsoleWindow();
  if (hConsole) {
    ShowWindow(hConsole, SW_HIDE);
  }

  HINSTANCE hInstance = GetModuleHandle(NULL);
  const TCHAR CLASS_NAME[] = _T("DesktopPetClass");

  // 注册窗口类
  WNDCLASS wc = {};
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL; // 不要背景刷，防止闪烁

  RegisterClass(&wc);

  // 创建一个层级窗口，没有标题栏和边框，置顶显示，且不在任务栏显示图标
  HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                             CLASS_NAME, _T("Desktop Pet"),
                             WS_POPUP, // 无边框样式
                             petX, petY, petWidth, petHeight, NULL, NULL,
                             hInstance, NULL);

  if (hwnd == NULL) {
    return 0;
  }

  // 将洋红色 (RGB(255, 0, 255)) 设置为透明色，这样就只显示出画的宠物形状了
  SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  // 设置一个定时器，每隔 50 毫秒触发一次 WM_TIMER 消息以刷新宠物帧
  SetTimer(hwnd, 1, 50, NULL);

  // 消息循环
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}
