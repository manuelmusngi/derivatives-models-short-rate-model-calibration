#include "HoLeeModel.hpp"
#include <cmath>
#include <iostream>

HoLeeModel::HoLeeModel(double r0, double sigma) : sigma_(sigma) {
    r0_ = r0;
}

double HoLeeModel::priceZeroCouponBond(double T) const {
    if (T < 1e-6) {
        return 1.0;
    }

    // For the Ho-Lee model, the ZCB price P(t, T) is given by:
    // P(t, T) = A(t, T) * exp(-B(t, T) * r_t)
    // We are at t=0.
    // B(0, T) = T
    // A(0, T) = (P_market(0, T)) * exp(r0*T - 0.5 * sigma^2 * T^2)
    // Here, P_market is the observed market price, making it arbitrage-free.
    // We combine these to get the model price based on the initial term structure.
    
    auto it = termStructure_.find(T);
    if (it == termStructure_.end()) {
        std::cerr << "Market price for maturity " << T << " not found!" << std::endl;
        return -1.0;
    }
    double p_market_0_T = it->second;

    double B_0_T = T;
    double A_0_T = p_market_0_T * exp(B_0_T * r0_ - 0.5 * sigma_ * sigma_ * T * T);
    
    // The simplified formula often seen is derived from the dynamics, but for calibration,
    // we must fit the market curve. The full formula is:
    // P(0,T) = P_market(0,T) * exp(-0.5 * sigma^2 * T^2)
    // However, this form isn't useful for calibration as it just adds a factor.
    // The true power is in pricing derivatives consistently.
    // For this example, we show the analytical ZCB pricing formula *without* market fitting
    // to demonstrate the model's core behavior.
    
    double term = 0.5 * sigma_ * sigma_ * T * T;
    return exp(-r0_ * T + term);
}
