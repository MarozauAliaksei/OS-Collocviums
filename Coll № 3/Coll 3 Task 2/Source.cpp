#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

class MatrixMultiplier {
private:
    int m, n, k;
    std::vector<std::vector<double>> A, B, C;

    // Мьютекс для синхронизации доступа к матрице C и выводу результатов
    std::mutex mutex;

public:
    MatrixMultiplier(int rowsA, int colsA, int colsB) : m(rowsA), n(colsA), k(colsB), A(rowsA, std::vector<double>(colsA)),
        B(colsA, std::vector<double>(colsB)), C(rowsA, std::vector<double>(colsB, 0.0)) {}

    // Заполнение матриц A и B
    void fillMatrices() {
        fillMatrix(A);
        fillMatrix(B);
    }

    // Вычисление произведения матриц в p потоков
    void multiplyMatrices(int numThreads) {
        std::vector<std::thread> threads;

        // Запуск потоков для вычисления элементов матрицы C
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&MatrixMultiplier::computeElements, this, i);
        }

        // Ждем завершения всех потоков вычисления
        for (auto& thread : threads) {
            thread.join();
        }

        // Вывод результата и запись в файл-протокол
        printAndWriteResult();
    }

private:
    // Функция для заполнения матрицы случайными значениями
    void fillMatrix(std::vector<std::vector<double>>& matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[0].size(); ++j) {
                matrix[i][j] = rand() % 10 + 1; // случайные значения от 1 до 10
            }
        }
    }

    // Функция для вычисления элементов матрицы C в отдельном потоке
    void computeElements(int threadIndex) {
        for (int i = threadIndex; i < m * k; i += 1) {
            int row = i / k;
            int col = i % k;
            double sum = 0.0;

            for (int j = 0; j < n; ++j) {
                sum += A[row][j] * B[j][col];
            }

            std::lock_guard<std::mutex> lock(mutex);
            C[row][col] = sum;
        }
    }

    // Функция для вывода результата и записи в файл-протокол
    void printAndWriteResult() {
        std::cout << "Result Matrix C:" << std::endl;
        for (const auto& row : C) {
            for (double value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }

        // Запись в файл-протокол
        std::ofstream logFile("log.txt");
        if (logFile.is_open()) {
            for (const auto& row : C) {
                for (double value : row) {
                    logFile << value << " ";
                }
                logFile << std::endl;
            }
            logFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }
};

int main() {
    srand(time(0));

    int m, n, k;
    std::cout << "Enter the dimensions of matrices (m n k): ";
    std::cin >> m >> n >> k;

    int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    MatrixMultiplier matrixMultiplier(m, n, k);
    matrixMultiplier.fillMatrices();
    matrixMultiplier.multiplyMatrices(numThreads);

    return 0;
}
