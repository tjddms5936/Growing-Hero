// ConsoleApplication2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;

string sData{};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	string s = "I1234";
	vector<int> a = { 1,3,5 };
	sort(a.begin(), a.end(), greater<int>());
	a.erase(a.begin());
	/*for (auto n : a)
	{
		cout << n << " ";
	}*/

	string ss = "-1";
	int bb = stoi(ss);
	cout << bb << "\n";
}
