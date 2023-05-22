#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int rows = 10000;
const int cols = 10000;

int matrix[rows][cols];

void init_matrix();
long long calculate_sum();
int find_min_row();
long long calculate_sum_row(int row);

int main() {
    srand(static_cast<unsigned int>(time(0)));
    init_matrix();

    double t1 = omp_get_wtime();
    for (int num_threads = 1; num_threads <= 32; num_threads *= 2) {
        omp_set_num_threads(num_threads); // Задання кількості потоків

        cout << "Number of threads: " << num_threads << endl;

#pragma omp parallel sections
        {
#pragma omp section
            {
                long long sum = calculate_sum();
                cout << "Sum of all elements: " << sum << endl;
                cout << endl; // Відступ після розрахунку суми
            }

#pragma omp section
            {
                int min_row = find_min_row();
                cout << "Row with the minimum sum: " << min_row << endl;
                cout << "Minimum sum value: " << calculate_sum_row(min_row) << endl;
                cout << endl; // Відступ після розрахунку мінімальної суми
            }
        }
    }
    double t2 = omp_get_wtime();

    cout << "Total time: " << t2 - t1 << " seconds" << endl;
    return 0;
}

void init_matrix() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100; // Заповнення випадковими числами від 0 до 99
        }
    }
}

long long calculate_sum() {
    long long sum = 0;
    double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sum += matrix[i][j];
        }
    }
    double t2 = omp_get_wtime();

    cout << "Sum calculation time: " << t2 - t1 << " seconds" << endl;

    return sum;
}

int find_min_row() {
    int min_row = 0;
    long long min_sum = calculate_sum_row(0);
    double t1 = omp_get_wtime();
#pragma omp parallel for
    for (int i = 1; i < rows; i++) {
        long long row_sum = calculate_sum_row(i);
#pragma omp critical
        {
            if (row_sum < min_sum) {
                min_sum = row_sum;
                min_row = i;
            }
        }
    }
    double t2 = omp_get_wtime();

    cout << "Minimum row calculation time: " << t2 - t1 << " seconds" << endl;

    return min_row;
}

long long calculate_sum_row(int row) {
    long long sum = 0;
    for (int j = 0; j < cols; j++) {
        sum += matrix[row][j];
    }
    return sum;
}
