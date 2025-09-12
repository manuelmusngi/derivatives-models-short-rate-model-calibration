#include "BDTModel.hpp"
#include <iostream>
#include <stdexcept>

BDTModel::BDTModel(double r0, double sigma) : sigma_(sigma) {
    r0_ = r0;
}

double BDTModel::priceZeroCouponBond(double T) const {
    // d(ln r_t) = (theta(t) - (sigma'(t)/sigma(t))*ln r_t)dt + sigma(t)dW_t
    // There is no closed-form analytical solution for the BDT model's bond price.
    // This model must be implemented using a numerical method, typically a 
    // binomial tree. The parameters theta(t) and sigma(t) are chosen at each
    // step of the tree to match the market yield curve and volatility curve.
    
    std::cout << "BDTModel requires a numerical implementation (e.g., binomial tree)." << std::endl;
    throw std::runtime_error("Pricing not implemented for BDTModel. Use numerical methods.");
    return -1.0;
}
