#include <vector>

vector<Parallelogram>& paras ordering_parallelograms_by_slope(vector<Parallelogram> paras) {
    sort(paras.begin(), paras.end(), [] const Parallelogram& a, const Parallelogram& b {
        return float(a.wside)/a.height < float(b.wside)/b.height;
    })
    return paras;
}