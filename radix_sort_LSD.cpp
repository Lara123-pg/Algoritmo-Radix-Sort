#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <iomanip> 
using namespace std;
using namespace std::chrono;

// Função para encontrar o maior valor no array
int getMax(int arr[], int n, long long& operations) {
    int mx = arr[0];
    operations++;
    for (int i = 1; i < n; i++) {
        operations++;
        if (arr[i] > mx) {
            mx = arr[i];
            operations++;
        }
    }
    return mx;
}

// Função de ordenação countSort
void countSort(int arr[], int n, int exp, long long& operations) {
    int* output = new int[n];
    int i, count[10] = {0};

    for (i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
        operations++;
    }

    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
        operations++;
    }

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        operations += 2;
    }

    for (i = 0; i < n; i++) {
        arr[i] = output[i];
        operations++;
    }

    delete[] output;
}

// Função de ordenação radixsort
void radixsort(int arr[], int n, long long& operations) {
    int m = getMax(arr, n, operations);
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countSort(arr, n, exp, operations);
    }
}

// Função para gerar array ordenado de forma crescente
void generateAscending(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

// Função para gerar array ordenado de forma decrescente
void generateDescending(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

// Função para gerar array com valores aleatórios
void generateRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (n + 1);
    }
}

// Função para rodar o experimento com um array específico
void runExperiment(int arr[], int n, const string& orderType) {
    long long operations = 0;

    auto start = high_resolution_clock::now();
    radixsort(arr, n, operations);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::duration<double, std::milli>>(stop - start);

    cout << n << ", " << orderType << ", " << fixed << setprecision(4) << duration.count() << " ms, " << operations << " operações\n"; 
}

int main() {
    int sizes[] = {1000, 10000, 100000, 1000000};
    cout << "Tamanho, Ordem, Tempo, Operações\n";

    for (int size: sizes) {
        int* arr = new int[size];

        // Ordem crescente
        generateAscending(arr, size);
        runExperiment(arr, size, "crescente");

        // Ordem decrescente
        generateDescending(arr, size);
        runExperiment(arr, size, "decrescente");

        // Ordem aleatória
        generateRandom(arr, size);
        runExperiment(arr, size, "aleatória");

        delete[] arr;
    }

    return 0;
}