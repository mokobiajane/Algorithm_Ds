#pragma once
#include <Eigen/Dense>
#include <string>

Eigen::MatrixXd readCSV(const std::string& filename);
void writeCSV(const std::string& filename, const Eigen::VectorXd& vec);
Eigen::VectorXd gaussianElimination(Eigen::MatrixXd augmented);
Eigen::MatrixXd generateSystem(int size, int seed = 42);