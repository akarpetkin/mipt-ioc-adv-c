#include <math.h>
#include <stdio.h>
#include "functions.h"

// Глобальная переменная для подсчета итераций в root
static int root_iterations = 0;

float f1(float x) {
    return 0.6 * x + 3;
}

float f2(float x) {
    return pow(x - 2, 3) - 1;
}

float f3(float x) {
    return 3 / x;
}

float root(float (*f)(float), float (*g)(float), float a, float b, float eps) {
    root_iterations = 0;
    float fa = f(a) - g(a);
    float fb = f(b) - g(b);
    
    if (fa * fb > 0) {
        fprintf(stderr, "Root may not exist in the given interval [%g, %g]\n", a, b);
        return NAN;
    }

    float c;
    while (b - a > eps) {
        root_iterations++;
        c = (a + b) / 2;
        float fc = f(c) - g(c);
        
        if (fabs(fc) < eps) {
            break;
        }
        
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    return c;
}

float integral(float (*f)(float), float a, float b, float eps) {
    int n = 2;
    float h = (b - a) / n;
    float s_old = 0;
    float s = (f(a) + 4 * f(a + h) + f(b)) * h / 3;
    
    do {
        s_old = s;
        n *= 2;
        h = (b - a) / n;
        s = f(a) + f(b);
        
        for (int i = 1; i < n; i += 2) {
            s += 4 * f(a + i * h);
        }
        for (int i = 2; i < n - 1; i += 2) {
            s += 2 * f(a + i * h);
        }
        
        s *= h / 3;
    } while (fabs(s - s_old) > eps);
    
    return s;
}

int get_root_iterations() {
    return root_iterations;
}