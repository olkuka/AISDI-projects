// Aleksandra Kukawka, Patryk Jan Sozański

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

/*void print(const vector<string>& v) {
    for (auto i : v) 
        cout << i << " ";
    cout << "\n";
}*/

template<typename IteratorType>
void bsort(IteratorType first, IteratorType last) {
    for (IteratorType jt = last; jt != first; --jt) {
        for (IteratorType it = first; it < jt - 1; ++it) {
            if (*it > *(it + 1)) {
                iter_swap(it, it + 1);
            }
        }
    }
}

template<typename IteratorType> 
IteratorType partition(IteratorType first, IteratorType last) {
    auto pivot = first;
    auto it = next(first);
    while(it != last) {
        if (*it < *first) {
            pivot++;
            swap(*pivot, *it);
        }
            it++;
    }
    swap(*first, *pivot);

    return pivot;
}

template<typename IteratorType> 
void qsort(IteratorType first, IteratorType last) {
    if(distance(first, last) > 1) {
        auto p = partition(first, last);
        qsort(first, p);
        qsort(p + 1, last);
    }
}

int main() {
    vector<string> v1, v2, v3;
    fstream in, out1, out2, out3;
    in.open("pan-tadeusz.txt", ios::in);
    out1.open("bubble_time.txt", ios::out);
    out2.open("quick_time.txt", ios::out);
    out3.open("stdsort_time.txt", ios::out);
    string input;
    while (in >> input) {
        v1.push_back(input);
        v2.push_back(input);
        v3.push_back(input);
    }
    
    for (int i = 1000; i <= 10000; i+=1000) {

            // BUBBLE SORT
        auto b1 = chrono::high_resolution_clock::now();
        bsort<vector<string>::iterator>(v1.begin(), v1.begin()+i);
        auto b2 = chrono::high_resolution_clock::now();
        auto db = chrono::duration_cast<chrono::microseconds>(b2 - b1);

        cout << "Bubble sort duration: " << db.count() << " milisekund.\n";
        out1 << db.count() << "\n";

             // QUICK SORT
        auto q1 = chrono::high_resolution_clock::now();
        qsort<vector<string>::iterator>(v2.begin(), v2.begin()+i);
        auto q2 = chrono::high_resolution_clock::now();
        auto dq = chrono::duration_cast<chrono::microseconds>(q2 - q1);

        cout << "Quick sort duration: " << dq.count() << " milisekund.\n";
        out2 << dq.count() << "\n";

            // STD::SORT
        auto s1 = chrono::high_resolution_clock::now();
        qsort<vector<string>::iterator>(v3.begin(), v3.begin()+i);
        auto s2 = chrono::high_resolution_clock::now();
        auto ds = chrono::duration_cast<chrono::microseconds>(s2 - s1);

        cout << "Std::sort duration: " << ds.count() << " milisekund.\n";
        out3 << ds.count() << "\n";
    }


    in.close();
    out1.close();
    out2.close();
    out3.close();
    return 0;
}
