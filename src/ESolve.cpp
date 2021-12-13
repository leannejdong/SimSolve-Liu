#include <iostream>
#include "../include/iteration.h"
#include "../include/ESolve.h"
#include <Eigen/Dense>

namespace SimpleSolve{
    void ESolve()
    {
        /// F
        auto F = [](const Eigen::VectorXd &m){
            Eigen::VectorXd res(3);
            res << m(0)-m(1)-2,
                    m(1)+m(2)-3,
                    K1 * (m(0) * std::abs(m(0)) + m(1) * std::abs(m(1))) - K3 * m(2) * std::abs(m(2));
            return res;
        };
        /// jacobian of F
        auto DF= [] (const Eigen::VectorXd &m){
            Eigen::MatrixXd J(3,3);
            J << 1, -1, 0,
                    0, 1, 1,
                    2 * K1 * std::abs(m(0)) ,  2 * K1 * std::abs(m(1)),  -2 * K3 * std::abs(m(2));
            return J;
        };

        Eigen::VectorXd m(3);
        m << 1.0, 1.0, 1.0; // initial value
        const double tolerance = 1e-14; //  Creates a Newton object which is pointed to by newton
        std::unique_ptr<SimpleSolve::Iteration> newton = std::make_unique<SimpleSolve::Newton>(m, tolerance, F, DF);
        VectorXd newtonResults = newton->solve(m);  // Dynamic dispatch works as expect
        std::cout << std::setprecision(17) << "solution = " << newtonResults << "\n";
        std::cout << std::setprecision(17) << "error norm = " << F(m).norm() << "\n";

    }

}

/* Dynamic dispatch: The function that is executed is chosen at runtime based on the dynamic type of the object.
 * As opposed to static dispatch, where the exact function that is going to be called is known at compile time */