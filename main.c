#include <stdio.h>

double simple(double*, double, double);

double putchard(double d) { putchar(d); return 0; }

int main() {
    double x,y;
    double arr[1024];
    for(int i = 0; i < 1024; ++i)
        arr[i] = i;

    printf("Inserisci due numeri interi, x e y\n");
    printf("x: ");
    scanf("%lf", &x);
    printf("y: ");
    scanf("%lf", &y);
    printf("Il valore di simple(x, y) è %lf\n", simple(arr, x, y));
}
