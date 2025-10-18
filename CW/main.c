#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

void print_help() {
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf(" -help Print this help message\n");
    printf(" -intersections Print intersection points abscissas\n");
    printf(" -iterations Print number of iterations for root finding\n");
    printf(" -test Test root and integral functions\n");
}

float test_function1(float x) {
    return x * x - 4;
}

float test_function2(float x) {
    return x;
}

void test_root() {
    printf("Testing root function...\n");
    float root_val = root(test_function1, test_function2, 1, 3, 1e-6);
    printf("Root found at: %f (expected ~2.0)\n", root_val);
    printf("Iterations used: %d\n", get_root_iterations());
}

void test_integral() {
    printf("Testing integral function...\n");
    float integral_val = integral(test_function2, 0, 2, 1e-6);
    printf("Integral value: %f (expected 2.0)\n", integral_val);
}

int main(int argc, char *argv[]) {
    int print_intersections = 0;
    int print_iterations = 0;
    int test_mode = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "-intersections") == 0) {
            print_intersections = 1;
        } else if (strcmp(argv[i], "-iterations") == 0) {
            print_iterations = 1;
        } else if (strcmp(argv[i], "-test") == 0) {
            test_mode = 1;
        }
    }

    if (test_mode) {
        test_root();
        test_integral();
        return 0;
    }

    // Поиск точек пересечения на отрезке 0 - 5
    float eps = 1e-6;
    float x12, x13, x23;
    int iter12, iter13, iter23;

     // f1 и f3
    x13 = root(f1, f3, 0, 5, eps);
    iter13 = get_root_iterations();

     // f2 и f3
    x23 = root(f2, f3, 0, 5, eps);
    iter23 = get_root_iterations();

    // f1 и f2
    x12 = root(f1, f2, 0, 5, eps);
    iter12 = get_root_iterations();

    if (print_intersections) {
        printf("Intersection points:\n");
        printf("f1 & f2: %f\n", x12);
        printf("f1 & f3: %f\n", x13);
        printf("f2 & f3: %f\n", x23);
    }

    if (print_iterations) {
        printf("Iterations count:\n");
        printf("f1 & f2: %d\n", iter12);
        printf("f1 & f3: %d\n", iter13);
        printf("f2 & f3: %d\n", iter23);
    }

    if (!print_intersections && !print_iterations) {
        // Вычисление площади
        float area = integral(f1, x13, x12, eps);
        
        // Вычисляем интегралы между точками пересечения
        area = area - integral(f2, x23, x12, eps);
        area = area - integral(f3, x13, x23, eps);
        
        printf("Area: %f\n", fabs(area));
    }

    return 0;
}