// ConsoleApplication5.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;

int n, m, k{};
int arData[54][54]{};
int dx[8] = {0,1,0,-1, 1, 0, -1, 0}; // 오 아래 왼 위  아래 오 위 왼
int dy[8] = {1,0,-1,0, 0, 1, 0, -1}; // 오 아래 왼 위  아래 오 위 왼
bool visited[54][54]{};
bool rotateCheck[10];
int minret = 987654321;

struct FInfo
{
	pair<int, int> StartPos;
	pair<int, int> LastPos;
};
vector<FInfo> vRotate{};

void Rotate(int RotateIdx, int StartDirIdx);
void go(int RotateIdx, int RotateCnt);
int CalcMin();

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n >> m >> k;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> arData[i][j];
		}
	}
	/*cout << "\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << arData[i][j] << " ";
		}
		cout << "\n";
	}
	vRotate.push_back({ {2 - 2, 3 - 2}, {2 + 2, 3 + 2} });
	Rotate(0, 0);
	cout << "\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << arData[i][j] << " ";
		}
		cout << "\n";
	}*/
	vRotate.reserve(10);
	for (int i = 0; i < k; i++)
	{
		int r, c, s;
		cin >> r >> c >> s;
		r--; c--;

		vRotate.push_back({ {r-s, c-s}, {r+s, c+s} });
	}

	for (int i = 0; i < k; i++)
	{
		memset(rotateCheck, 0, sizeof(rotateCheck));
		rotateCheck[i] = true;
		Rotate(i, 0);
		go(i, 1);
		Rotate(i, 4);
		rotateCheck[i] = false;
	}

	cout << minret << "\n";
}

void Rotate(int RotateIdx, int StartDirIdx)
{
	memset(visited, 0, sizeof(visited));
	pair<int, int> StartPos = vRotate[RotateIdx].StartPos;
	pair<int, int> LastPos = vRotate[RotateIdx].LastPos;
	int x = StartPos.first;
	int y = StartPos.second;
	while (StartPos.first < LastPos.first)
	{

		int dirIdx = StartDirIdx;
		int x = StartPos.first;
		int y = StartPos.second;
		int data_ = arData[x][y];
		while (!visited[x][y])
		{
			visited[x][y] = true;
			if (x + dx[dirIdx] < StartPos.first || x + dx[dirIdx] > LastPos.first
				|| y + dy[dirIdx] < StartPos.second || y + dy[dirIdx] > LastPos.second)
				dirIdx++;

			int nx = x + dx[dirIdx];
			int ny = y + dy[dirIdx];
			int nData = arData[nx][ny];

			arData[nx][ny] = data_;
			x = nx; y = ny; data_ = nData;
		}
		StartPos.first++;
		StartPos.second++;
		LastPos.first--;
		LastPos.second--;
	}
}

void go(int RotateIdx, int RotateCnt)
{
	if (RotateCnt == k)
	{
		// Logic
		minret = min(minret, CalcMin());
		return;
	}

	for (int i = 0; i < k; i++)
	{
		if (!rotateCheck[i])
		{
			rotateCheck[i] = true;
			Rotate(i, 0);
			go(i, RotateCnt + 1);
			Rotate(i, 4);
			rotateCheck[i] = false;
		}
	}
}

int CalcMin()
{
	// 전체 배열을 가지고 최소값 찾기
	int ret = 987654321;
	int x = 0;
	while (x != n)
	{
		int tmp{};
		for (int y = 0; y < m; y++)
		{
			tmp += arData[x][y];
		}
		ret = min(ret, tmp);
		x++;
	}
	return ret;
}