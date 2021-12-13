#include <iostream>
#include <iomanip>
#include<vector>
#include<string>
#include <cmath>
#include <cstdlib>
#include <functional>
//#include <fmt/format.h>
#include <cassert>
#include <memory>
#include <iostream>

#include <Eigen/Dense>
using std::cerr;
using Eigen::VectorXd;
using Eigen::MatrixXd;

constexpr double K1{0.0179};
constexpr double K3{0.0269};

namespace SimpleSolve{
    class Iteration{
    public:
        virtual VectorXd solve(Eigen::VectorXd&) { return {}; }

//        int numberOfIterations() const { return mNumberOfIterations; }
        virtual ~Iteration() = default; // A virtual function is a member function which is declared within a base class and re-defined by a derived class.

    protected: // inaccessible outside class, accessible by derived class
        explicit Iteration(double epsilon) : mEpsilon(epsilon), mNumberOfIterations(0) {} // explicit prevent ctor from automatic conversion

        void resetNumberOfIterations() { mNumberOfIterations = 0; }
//        int incrementNumberOfIterations() { return mNumberOfIterations++; }
        double epsilon() const { return mEpsilon; }

    private:
        const double mEpsilon;
        int mNumberOfIterations;
    };

    class Newton : public Iteration{
    public:

        Newton(Eigen::VectorXd& m,
               double tolerance,
               std::function<Eigen::VectorXd(const Eigen::VectorXd&)> F,
               std::function<Eigen::MatrixXd(const Eigen::VectorXd&)> DF
               ): Iteration(tolerance), _F(F), _DF(DF){}
        //~Newton() override = default;

       VectorXd solve(Eigen::VectorXd&x) override {
            resetNumberOfIterations();

            VectorXd Fx = _F(x);
            MatrixXd DF = _DF(x);

            calculateX(x, epsilon(), _F, _DF );
            return x;
       }
    private:// the calculateX uses std::function which allows us to optimize different function
        static void calculateX(VectorXd& x, double tolerance,
                               std::function<Eigen::VectorXd(const Eigen::VectorXd&)> F,
                               std::function<Eigen::MatrixXd(const Eigen::VectorXd&)> DF) {
            Eigen::VectorXd x_prev, s;
            std::vector<double> errors;
            errors.push_back((F(x)).norm());
            do{
                x_prev = x;
                s = DF(x).lu().solve(F(x));
                x = x - s;
            } while (s.norm() > tolerance *x.norm());

        }
        std::function<Eigen::VectorXd(const Eigen::VectorXd&)> _F;
        std::function<Eigen::MatrixXd(const Eigen::VectorXd&)> _DF;

    };


}
