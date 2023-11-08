#include <iostream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

#ifndef CLASSES
#define CLASSES

struct Rectangle {
    tuple<int,int> coords;
    int height;
    int width;
    
public:
    Rectangle(int x, int y, int h, int w) : coords(make_tuple(x, y)), height(h), width(w) {}
    Rectangle(int h, int w) : coords(make_tuple(0, 0)), height(h), width(w) {}
};

struct Polygon {
    vector<tuple<int,int>> vertices;

    int calculateWidth() const {
        return get<0>(max_vert()) - get<0>(min_vert());
    }

public:
    Polygon(vector<tuple<int, int>> verts) : vertices(verts), width(calculateWidth()) {};

    int width;

    tuple<int, int> min_vert(tuple<int, int> vect = make_tuple(1,0)) const {
        return *min_element(vertices.begin(), vertices.end(), [&vect](const tuple<int, int>& vert1, const tuple<int, int>& vert2) {
            return get<0>(vert1)*get<0>(vect) + get<1>(vert1)*get<1>(vect) < get<0>(vert2)*get<0>(vect) + get<1>(vert2)*get<1>(vect);
            });
    }
    tuple<int, int> max_vert(tuple<int, int> vect = make_tuple(1,0)) const {
        return *max_element(vertices.begin(), vertices.end(), [&vect](const tuple<int, int>& vert1, const tuple<int, int>& vert2) {
            return get<0>(vert1)*get<0>(vect) + get<1>(vert1)*get<1>(vect) < get<0>(vert2)*get<0>(vect) + get<1>(vert2)*get<1>(vect);
            });
    }
};

class Parallelogram {
    int base;
    int wside;
    int height;

public:
    Parallelogram(int b, int h, int wside) : base(b), height(h), wside(wside) {};
};

#endif // CLASSES