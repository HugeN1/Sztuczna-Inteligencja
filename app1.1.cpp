#include <iostream>
#include <cstdlib>

using namespace std;

auto obliczenia = [](auto f1,auto f2,int n){
    int max_x = f2(0);
    int max_y = f1(max_y);
    for(int i = 1; i < n; i++)
    {
        int x = f2(i);
        int y = f1(x);
        if(y > max_y){
            max_x = x;
            max_y = y;
        }
    }
    return make_pair(max_x, max_y);
};

auto f2 = [](int i){
    int a = (2*(i%2)-1)*i/2;
    return a;
};

auto f1 = [](int x){
   b = 20-abs(x);
    return b;
};

int main(int argc, char **argv) {
    auto a = obliczenia(f1, f2, 10);
    cout<< a.first << "|" << a.second<<endl;
}

