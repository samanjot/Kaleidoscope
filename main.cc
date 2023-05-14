#include <iostream>

extern "C" {
    double simple(double*, double, double);
}

int main() {
    double x,y;
    double arr[1024];
    for(int i = 0; i < 1024; ++i)
        arr[i] = i;

    std::cout << "Inserisci due numeri interi, x e y\nx: ";
    std::cin >> x;
    std::cout << "y: ";
    std::cin >> y;
    std::cout << "Il valore di simple(x,y) è " << simple(arr, x, y) << std::endl;
}
