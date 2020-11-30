#include <iostream>
#include "../include/functions.hpp"
// #include "../include/test.hpp"
// #include <../include/common.hpp>

using namespace std;

int main(){
    cout << "Make two 1*1000000 vectors which store in vector1 and vector2" << endl;
    makeData();
    cout << endl;
    cout << "run " << TIMES << " times" << endl << endl;
    cout << "Now try to add them normally, the output stored in addNormal_output" << endl;
    addNormal();
    cout << endl;
    cout << "Now try to add them using AVX, the output stored in addUsingAVX_output" << endl;
    addUsingAVX();
    cout << endl;
    cout << "Now try to add them using threads, the output stored in addUsingThreads_output" << endl;
    addUsingThreads();
    cout << endl;
    
    return 0;
}