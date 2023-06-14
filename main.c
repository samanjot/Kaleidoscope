#include <stdio.h>

//double simple(double, double);

double putchard(double d) { putchar(d); return 0; }
double printd(double d) {
    printf("\n%lf", d);
    return 0;
}

double squareSum(double);
double search(double *, double, double);
double sort(double *, double);

double fibonacci(double);

double sum();
double sum2();

int main() {
    double x,y;
    double arr[10];
    for(int i = 0; i < 10; ++i)
        arr[i] = i + 1;

    printf("fibonacci(5) = %lf ", fibonacci(5));
    printf("\nsomma dei quadrati da 1 a 10 = %lf", squareSum(10));
    printf("\nposizione di 5 nell'array: %lf", search(arr, 10, 5));

    for (int i = 9; i >= 0; i--)
        arr[i] = i;

    printf("\nsort %lf\n", sort(arr, 10));

    for (int i = 0; i < 10; i++)
         printf("\t %lf", arr[i]);

    
    printf("\n somma dei primi 5 numeri naturali(partendo da 1): %lf", sum());
    printf("\n somma dei numeri pari (fino a 20): %lf\n", sum2());

    return 0;
}
