// ConsoleApplication3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "bits/stdc++.h"
using namespace std;
typedef long long ll;

int n;
int arData[104]{};
ll dp[104][24]{};

ll go(int curSum, int cnt);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> arData[i];
	}

	fill(&dp[0][0], &dp[103][24], -1);
	cout << go(arData[0], 0) << "\n";

}

/*
idea
그러면 
dp[몇번째][현재합] = 성공횟수 
*/

ll go(int curSum, int cnt)
{
	if (curSum < 0 || curSum > 20)
		return 0;

	if (cnt == n-2)
	{
		if (curSum == arData[n - 1])
			return 1;
		return 0;
	}

	ll& ret = dp[cnt][curSum];
	if (ret != -1)
		return ret;

	ret = 0;

	ret += go(curSum + arData[cnt+1], cnt + 1);
	ret += go(curSum - arData[cnt+1], cnt + 1);

	return ret;
}
