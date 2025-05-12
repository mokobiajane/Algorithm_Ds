#include "gauss.hpp"
#include <iostream>

int main() {
    Eigen::MatrixXd augmented = readCSV("input.csv");
    Eigen::VectorXd solution = gaussianElimination(augmented);
    writeCSV("output.csv", solution);
    std::cout << "Solution written to output.csv\n";
    return 0;
}