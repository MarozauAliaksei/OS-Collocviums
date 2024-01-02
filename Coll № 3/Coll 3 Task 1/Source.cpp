#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <numeric>
#include <string>
// Класс, представляющий действие
class Operation {
public:
    virtual double perform(const std::vector<double>& values) const = 0;
    virtual ~Operation() = default;
};

// Класс для сложения чисел
class Addition : public Operation {
public:
    double perform(const std::vector<double>& values) const override {
        return std::accumulate(values.begin(), values.end(), 0.0);
    }
};

// Класс для умножения чисел
class Multiplication : public Operation {
public:
    double perform(const std::vector<double>& values) const override {
        double result = 1.0;
        for (double value : values) {
            result *= value;
        }
        return result;
    }
};

// Класс для вычисления суммы квадратов
class SumOfSquares : public Operation {
public:
    double perform(const std::vector<double>& values) const override {
        return std::accumulate(values.begin(), values.end(), 0.0,
            [](double sum, double value) {
                return sum + value * value;
            });
    }
};

// Класс для обработки входных файлов и выполнения операций
class FileProcessor {
private:
    std::string directory;
    int numThreads;

    // Структура для хранения данных из файла
    struct FileData {
        int action;
        std::vector<double> values;
    };

    // Вектор для хранения данных из файлов
    std::vector<FileData> fileData;

    // Мьютекс для синхронизации доступа к fileData
    std::mutex dataMutex;

public:
    FileProcessor(const std::string& dir, int threads) : directory(dir), numThreads(threads) {}

    // Функция для чтения данных из файла
    void readFile(const std::string& filename) {
        std::ifstream file(directory + "/" + filename);
        if (file.is_open()) {
            FileData data;
            file >> data.action;
            double value;
            while (file >> value) {
                data.values.push_back(value);
            }

            std::lock_guard<std::mutex> lock(dataMutex);
            fileData.push_back(data);

            file.close();
        }
        else {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }

    // Функция для выполнения операций и записи результата в файл
    void processFiles() {
        std::vector<std::thread> threads;

        // Запускаем потоки для чтения данных из файлов
        for (int i = 1; i <= numThreads; ++i) {
            threads.emplace_back(&FileProcessor::readFile, this, "in_" + std::to_string(i) + ".dat");
        }

        // Ждем завершения всех потоков чтения
        for (auto& thread : threads) {
            thread.join();
        }

        // Запускаем потоки для выполнения операций
        threads.clear();
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&FileProcessor::performOperation, this, i);
        }

        // Ждем завершения всех потоков выполнения операций
        for (auto& thread : threads) {
            thread.join();
        }

        // Суммируем результаты операций
        double result = std::accumulate(results.begin(), results.end(), 0.0);

        // Записываем результат в файл
        std::ofstream outFile(directory + "/out.dat");
        if (outFile.is_open()) {
            outFile << result << std::endl;
            outFile.close();
        }
        else {
            std::cerr << "Error opening output file." << std::endl;
        }
    }

private:
    // Вектор для хранения результатов операций
    std::vector<double> results;

    // Функция для выполнения операции в отдельном потоке
    void performOperation(int threadIndex) {
        const FileData& data = fileData[threadIndex];
        double result;

        switch (data.action) {
        case 1:
            result = Addition().perform(data.values);
            break;
        case 2:
            result = Multiplication().perform(data.values);
            break;
        case 3:
            result = SumOfSquares().perform(data.values);
            break;
        default:
            std::cerr << "Unknown action: " << data.action << std::endl;
            result = 0.0;
        }

        // Сохраняем результат в общий вектор
        std::lock_guard<std::mutex> lock(dataMutex);
        results.push_back(result);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <directory> <num_threads>" << std::endl;
        return 1;
    }

    std::string directory = argv[1];
    int numThreads = std::stoi(argv[2]);

    FileProcessor fileProcessor(directory, numThreads);
    fileProcessor.processFiles();

    return 0;
}
