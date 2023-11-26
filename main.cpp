#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <utility>
#include <numeric>
#include <random>

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 10); // Adjust range as needed


#include "classes.cpp"
#include "rectanglePacking.cpp"
#include "polygonPacking.cpp"

using namespace std;

vector<tuple<int, int>> generateRandomVertices(int numVertices) {
    vector<tuple<int, int>> vertices;
    for (int i = 0; i < numVertices; ++i) {
        int x = dis(gen);
        int y = dis(gen);
        vertices.emplace_back(x, y);
    }
    return vertices;
}

vector<Polygon> generateRandomPolygons(int n) {
    vector<Polygon> polygons;
    for (int i = 0; i < n; ++i) {
        int numVertices = dis(gen) / 2 + 3; // Ensure at least 3 vertices
        polygons.emplace_back(generateRandomVertices(numVertices));
    }
    return polygons;
}

void printPackingResult(const vector<Polygon>& polygons, const vector<Parallelogram>& parallelograms, int width, int height) {
    cout << "Packed Polygons:" << endl;
    for (const auto& polygon : polygons) {
        polygon.print(); // Assuming Polygon class has a print method
    }

    cout << "Packed Parallelograms:" << endl;
    // Assuming Parallelogram class has a method to print its details
    for (const auto& parallelogram : parallelograms) {
        parallelogram.print();
    }

    cout << "Packing Width: " << width << ", Packing Height: " << height << endl;
}


int main() {
    Rectangle myRect(1,2,7,4);
    //cout << myRect.height;

    cout << "main gets started" << endl;

    
    // Create a vector of 10 Rectangle instances with varying heights and widths
    vector<Rectangle> rectangles = {
        Rectangle(100, 22),
        Rectangle(60, 12),
        Rectangle(50, 33),
        Rectangle(30, 31),
        Rectangle(90, 28),
        Rectangle(80, 20),
        Rectangle(33, 29),
        Rectangle(56, 25),
        Rectangle(27, 34),
        Rectangle(10, 15)
    };
    
    int stripWidth = 100;
    bool returnMap = false;
    // Now you can pass this vector to your nfdh function to see how it arranges them.
    auto [nfdh_shelves, nfdh_shelf_heights, nfdh_mapping] = nfdh(rectangles, stripWidth, returnMap);
    auto [ffdh_shelves, ffdh_shelf_heights, ffdh_mapping] = ffdh(rectangles, stripWidth, returnMap);


    // Print the result
    cout << "Shelves (each line represents a shelf and shows rectangles as [width x height]):" << endl;

    for (const auto& shelf : nfdh_shelves) {
        for (const auto& rect : shelf) {
            cout << "[" << rect.width << "x" << rect.height << "] ";
        }
        cout << endl;
    }

    cout << "FFDH:" << endl;

    for (const auto& shelf : ffdh_shelves) {
        for (const auto& rect : shelf) {
            cout << "[" << rect.width << "x" << rect.height << "] ";
        }
        cout << endl;
    }

    /*
    cout << "Heights of each shelf:" << endl;
    for (const auto& height : nfdh_shelf_heights) {
        cout << height << " ";
    }
    cout << endl;

    if (return_map) {
        cout << "Mapping from sorted to original indices:" << endl;
        for (const auto& map_pair : nfdh_mapping) {
            cout << "(" << map_pair.first << "->" << map_pair.second << ") ";
        }
        cout << endl;
    }
    */

    cout << "min and max vertices of poly" << endl;
    Polygon poly({make_tuple(1,1),make_tuple(0,0),make_tuple(2,0)});

    auto minVert = poly.min_vert();
    auto maxVert = poly.max_vert(make_tuple(0,1));
    cout << "(" << get<0>(minVert) << ", " << get<1>(minVert) << ") "
        << "(" << get<0>(maxVert) << ", " << get<1>(maxVert) << ")" << endl;
    cout << poly.width << endl;
    cout << poly.height << endl;

    cout << "bounding para" << endl;
    Parallelogram para = get<0>(poly.computeBoundingPara());

    cout << para.height << ", " << para.base << ", " << para.wside << endl;

    vector<Polygon> polygons = generateRandomPolygons(30);

    auto [packedPolygons, parallelograms, width, height] = polygon_packing(polygons);
    printPackingResult(packedPolygons, parallelograms, width, height);

    // Now you can use packedPolygons, parallelograms, width, and height as needed

    return 0;
}
