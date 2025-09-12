#ifndef HULL_WHITE_MODEL_HPP
#define HULL_WHITE_MODEL_HPP

#include "ShortRateModel.hpp"

// Hull-White Model: dr_t = (theta(t) - a*r_t)dt + sigma * dW_t
class HullWhiteModel : public ShortRateModel {
public:
    HullWhiteModel(double r0, double a, double sigma);

    double priceZeroCouponBond(double T) const override;

    // Helper to get the market instantaneous forward rate at time T
    double instantaneousForwardRate(double T) const;

private:
    double a_;     // Speed of mean reversion
    double sigma_; // Volatility
};

#endif // HULL_WHITE_MODEL_HPP
