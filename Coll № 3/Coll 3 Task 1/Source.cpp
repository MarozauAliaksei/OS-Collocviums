#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <numeric>
#include <string>
// �����, �������������� ��������
class Operation {
public:
    virtual double perform(const std::vector<double>& values) const = 0;
    virtual ~Operation() = default;
};

// ����� ��� �������� �����
class Addition : public Operation {
public:
    double perform(const std::vector<double>& values) const override {
        return std::accumulate(values.begin(), values.end(), 0.0);
    }
};

// ����� ��� ��������� �����
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

// ����� ��� ���������� ����� ���������
class SumOfSquares : public Operation {
public:
    double perform(const std::vector<double>& values) const override {
        return std::accumulate(values.begin(), values.end(), 0.0,
            [](double sum, double value) {
                return sum + value * value;
            });
    }
};

// ����� ��� ��������� ������� ������ � ���������� ��������
class FileProcessor {
private:
    std::string directory;
    int numThreads;

    // ��������� ��� �������� ������ �� �����
    struct FileData {
        int action;
        std::vector<double> values;
    };

    // ������ ��� �������� ������ �� ������
    std::vector<FileData> fileData;

    // ������� ��� ������������� ������� � fileData
    std::mutex dataMutex;

public:
    FileProcessor(const std::string& dir, int threads) : directory(dir), numThreads(threads) {}

    // ������� ��� ������ ������ �� �����
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

    // ������� ��� ���������� �������� � ������ ���������� � ����
    void processFiles() {
        std::vector<std::thread> threads;

        // ��������� ������ ��� ������ ������ �� ������
        for (int i = 1; i <= numThreads; ++i) {
            threads.emplace_back(&FileProcessor::readFile, this, "in_" + std::to_string(i) + ".dat");
        }

        // ���� ���������� ���� ������� ������
        for (auto& thread : threads) {
            thread.join();
        }

        // ��������� ������ ��� ���������� ��������
        threads.clear();
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&FileProcessor::performOperation, this, i);
        }

        // ���� ���������� ���� ������� ���������� ��������
        for (auto& thread : threads) {
            thread.join();
        }

        // ��������� ���������� ��������
        double result = std::accumulate(results.begin(), results.end(), 0.0);

        // ���������� ��������� � ����
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
    // ������ ��� �������� ����������� ��������
    std::vector<double> results;

    // ������� ��� ���������� �������� � ��������� ������
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

        // ��������� ��������� � ����� ������
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
