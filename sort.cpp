#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;

template<typename iteratorType>
iteratorType myPartition( iteratorType first, iteratorType last )
{
    auto p = first;
    auto current = next(first);
    while (current != last)
    {
        if (*current < *first)
        swap(*++p, *current);
        ++current;
    }

    swap(*first, *p);

    return p;

}

template<typename iteratorType>
void myQuickSort(iteratorType first, iteratorType last)
{
    if(distance(first, last) >= 2){
        auto p = myPartition(first, last);
        myQuickSort(first,p);
                //cout<<"std blad 1"<<endl;
        myQuickSort(p+1,last);
                //cout<<"std blad 2"<<endl;
    }
}

int main()
{
    vector<string> quick, se, defSort;
    string str;
    fstream inputFile, outputFile;
    inputFile.open( "pan-tadeusz.txt" );
    outputFile.open( "out.txt" );

    if(inputFile.is_open() && outputFile.is_open()){
        while(inputFile>>str){
            defSort.push_back(str);
            quick.push_back(str);
            se.push_back(str);

        }



    auto t1 = std::chrono::high_resolution_clock::now();
    seSort(se.begin(), se.end());
    auto t2 = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    
    t1 = std::chrono::high_resolution_clock::now();
    myQuickSort(quick.begin(), quick.end());
    t2 = std::chrono::high_resolution_clock::now();
    dt = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    outputFile<<"QUICKSORT:\n";
    for(auto i:quick){

        outputFile<<i<<"\n";
    }
    outputFile<<"QuTIME: "<<dt.count()<<"\n";

auto     t1 = std::chrono::high_resolution_clock::now();
    sort(defSort.begin(), defSort.end());
   auto t2 = std::chrono::high_resolution_clock::now();
   auto dt = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    outputFile<<"DEFAULT SORT:\n";
    for(auto i:defSort){

        outputFile<<i<<"\n";
    }
    outputFile<<"DeTIME: "<<dt.count()<<"\n";

    return 0;
    }
}
main.cpp
Wyświetlanie main.cpp.