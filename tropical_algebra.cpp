#include <math.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

int INF = 1e9; // infinity symbol
int n, s, t; // number of vertices in the graph, numbers of start and end vertices
vector<int> parent(1000000, -1); // ancestor array to recover the shortest path

vector<vector<int>> operator*(vector<vector<int>> A, vector<vector<int>> B) // matrix multiplication
{
    int n = A.size();
    vector<vector<int>> res(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[i][j] = INF;
            for (int k = 0; k < n; k++)
            {
                if (A[i][k] != INF && B[k][j] != INF)
                {
                    if (A[i][k] + B[k][j] < res[i][j])
                    {
                        res[i][j] = A[i][k] + B[k][j];
                        if (i == s)
                        {
                            parent[j] = k;
                        }
                    }
                }
            }
        }
    }
    return res;
}

vector<vector<int>> my_pow(vector<vector<int>> A, int x) // degree matrix
{
    vector<vector<int>> res = A;
    for (int i = 2; i <= n - 1; i++)
    {
        res = res * A;
    }
    return res;
}

int main()
{
    cin >> n;
    int m; // number of edges
    cin >> m;
    cin >> s >> t;
    int k; // number of locked edges
    cin >> k;

    parent[s] = s;
    vector<vector<int>> Matrix(n, vector<int>(n)); // adjacency matrix
    set<pair<int, int>> blocked; // multiple locked edges

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Matrix[i][j] = INF;
            if (i == j)
            {
                Matrix[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        int is_blocked;
        cin >> is_blocked;
        Matrix[a][b] = w;
        Matrix[b][a] = w;

        if (is_blocked == 1)
        {
            blocked.insert({a, b});
            blocked.insert({b, a});
        }

    }

    int result = 0;

    while (true)
    {
        vector<vector<int>> tmp = my_pow(Matrix, n - 1);
        int v = t;
        vector<int> path; // shortest (s, t)-path

        while (v != parent[v])
        {
            path.push_back(v);
            v = parent[v];
        }

        path.push_back(v);
        reverse(path.begin(), path.end());

        int flag = 0;
        int cnt = 0;
        for (int i = 0; i < path.size() - 1; i++)
        {
            if (blocked.find({path[i], path[i + 1]}) != blocked.end())
            {
                Matrix[path[i]][path[i + 1]] = INF;
                Matrix[path[i + 1]][path[i]] = INF;
                flag = 1;
                cnt = i;
            }
        }
        
        if (flag == 0)
        {
            cout << (double)(result + tmp[s][t]) / tmp[s][t];
            return 0;
        }
        else
        {
            result += 2 * tmp[s][cnt];
            continue;
        }
    }
    return 0;
}