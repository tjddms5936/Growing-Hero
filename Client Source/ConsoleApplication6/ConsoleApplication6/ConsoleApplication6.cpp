#include <iostream>
#include <vector>

using namespace std;

// 함수를 정의하여 주어진 n, a, b에 따라 특정 위치의 값을 반환
int getValueAtPosition(int n, int a, int b) 
{
    int center = n;
    int pivot = a + b - 1;

    int startNum{};

    if (pivot == center)
    {

    }
    else if (pivot < center)
    {
        // 윗대각
    }
    else
    {
        // 아랫대각
    }
}

int main() {
    int n, a, b;
    cout << "Enter the values of n, a, b: ";
    cin >> n >> a >> b;

    // 함수를 사용하여 결과를 얻고 출력
    int result = getValueAtPosition(n, a, b);
    cout << result << endl;

    return 0;
}
