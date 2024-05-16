#include "bits/stdc++.h"
using namespace std;

int arData[100004]{};
int dp[100004][5][5]{}; // idx 왼발위치 오른발위치
int cnt{};

int go(int idx, int leftPos, int rightPos);
int getPrice(int Pivotpos, int TargetPos);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	int nInput{};
	while (scanf_s("%d", &nInput))
	{
		if (nInput == 0)
			break;
		arData[cnt] = nInput;
		cnt++;
	}

	memset(dp, -1, sizeof(dp));
	
	cout << go(0, 0, 0) << "\n";

	return 0;
}

int go(int idx, int leftPos, int rightPos)
{
	if(idx >= cnt)
		return 0;

	int& ret = dp[idx][leftPos][rightPos];
	if (ret != -1)
		return ret;
	ret = INT_MAX;

	if (leftPos == arData[idx])
	{
		ret = min(ret,go(idx + 1, leftPos, rightPos) + getPrice(leftPos, arData[idx]));
	}
	if (rightPos == arData[idx])
	{
		ret = min(ret, go(idx + 1, leftPos, rightPos) + getPrice(rightPos, arData[idx]));
	}
	if(leftPos != arData[idx] && arData[idx] != rightPos)
	{
		ret = min(ret, go(idx + 1, arData[idx], rightPos) + getPrice(leftPos, arData[idx]));
	}
	if (rightPos != arData[idx] && arData[idx] != leftPos)
	{
		ret = min(ret, go(idx + 1, leftPos, arData[idx]) + getPrice(rightPos, arData[idx]));
	}

	return ret;
}

int getPrice(int Pivotpos, int TargetPos)
{
	if (Pivotpos == 0)
		return 2;
	if (Pivotpos == TargetPos)
		return 1;
	if ((Pivotpos == 1 && TargetPos == 3) ||
		(Pivotpos == 3 && TargetPos == 1) ||
		(Pivotpos == 2 && TargetPos == 4) ||
		(Pivotpos == 4 && TargetPos == 2)
		)
		return 4;
	return 3;
}
