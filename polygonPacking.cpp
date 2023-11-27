#include <vector>
#include <algorithm>
#include <tuple>
#include <numeric>
#include "classes.cpp"
#include "rectanglePacking.cpp"


#ifndef POLYGONPACKING
#define POLYGONPACKING

tuple<vector<Parallelogram>, vector<int>> ordering_parallelograms_by_slope(vector<Parallelogram> paras) {
    vector<int> parallelograms_order(paras.size());
    iota(parallelograms_order.begin(), parallelograms_order.end(), 0);

    sort(parallelograms_order.begin(), parallelograms_order.end(), [&paras](int i, int j) {
        return float(paras[i].wside)/paras[i].height < float(paras[j].wside)/paras[j].height;
    });

    vector<Parallelogram> sorted_paras(paras.size());
    for (size_t i = 0; i < paras.size(); ++i) {
        sorted_paras[i] = paras[parallelograms_order[i]];
    }

    return make_tuple(sorted_paras, parallelograms_order);
}

tuple<vector<Polygon>, vector<Parallelogram>, int, int> polygon_packing(vector<Polygon> polygons, int c=3, string rectangle_strip_packing_algorithm = "ffdh") {
    vector<Parallelogram> paras;
    vector<int> deltas;
    int w_max = 0;

    for (Polygon poly : polygons) {
        auto [para, delta] = poly.computeBoundingPara();
        paras.push_back(para);
        deltas.push_back(delta);
        if (poly.width > w_max) {
            w_max = poly.width;
        }
    }

    // Stripwidth:
    int stripWidth = c*w_max;

    // For each Polygon q=(base,height,wside), define a rectangle r=(base,height)
    vector<Rectangle> rectangles;
    for (Parallelogram para : paras) {
        rectangles.push_back(Rectangle(para.base, para.height));
    }

    // Pack rectangles with FFDH or NFDH algorithm, as specified
    vector<vector<Rectangle>> rectangles_shelves;
    vector<int> heights_shelves;
    vector<pair<int, int>> map_rectangles_shelves;

    if (rectangle_strip_packing_algorithm == "ffdh") {
        tie(rectangles_shelves, heights_shelves, map_rectangles_shelves) = ffdh(rectangles, stripWidth, true);
    }
    else if (rectangle_strip_packing_algorithm == "nfdh") {
        tie(rectangles_shelves, heights_shelves, map_rectangles_shelves) = nfdh(rectangles, stripWidth, true);
    }
    else {
        cout << "Rectangle Shelf Packing Algorithm \"" << rectangle_strip_packing_algorithm << "\" is not supported.\n";
        return {};
    }

    // Compute the shelves with the parallelograms and create a mapping between parallelograms and their shelf number and order within that shelf.
    vector<vector<Parallelogram>> parallelograms_shelves(rectangles_shelves.size());
    for (int i=0; i<rectangles_shelves.size(); i++) {
        parallelograms_shelves[i].resize(rectangles_shelves[i].size());
    }

    for (int i=0; i<polygons.size(); i++) {
        parallelograms_shelves[map_rectangles_shelves[i].first][map_rectangles_shelves[i].second] = paras[i];
    }

    // Order the parallelograms within each shelf by their slope
    vector<vector<Parallelogram>> parallelograms_shelves_ordered(rectangles_shelves.size());
    vector<vector<int>> parallelograms_shelves_ordering_map(rectangles_shelves.size());
    for (int i=0; i<parallelograms_shelves.size(); i++) {
        auto [pso, psom] = ordering_parallelograms_by_slope(paras);
        parallelograms_shelves_ordered[i] = pso;
        parallelograms_shelves_ordering_map[i] = psom;
    }

    vector<tuple<int, int>> map_parallelograms_shelves(paras.size());
    for (int i=0; i<paras.size(); i++) {
        map_parallelograms_shelves[i] = {map_rectangles_shelves[i].first,parallelograms_shelves_ordering_map[map_rectangles_shelves[i].first][map_rectangles_shelves[i].second]};
    }

    //Compute the coordinates of the left lower vertex of each parallelogram in the packing.
    vector<pair<int, int>> coordinates_parallelograms;

    for (size_t i = 0; i < parallelograms_shelves_ordered.size(); ++i) {
        int x_coord = w_max;
        for (int j = 0; j < get<1>(map_parallelograms_shelves[i]); ++j) {
            auto& t = parallelograms_shelves_ordered[get<0>(map_parallelograms_shelves[i])][j];
            x_coord += t.base;
        }

        int y_coord = 0;
        for (int k = 0; k < get<0>(map_parallelograms_shelves[i]); ++k) {
            y_coord += heights_shelves[k];
        }

        coordinates_parallelograms.push_back({x_coord, y_coord});
    }

    //Compute the coordinates of all vertices of each parallelogram in the packing.
    for (int i=0; i<paras.size(); i++) {
        int x_0 = coordinates_parallelograms[i].first;
        int y_0 = coordinates_parallelograms[i].second;
        int base = paras[i].base;
        int height = paras[i].height;
        int wside = paras[i].wside;
        paras[i].coords = make_tuple(x_0,y_0);
        for (int j=0; j<polygons[i].vertices.size(); j++) {
            polygons[i].vertices[j] = make_tuple(x_0 - deltas[i] + get<0>(polygons[i].vertices[j]),y_0 + get<1>(polygons[i].vertices[j]));
        }
    }
    
    //Compute bounds on the maximum width and the height of the packing.
    int width = (c+2)*w_max;
    int height = accumulate(heights_shelves.begin(), heights_shelves.end(), 0);
    
   return make_tuple(polygons, paras, width, height);
}
#endif