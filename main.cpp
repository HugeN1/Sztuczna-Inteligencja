#include <iostream>

using namespace std;

float obliczenia(f1, f2, n){
    max_x = f2(0);
    max_y = f1(max_x);
    for(i=1; i<n; i++)
{
        x = f2(i);
        y = f1(x);
        if(y > max_y){
            max_x = x;
            max_y = y;
        }
}
    return max_x, max_y;
}

float f2(i){
        (2*(i%2)-1)*i/2
};

float f1(x){
    20-abs(x);
};

int main() {
      cout<< max_x;
    cout<< max_y;
    return 0;
}