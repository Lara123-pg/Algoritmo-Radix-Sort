#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono;

// Estrutura para o nó da árvore
struct Node {
    vector<int> arr;
    Node* nxt[10];
};

Node* new_node() {
    Node* tempNode = new Node();
    for (int i = 0; i < 10; i++) {
        tempNode->nxt[i] = nullptr;
    }
    return tempNode;
}

// Função de ordenação MSD
void msd_sort(Node* root, int exp, vector<int>& sorted_arr, long long& operations) {
    if (exp <= 0) return;
    operations++;

    int j;
    for (int i = 0; i < root->arr.size(); i++) {
        j = (root->arr[i] / exp) % 10;
        operations += 3;

        if (root->nxt[j] == nullptr) {
            root->nxt[j] = new_node();
            operations += 2;
        }
        root->nxt[j]->arr.push_back(root->arr[i]);
        operations++;
    }

    for (int i = 0; i < 10; i++) {
        if (root->nxt[i] != nullptr) {
            if (root->nxt[i]->arr.size() > 1) {
                operations++;
                msd_sort(root->nxt[i], exp / 10, sorted_arr, operations);
            } else {
                sorted_arr.push_back(root->nxt[i]->arr[0]);
                operations++;
            }
            operations++;
        }
    }
}

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

// Função para rodar o experimento com um array específico
void runExperiment(int arr[], int n, const string& orderType) {
    Node* root = new_node();
    vector<int> sorted_arr;
    long long operations = 0;

    auto start = high_resolution_clock::now();
    int exp = getMax(arr, n, operations);
    for (int i = 1; exp > 0; exp /= 10, i *= 10) {
        msd_sort(root, exp, sorted_arr, operations);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::duration<double, std::milli>>(stop - start);

    cout << n << ", " << orderType << ", " << fixed << setprecision(4) << duration.count() << " ms, " << operations << " operações\n";

    delete root;
}

void generateAscending(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

void generateDescending(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void generateRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (n + 1);
    }
}

int main() {
    int sizes[] = {1000, 10000, 100000, 1000000};
    cout << "Tamanho, Ordem, Tempo, Operações\n";

    for (int size : sizes) {
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