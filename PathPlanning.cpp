#include "PathPlanning.h"
#include "common.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <vector>



int reverse_dir(int dir) {
    if(dir == 0)
        return 1;
    else if(dir == 1)
        return 0;
    else if(dir == 2)
        return 3;
    else if(dir == 3)
        return 2;
    return -1;
}

// 检查新位置是否有效
bool isValid(int x, int y, int N, vector<vector<char>>& char_map) {
    return x >= 0 && x < N && y >= 0 && y < N && (char_map[x][y] == 'B' || char_map[x][y] == '.');
}

bool bfs_get_full_path(int start_row, int start_col, int berth_id, vector<vector<char>> &char_map, Map *map) {
    vector<vector<bool>> visited(MAP_H, vector<bool>(MAP_W, false));
    vector<vector<list<int>>> path(MAP_H, vector<list<int>>(MAP_W)); // 路径

    queue<pair<int, int>> q;
    q.push({start_row, start_col});
    visited[start_row][start_col] = true;
    map->path_to_berth_id[start_row][start_col] = berth_id;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // 遍历所有可能的移动
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (isValid(nx, ny, MAP_H, char_map) && !visited[nx][ny]) {
                q.push({nx, ny});
                visited[nx][ny] = true;
                path[nx][ny] = path[x][y];
                path[nx][ny].push_front(reverse_dir(i));
                if(map->path_to_berth[nx][ny].size() > path[nx][ny].size() || map->path_to_berth_id[nx][ny] == -1) {
                    map->path_to_berth[nx][ny] = path[nx][ny];
                    map->path_to_berth_id[nx][ny] = berth_id;
                }
            }
        }
    }
    return true;
}


//------------A*----------------
struct Node {
    int x, y;
    int g, h;  // g: 从起点走到当前格子的成本，h: 从当前格子走到终点的预估成本
    Node *parent;  // 父节点指针

    int move = -1; // 到达当前节点指令

    Node(int x, int y, Node *parent = nullptr, int m = -1) : x(x), y(y), g(0), h(0), parent(parent), move(m) {}

    int f() const { return g + h; }  // f = g + h
};

struct NodeCompare {
    bool operator()(const Node* a, const Node* b) const {
        return a->f() > b->f();
    }
};

// 曼哈顿距离作为启发式函数
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 从终点回溯到起点来重建路径
void reconstructPath(Node *current, list<int> &path) {
    while (current && current->move != -1) {
        path.push_front(current->move);
        current = current->parent;
    }
}

bool astar_get_two_path(int start_row, int start_col, int target_row, int target_col, 
                        vector<vector<char>> &char_map, list<int> &path, Map *map) {
    // TODO
    path.clear();

    priority_queue<Node*, vector<Node*>, NodeCompare> openSet;
    vector<vector<bool>> visited(MAP_H, vector<bool>(MAP_W, false));

    Node *startNode = new Node(start_row, start_col);
    startNode->h = heuristic(start_row, start_col, target_row, target_col);
    openSet.push(startNode);
    visited[start_row][start_col] = true;

    while (!openSet.empty()) {
        Node *current = openSet.top();
        openSet.pop();

        if (current->x == target_row && current->y == target_col) {
            reconstructPath(current, path);
            return true;
        }
        for (int i = 0; i < 4; i++) {
            int nx = current->x + dx[i], ny = current->y + dy[i];
            if (isValid(nx, ny, MAP_H, char_map) && !visited[nx][ny]) {
                Node *successor = new Node(nx, ny, current, i);
                successor->g = current->g + 1;
                successor->h = heuristic(nx, ny, target_row, target_col);
                openSet.push(successor);
                visited[nx][ny] = true;
            }
        }
    }

    return false;  // 没有找到路径
}