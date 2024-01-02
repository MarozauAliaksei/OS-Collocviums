#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

class MatrixMultiplier {
private:
    int m, n, k;
    std::vector<std::vector<double>> A, B, C;

    // ������� ��� ������������� ������� � ������� C � ������ �����������
    std::mutex mutex;

public:
    MatrixMultiplier(int rowsA, int colsA, int colsB) : m(rowsA), n(colsA), k(colsB), A(rowsA, std::vector<double>(colsA)),
        B(colsA, std::vector<double>(colsB)), C(rowsA, std::vector<double>(colsB, 0.0)) {}

    // ���������� ������ A � B
    void fillMatrices() {
        fillMatrix(A);
        fillMatrix(B);
    }

    // ���������� ������������ ������ � p �������
    void multiplyMatrices(int numThreads) {
        std::vector<std::thread> threads;

        // ������ ������� ��� ���������� ��������� ������� C
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&MatrixMultiplier::computeElements, this, i);
        }

        // ���� ���������� ���� ������� ����������
        for (auto& thread : threads) {
            thread.join();
        }

        // ����� ���������� � ������ � ����-��������
        printAndWriteResult();
    }

private:
    // ������� ��� ���������� ������� ���������� ����������
    void fillMatrix(std::vector<std::vector<double>>& matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[0].size(); ++j) {
                matrix[i][j] = rand() % 10 + 1; // ��������� �������� �� 1 �� 10
            }
        }
    }

    // ������� ��� ���������� ��������� ������� C � ��������� ������
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

    // ������� ��� ������ ���������� � ������ � ����-��������
    void printAndWriteResult() {
        std::cout << "Result Matrix C:" << std::endl;
        for (const auto& row : C) {
            for (double value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }

        // ������ � ����-��������
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
