#include <chrono>
#include <cstdio>
#include <omp.h>
#include <vector>
#include <stdlib.h>

using namespace std;

const int SIZE = 10000000;
vector<int> v(SIZE);
vector<int> u(SIZE);
// Vector para guardar el calculo
vector<int> w(SIZE);

void llenar()
{
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
        v[i] = 1;
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
        u[i] = 2;
}

void llenarv2()
{
#pragma omp parallel for schedule(static, 5000)
    for (int i = 0; i < SIZE; i++)
        v[i] = 1;
#pragma omp parallel for schedule(dynamic, 5000)
    for (int i = 0; i < SIZE; i++)
        u[i] = 2;
}

void calcAbsSubstract()
{
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
        w[i] = abs(u[i] - v[i]);
}

void sumar(int &res)
{
    int sum = 0;
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < SIZE; i++)
        sum += w[i];
    res = sum;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    llenar();
    auto end = chrono::high_resolution_clock::now();
    // Duración en milisegundos
    auto diff = end - start;
    printf("Tiempo llenado v1: %f\n", chrono::duration<double, milli>(diff).count());

    auto start2 = chrono::high_resolution_clock::now();
    llenarv2();
    auto end2 = chrono::high_resolution_clock::now();
    // Duración en milisegundos
    auto diff2 = end2 - start2;
    printf("Tiempo llenado v2: %f\n",
           chrono::duration<double, milli>(diff2).count());

    auto start3 = chrono::high_resolution_clock::now();
    calcAbsSubstract();
    auto end3 = chrono::high_resolution_clock::now();
    // Duración en milisegundos
    auto diff3 = end3 - start3;
    printf("Tiempo calculo: %f\n",
           chrono::duration<double, milli>(diff3).count());

    auto start4 = chrono::high_resolution_clock::now();
    int sum = 0;
    sumar(ref(sum));
    auto end4 = chrono::high_resolution_clock::now();
    // Duración en milisegundos
    auto diff4 = end4 - start4;
    printf("Tiempo suma: %f\n",
           chrono::duration<double, milli>(diff4).count());
    printf("Suma: %d\n", sum);


}