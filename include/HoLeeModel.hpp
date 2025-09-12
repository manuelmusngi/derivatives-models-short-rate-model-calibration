#ifndef HO_LEE_MODEL_HPP
#define HO_LEE_MODEL_HPP

#include "ShortRateModel.hpp"

// Ho-Lee Model: dr_t = theta(t)dt + sigma * dW_t
class HoLeeModel : public ShortRateModel {
public:
    HoLeeModel(double r0, double sigma);

    double priceZeroCouponBond(double T) const override;

private:
    double sigma_; // Volatility
};

#endif // HO_LEE_MODEL_HPP
