#pragma once

#include "Map.h"
#include "common.h"
#include <vector>

using namespace std;

// 预处理所有泊位到全图路径 超时？弃用？
// bool bfs_get_full_path(int start_row, int start_col, int berth_id, vector<vector<char>> &char_map, Map *map);

// 预处理全图所有点目标泊位
bool bfs_get_full_canto(int start_row, int start_col, int berth_id, vector<vector<char>> &char_map, Map *map);

bool astar_get_two_path(int start_row, int start_col, int target_row, int target_col, 
                        vector<vector<char>> &char_map, list<int> &path, Map *map);