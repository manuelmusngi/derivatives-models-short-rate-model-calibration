#include "BlackKarasinskiModel.hpp"
#include <iostream>
#include <stdexcept>

BlackKarasinskiModel::BlackKarasinskiModel(double r0, double a, double sigma)
    : a_(a), sigma_(sigma) {
    r0_ = r0;
}

double BlackKarasinskiModel::priceZeroCouponBond(double T) const {
    // d(ln r_t) = (theta(t) - a*ln(r_t))dt + sigma*dW_t
    // Like the BDT model, Black-Karasinski is a lognormal model with no
    // analytical solution for bond prices. It is typically implemented
    // using a trinomial tree. The theta(t) term is adjusted at each time
    // step to ensure the model correctly prices the initial term structure.

    std::cout << "BlackKarasinskiModel requires a numerical implementation (e.g., trinomial tree)." << std::endl;
    throw std::runtime_error("Pricing not implemented for BlackKarasinskiModel. Use numerical methods.");
    return -1.0;
}
