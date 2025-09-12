#ifndef BDT_MODEL_HPP
#define BDT_MODEL_HPP

#include "ShortRateModel.hpp"

// Black-Derman-Toy Model: d(ln r_t) = (theta(t) - (sigma'(t)/sigma(t))*ln r_t)dt + sigma(t)dW_t
class BDTModel : public ShortRateModel {
public:
    BDTModel(double r0, double sigma);

    double priceZeroCouponBond(double T) const override;

private:
    double sigma_; // Volatility (can be time-dependent in full implementation)
};

#endif // BDT_MODEL_HPP
