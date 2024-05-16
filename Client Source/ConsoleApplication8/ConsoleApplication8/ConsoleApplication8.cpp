// ConsoleApplication8.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;
typedef long long ll;

ll n, m, k;
ll arData[1000004]{};
ll a, b, c;
ll SegmentTree[4000004]{};

ll MakeSegmentTree(ll nodeNum, ll start, ll end);
void update(ll nodeNum, ll start, ll end, const ll idx, const ll diff);
ll query(ll nodeNum, ll start, ll end, ll b, ll c);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n >> m >> k;
	for (int i = 0; i < n; i++)
	{
		cin >> arData[i];
	}

	MakeSegmentTree(1, 0, n - 1);

	for (int i = 0; i < m + k; i++)
	{
		cin >> a >> b >> c;
		if (a == 1)
		{
			// arData[b] = c 로 교체
			update(1, 0, n - 1, b-1, c - arData[b-1]);
		}
		else
		{
			// 구간 [b,c] 합 구하기
			cout << query(1, 0, n - 1, b-1, c-1) << "\n";
		}
	}

}

ll MakeSegmentTree(ll nodeNum, ll start, ll end)
{
	if (start == end)
	{
		// 리프노드
		SegmentTree[nodeNum] = arData[start];
		// TODO
		return SegmentTree[nodeNum];
	}

	ll mid = (start + end) / 2;
	ll LeftSum =	MakeSegmentTree(nodeNum * 2,	start, mid);
	ll RightSum =	MakeSegmentTree(nodeNum * 2+1,	mid+1, end);

	SegmentTree[nodeNum] = LeftSum + RightSum;

	return SegmentTree[nodeNum];
}

void update(ll nodeNum, ll start, ll end, const ll idx, const ll diff)
{
	if (idx < start || idx > end)
		return;
	if (start == end)
	{
		SegmentTree[nodeNum] += diff;
		arData[idx] += diff;
		return; 
	}

	SegmentTree[nodeNum] += diff;

	ll mid = (start + end) / 2;
	update(nodeNum * 2,		start,	mid, idx, diff);
	update(nodeNum * 2+1,	mid+1,	end, idx, diff);
}

ll query(ll nodeNum, ll start, ll end, ll b, ll c)
{
	if (c < start || b > end)
		return 0;
	if (start == b && end == c)
	{
		// 원하는 구간 나옴. 여기 저장된 합 return
		return SegmentTree[nodeNum];
	}

	ll mid = (start + end) / 2;

	ll ret{};
	ret += query(nodeNum * 2,		start,	mid,	b,				min(c, mid));
	ret += query(nodeNum * 2+1,		mid+1,	end,	max(b, mid+1),	c);

	return ret;
}
