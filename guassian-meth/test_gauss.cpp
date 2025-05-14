#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <random>
#include "gauss.hpp"

// Test 1: 3x3 system
TEST(GaussianElimination, Solves3x3System) {
    Eigen::MatrixXd A(3, 4);
    A << 2, 1, -1, 8,
        -3, -1, 2, -11,
        -2, 1, 2, -3;

    Eigen::VectorXd result = gaussianElimination(A);

    ASSERT_EQ(result.size(), 3);
    EXPECT_NEAR(result[0], 2.0, 1e-9);
    EXPECT_NEAR(result[1], 3.0, 1e-9);
    EXPECT_NEAR(result[2], -1.0, 1e-9);
}



// Test 2: Large consistent system
TEST(GaussianElimination, SolvesLargeSystem) {
    const int N = 100;
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dist(-10.0, 10.0);

    Eigen::MatrixXd A(N, N);
    Eigen::VectorXd b(N);
    Eigen::VectorXd x_true(N);

    
    for (int i = 0; i < N; ++i) {
        x_true(i) = dist(gen);
        for (int j = 0; j < N; ++j) {
            A(i, j) = dist(gen);
        }
    }
    b = A * x_true;

    Eigen::MatrixXd aug(N, N + 1);
    aug << A, b;

    Eigen::VectorXd result = gaussianElimination(aug);

    ASSERT_EQ(result.size(), N);
    for (int i = 0; i < N; ++i) {
        EXPECT_NEAR(result[i], x_true[i], 1e-6);
    }
}

// Test 3: Singular matrix with linearly dependent rows
TEST(GaussianElimination, ThrowsOnLinearlyDependentRows) {
    Eigen::MatrixXd A(3, 4);
    
    A << 1, 2, -1, 3,
         2, 4, -2, 6,   
        -1, -2, 1, -3;  

    EXPECT_THROW({
        gaussianElimination(A);
    }, std::runtime_error); 
}

// Test 4: Singular matrix (linearly dependent rows)
TEST(GaussianElimination, ThrowsOnSingularMatrix) {
    Eigen::MatrixXd A(3, 4);
    A << 1, 2, -1, 3,
         2, 4, -2, 6,   
        -1, -2, 1, -3;  

    EXPECT_THROW({
        gaussianElimination(A);
    }, std::runtime_error);
}