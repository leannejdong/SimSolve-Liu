#include "doctest.h"
#include "../include/iteration.h"
#include <cassert>
#include <Eigen/Dense>
#include <iostream>


using Eigen::VectorXd;

TEST_CASE("Test eigen 3d solver"){
  VectorXd expect(3);
  expect << 2.712375280851471, 0.71237528308514719, 2.2876247169148529;
  std::cerr << "The expected solutions are: " << expect.transpose() << "\n";

  // F
  auto F = [](const Eigen::VectorXd &m){
    Eigen::VectorXd res(3);
    res << m(0)-m(1)-2,
        m(1)+m(2)-3,
        K1 * (m(0) * abs(m(0)) + m(1) * abs(m(1))) - K3 * m(2) * abs(m(2));
    return res;
  };
  // jacobian of F
  auto DF= [] (const Eigen::VectorXd &m){
    Eigen::MatrixXd J(3,3);
    J << 1, -1, 0,
        0, 1, 1,
        2 * K1 * abs(m(0)) ,  2 * K1 * abs(m(1)),  -2 * K3 * abs(m(2));
    return J;
  };

  Eigen::VectorXd m(3);
  m << 1.0, 1.0, 1.0; // initial value
  const double tolerance = 1e-14;
  std::unique_ptr<SimpleSolve::Iteration> newton = std::make_unique<SimpleSolve::Newton>(m, tolerance, F, DF);
  VectorXd result = newton->solve(m);
  std::cerr << std::setprecision(17) << "solution = " << result << "\n";
  CHECK((result - expect).norm()<=1e-4);
  std::cerr << "pass l1 norm\n";
  CHECK((result - expect).squaredNorm()<=1e-8);
  std::cerr << "pass l2 norm\n";
}