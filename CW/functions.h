#ifndef FUNCTIONS_H
#define FUNCTIONS_H

float f1(float x);
float f2(float x);
float f3(float x);

float root(float (*f)(float), float (*g)(float), float a, float b, float eps);
float integral(float (*f)(float), float a, float b, float eps);

int get_root_iterations();

#endif