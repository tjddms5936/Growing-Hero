// ConsoleApplication7.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;

int n, m{};
int arData[100004];
int SegmentTreeMin[400004]{}; // 구간의 최소값을 저장하는 세그먼트 트리

int MakeMinTree(int nodeNum, int start, int end);
int update(int nodeNum, int start, int end, int idx, int changeNum);
void printMinIdx(int nodeNum, int start, int end);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cin >> arData[i];
	}

	int h = (int)ceil(log2(n));
	int tree_size = (1 << (h + 1));
	MakeMinTree(1, 1, n);

	cin >> m;
	while (m > 0)
	{
		int query, i, v;
		cin >> query;
		if (query == 1)
		{
			// 1번 쿼리 호출은 다음과 같이 하면 됨. 
			cin >> i >> v;
			update(1, 1, n, i, v);
		}
		else
		{
			// 2번 쿼리 호출은 리프노드 찾아가는거임. 
			printMinIdx(1, 1, n);
		}
		m--;
	}
}

int MakeMinTree(int nodeNum, int start, int end)
{
	if (start == end)
	{
		// 리프노드 도달. 
		SegmentTreeMin[nodeNum] = arData[start];
		return SegmentTreeMin[nodeNum];
	}

	int mid = (start + end) / 2;

	int leftData = MakeMinTree(nodeNum * 2, start, mid);
	int rightData = MakeMinTree(nodeNum * 2 + 1, mid + 1, end);

	
	SegmentTreeMin[nodeNum] = min(leftData, rightData);

	return SegmentTreeMin[nodeNum];
};

int update(int nodeNum, int start, int end, int idx, int changeNum)
{
	if (idx < start || idx > end)
		return SegmentTreeMin[nodeNum];

	if (start == end)
	{
		// 리프노드 도달. 
		SegmentTreeMin[nodeNum] = changeNum;
		arData[idx] = changeNum;

		return SegmentTreeMin[nodeNum];
	}

	int mid = (start + end) / 2;
	SegmentTreeMin[nodeNum] = min(
		update(nodeNum * 2, start, mid, idx, changeNum),
		update(nodeNum * 2 + 1, mid + 1, end, idx, changeNum));
	return SegmentTreeMin[nodeNum];
};

void printMinIdx(int nodeNum, int start, int end)
{
	if (start == end)
	{
		cout << start << "\n";
		return;
	}

	int mid = (start + end) / 2;
	if (SegmentTreeMin[nodeNum] == SegmentTreeMin[nodeNum * 2])
		printMinIdx(nodeNum * 2, start, mid);
	else
		printMinIdx(nodeNum * 2+1, mid + 1, end);
};