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

struct Parallelogram {
    int base;
    int height;
    int wside;
    tuple<int, int> coords;

public:
    Parallelogram(int b, int h, int wside, int x, int y) : base(b), height(h), wside(wside), coords(make_tuple(x,y)) {};
    Parallelogram(int b, int h, int wside) : base(b), height(h), wside(wside), coords(make_tuple(0,0)) {};
};

struct Polygon {
    vector<tuple<int,int>> vertices;
    int width;
    int height;

private:
    int calculateWidth() const {
        return get<0>(max_vert()) - get<0>(min_vert());
    }

    int calculateHeight() const {
        return get<1>(max_vert(make_tuple(0,1))) - get<1>(min_vert(make_tuple(0,1)));
    }

public:
    Polygon(vector<tuple<int, int>> verts) : vertices(verts), width(calculateWidth()), height(calculateHeight()) {};

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

    /*
    void pack_into(Parallelogram para) {
        for (int i=0; i<vertices.size(); i++) {
            vertices[i] = (get<0>(vertices[i])-para.shift_to_poly+get<0>(para.coords), get<1>(vertices[i]) + get<1>(para.coords));
        }
    }
    */

    Parallelogram computeBoundingPara() {
        tuple<int, int> vertexWithMinHeight = min_vert(make_tuple(0,1));
        tuple<int, int> vertexWithMaxHeight = max_vert(make_tuple(0,1));

        tuple<tuple<int, int>, tuple<int, int>> spineVertices = {vertexWithMinHeight, vertexWithMaxHeight};
    
        int wside = get<0>(get<1>(spineVertices)) - get<0>(get<0>(spineVertices));
    
        tuple<int, int> normalToSpine = {get<1>(vertexWithMaxHeight) - get<1>(vertexWithMinHeight), -(get<0>(vertexWithMaxHeight) - get<0>(vertexWithMinHeight))};

        tuple<int, int> leftestWrtSpine = min_vert(normalToSpine);
        tuple<int, int> rightestWrtSpine = max_vert(normalToSpine);
        tuple<int, int> leftLowerPointOfPara = {get<0>(leftestWrtSpine) - wside*get<1>(leftestWrtSpine)/height,0};
        tuple<int, int> rightLowerPointOfPara = {get<0>(rightestWrtSpine) - wside*get<1>(rightestWrtSpine)/height,0};

        int base = get<0>(rightLowerPointOfPara) - get<0>(leftLowerPointOfPara);

        cout << width << " " << base << " " << height << " " << wside << endl;

        if (base < width) {
            Parallelogram para(base, height, wside);
            return para;
        }
        else {
            Parallelogram para(width, height, 0);
            return para;
        }
    }
};

#endif // CLASSES