#pragma once

#include "Map.h"
#include <vector>

using namespace std;

bool bfs_get_full_path(int start_row, int start_col, int berth_id, vector<vector<char>> &char_map, Map *map);

bool astar_get_two_path(int start_row, int start_col, int target_row, int target_col, 
                        vector<vector<char>> &char_map, list<int> &path, Map *map);