#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include "classes.cpp"

using namespace std;

#ifndef RECTANGLEPACKING
#define RECTANGLEPACKING

#include <algorithm>
#include <utility>
#include <numeric>

// Sort rectangles in descending order by height
void order_by_height(vector<Rectangle>& rectangles) {
    sort(rectangles.begin(), rectangles.end(), [] (const Rectangle& a, const Rectangle& b) {return a.height > b.height; });
}

// Helper function to find the index of a rectangle
vector<int> sort_indexes(const vector<Rectangle>& v) {
    vector<int> idx(v.size());
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(),
         [&v] (int i1, int i2) { return v[i1].height > v[i2].height; });
    return idx;
}

// NFDH algorithm implementation in C++
tuple<vector<vector<Rectangle>>, vector<int>, vector<pair<int, int>>> nfdh(vector<Rectangle> rectangles, int stripwidth = 100, bool return_map = false) {
    cout << "Gets executed" << endl;
    // Sort rectangles by height in descending order
    vector<Rectangle> rectangles_sorted = rectangles;
    order_by_height(rectangles_sorted);

    vector<pair<int, int>> rectangles_map;
    if (return_map) {
        vector<int> rectangles_height_order = sort_indexes(rectangles);
        for (size_t i = 0; i < rectangles.size(); ++i) {
            rectangles_map.emplace_back(make_pair(rectangles_height_order[i], i));
        }
        sort(rectangles_map.begin(), rectangles_map.end(),
                  [](const pair<int, int>& a, const pair<int, int>& b) {
                      return a.first < b.first;
                  });
    }

    vector<vector<Rectangle>> S = {{rectangles_sorted[0]}};
    vector<int> w_S = {rectangles_sorted[0].width};
    vector<int> h_S = {rectangles_sorted[0].height};
    vector<pair<int, int>> rectangles_map_for_sorted = {{0, 0}};

    for (size_t i = 1; i < rectangles_sorted.size(); ++i) {
        // If the rectangle fits in the current shelf, add it
        if (w_S.back() + rectangles_sorted[i].width <= stripwidth) {
            S.back().push_back(rectangles_sorted[i]);
            w_S.back() += rectangles_sorted[i].width;
        } else {
            S.push_back({rectangles_sorted[i]});
            w_S.push_back(rectangles_sorted[i].width);
            h_S.push_back(rectangles_sorted[i].height);
        }
        if (return_map) {
            rectangles_map_for_sorted.emplace_back(S.size() - 1, S.back().size() - 1);
        }
    }

    if (return_map) {
        vector<pair<int, int>> rectangles_map_sorted(rectangles.size());
        for (size_t i = 0; i < rectangles_map.size(); ++i) {
            rectangles_map_sorted[i] = rectangles_map_for_sorted[rectangles_map[i].second];
        }
        return {S, h_S, rectangles_map_sorted};
    }

    return {S, h_S, {}};
}

// FFDH algorithm implementation in C++
tuple<vector<vector<Rectangle>>, vector<int>, vector<pair<int, int>>> ffdh(vector<Rectangle> rectangles, int stripwidth = 100, bool return_map = false) {
    // Sort rectangles by height in descending order
    vector<Rectangle> rectangles_sorted = rectangles;
    order_by_height(rectangles_sorted);

    vector<pair<int, int>> rectangles_map;
    if (return_map) {
        vector<int> rectangles_height_order = sort_indexes(rectangles);
        for (size_t i = 0; i < rectangles.size(); ++i) {
            rectangles_map.emplace_back(make_pair(rectangles_height_order[i], i));
        }
        sort(rectangles_map.begin(), rectangles_map.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.first < b.first;
             });
    }

    vector<vector<Rectangle>> S = {{rectangles_sorted[0]}};
    vector<int> w_S = {rectangles_sorted[0].width};
    vector<int> h_S = {rectangles_sorted[0].height};
    vector<pair<int, int>> rectangles_map_for_sorted = {{0, 0}};

    for (size_t i = 1; i < rectangles_sorted.size(); ++i) {
        bool fit_in = false;
        // Iterate over all opened shelves
        for (size_t j = 0; j < S.size(); ++j) {
            // If the rectangle fits in the currently considered shelf, add it
            if (w_S[j] + rectangles_sorted[i].width <= stripwidth) {
                S[j].push_back(rectangles_sorted[i]);
                w_S[j] += rectangles_sorted[i].width;
                fit_in = true;
                // Update rectangle map for sorted list
                if (return_map) {
                    rectangles_map_for_sorted.emplace_back(j, S[j].size() - 1);
                }
                break;
            }
        }
        if (!fit_in) {
            // If the rectangle fits into none of the previously opened shelves, start a new one
            S.push_back({rectangles_sorted[i]});
            w_S.push_back(rectangles_sorted[i].width);
            h_S.push_back(rectangles_sorted[i].height);
            // Update rectangle map for sorted list
            if (return_map) {
                rectangles_map_for_sorted.emplace_back(S.size() - 1, 0);
            }
        }
    }

    if (return_map) {
        vector<pair<int, int>> rectangles_map_sorted(rectangles.size());
        for (size_t i = 0; i < rectangles_map.size(); ++i) {
            rectangles_map_sorted[i] = rectangles_map_for_sorted[rectangles_map[i].second];
        }
        return {S, h_S, rectangles_map_sorted};
    }

    return {S, h_S, {}};
}


// Function to calculate the coordinates of the shelf packing
/*
vector<tuple<pair<int, int>, int, int>>
calc_coords_of_shelf_packing(const vector<vector<Rectangle>>& shelves, const vector<int>& shelf_heights) {
    vector<tuple<pair<int, int>, int, int>> rectangles_placed;
    int H = 0;

    for (size_t i = 0; i < shelves.size(); ++i) {
        int W = 0;
        for (size_t j = 0; j < shelves[i].size(); ++j) {
            rectangles_placed.push_back({{W, H}, shelves[i][j].first, shelves[i][j].second});
            W += shelves[i][j].first;
        }
        H += shelf_heights[i];
    }

    return rectangles_placed;
}
*/

#endif // RECTANGLEPACKING