#include "HullWhiteModel.hpp"
#include <cmath>
#include <iostream>

HullWhiteModel::HullWhiteModel(double r0, double a, double sigma)
    : a_(a), sigma_(sigma) {
    r0_ = r0;
}

// Function to get instantaneous forward rate f(0, T) from the market curve.
// f(0, T) = -d/dT(ln P(0, T))
// We approximate this using finite differences.
double HullWhiteModel::instantaneousForwardRate(double T) const {
    double dt = 0.001;
    auto p_T = termStructure_.find(T);
    auto p_T_dt = termStructure_.find(T + dt);

    if (p_T == termStructure_.end() || p_T_dt == termStructure_.end()) {
        // Fallback for the last point
        return -log(termStructure_.rbegin()->second) / termStructure_.rbegin()->first;
    }

    return -(log(p_T_dt->second) - log(p_T->second)) / dt;
}


double HullWhiteModel::priceZeroCouponBond(double T) const {
    if (T < 1e-6) {
        return 1.0;
    }
    // For the Hull-White model, the ZCB price P(t, T) is given by:
    // P(t, T) = A(t, T) * exp(-B(t, T) * r_t)
    // We are at t=0.

    // B(t, T) = (1/a) * (1 - exp(-a * (T - t)))
    double B_0_T = (1.0 / a_) * (1.0 - exp(-a_ * T));

    // A(t, T) ensures the model is arbitrage-free (fits the initial term structure).
    // ln A(t, T) = ln(P_market(0, T)/P_market(0, t)) + B(t,T)f_market(0,t) - (sigma^2/(4a))*(1-exp(-2at))*B(t,T)^2
    // At t=0, P_market(0,0)=1 and f_market(0,0)=r0
    // So, ln A(0, T) = ln(P_market(0, T)) + B(0,T)*r0 - (sigma^2/(4a))*(1-exp(0))*B(0,T)^2 = ln(P_market(0,T))
    // A simplified form is often used which integrates the theta term.

    // The price is P(0, T) = exp(-integral from 0 to T of f(0,u)du)
    // The model's instantaneous forward rate f(t,T) is:
    // f(t,T) = f_market(0, T) + (sigma^2/(2a^2))*(1-exp(-a(T-t)))^2 + (r_t - f_market(0,t))*exp(-a(T-t))
    // At t=0, this simplifies and allows us to derive the price.
    
    // A commonly used analytical solution for P(0, T) is:
    double term1 = instantaneousForwardRate(T);
    double term2 = (sigma_ * sigma_ / (2.0 * a_ * a_)) * pow(1.0 - exp(-a_ * T), 2.0);
    double expected_avg_rate_integral = term1 * T; // This is a simplification; in reality, we integrate f(0,u)

    // The full analytical price at t=0, fitting the initial term structure is:
    auto it = termStructure_.find(T);
    if (it == termStructure_.end()) {
        std::cerr << "Market price for maturity " << T << " not found!" << std::endl;
        return -1.0;
    }
    double p_market_0_T = it->second;
    
    double term_for_A = (sigma_ * sigma_ / (4.0 * a_)) * (1 - exp(-2 * a_ * T)) * B_0_T * B_0_T;
    double log_A_0_T = log(p_market_0_T) + B_0_T * instantaneousForwardRate(0.0) - term_for_A;

    return exp(log_A_0_T - B_0_T * r0_);
}
