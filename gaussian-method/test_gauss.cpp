#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "gauss.hpp"

TEST_CASE("Gaussian elimination solves a simple system") {
    Eigen::MatrixXd augmented(3, 4);
    augmented << 2, 1, -1, 8,
                -3, -1, 2, -11,
                -2, 1, 2, -3;

    Eigen::VectorXd result = gaussianElimination(augmented);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == Approx(2.0).margin(1e-6));
    REQUIRE(result[1] == Approx(3.0).margin(1e-6));
    REQUIRE(result[2] == Approx(-1.0).margin(1e-6));
}
TEST_CASE("Gaussian elimination solves a random generated system") {
    int size = 5;
    Eigen::MatrixXd system = generateSystem(size, 123);
    Eigen::MatrixXd A = system.leftCols(size);
    Eigen::VectorXd b = system.rightCols(1);
    
    Eigen::VectorXd x = gaussianElimination(system);
    Eigen::VectorXd reconstructed_b = A * x;

    for (int i = 0; i < size; ++i) {
        REQUIRE(reconstructed_b(i) == Approx(b(i)).margin(1e-6));
    }
}
TEST_CASE("Gaussian elimination solves identity system") {
    int size = 4;
    Eigen::MatrixXd augmented = Eigen::MatrixXd::Zero(size, size + 1);
    for (int i = 0; i < size; ++i) {
        augmented(i, i) = 1.0;
        augmented(i, size) = static_cast<double>(i + 1);  // b = [1, 2, 3, 4]
    }

    Eigen::VectorXd x = gaussianElimination(augmented);
    for (int i = 0; i < size; ++i) {
        REQUIRE(x(i) == Approx(i + 1.0).margin(1e-6));
    }
}