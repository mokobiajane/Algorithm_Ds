#include "gauss.hpp"
#include <fstream>
#include <sstream>
#include <random>

Eigen::MatrixXd readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> values;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        values.push_back(row);
    }

    Eigen::MatrixXd mat(values.size(), values[0].size());
    for (size_t i = 0; i < values.size(); ++i)
        for (size_t j = 0; j < values[i].size(); ++j)
            mat(i, j) = values[i][j];
    return mat;
}

void writeCSV(const std::string& filename, const Eigen::VectorXd& vec) {
    std::ofstream file(filename);
    for (int i = 0; i < vec.size(); ++i)
        file << vec(i) << "\n";
}

Eigen::MatrixXd generateSystem(int size, int seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    Eigen::MatrixXd mat(size, size + 1);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size + 1; ++j)
            mat(i, j) = dis(gen);
    return mat;
}

Eigen::VectorXd gaussianElimination(Eigen::MatrixXd augmented) {
    const int n = augmented.rows();
    const double epsilon = 1e-9;  // Small threshold to consider as zero for numerical precision

    for (int i = 0; i < n; ++i) {
        int maxRow = i;
        // Find the row with the largest pivot element in column i
        for (int k = i + 1; k < n; ++k)
            if (std::abs(augmented(k, i)) > std::abs(augmented(maxRow, i)))
                maxRow = k;

        // Swap the current row with the one having the largest pivot element
        augmented.row(i).swap(augmented.row(maxRow));

        // Check if the pivot element is close to zero, indicating a singular matrix
        if (std::abs(augmented(i, i)) < epsilon) {
            // Check if the entire row is near zero (indicating linear dependence)
            bool isRowZero = true;
            for (int j = i; j < n; ++j) {
                if (std::abs(augmented(i, j)) > epsilon) {
                    isRowZero = false;
                    break;
                }
            }

            if (isRowZero) {
                throw std::runtime_error("Matrix is singular due to linearly dependent rows.");
            } else {
                throw std::runtime_error("Matrix is singular and cannot be solved.");
            }
        }

        // Eliminate the entries below the pivot element
        for (int k = i + 1; k < n; ++k) {
            double factor = augmented(k, i) / augmented(i, i);
            augmented.row(k) -= factor * augmented.row(i);
        }
    }

    // Back substitution to solve for the unknowns
    Eigen::VectorXd x(n);
    for (int i = n - 1; i >= 0; --i) {
        x(i) = augmented(i, n);
        for (int j = i + 1; j < n; ++j)
            x(i) -= augmented(i, j) * x(j);
        x(i) /= augmented(i, i);
    }

    return x;
}

