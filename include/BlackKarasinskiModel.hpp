#ifndef BLACK_KARASINSKI_MODEL_HPP
#define BLACK_KARASINSKI_MODEL_HPP

#include "ShortRateModel.hpp"

// Black-Karasinski Model: d(ln r_t) = (theta(t) - a*ln(r_t))dt + sigma*dW_t
class BlackKarasinskiModel : public ShortRateModel {
public:
    BlackKarasinskiModel(double r0, double a, double sigma);

    double priceZeroCouponBond(double T) const override;

private:
    double a_;     // Speed of mean reversion
    double sigma_; // Volatility
};

#endif // BLACK_KARASINSKI_MODEL_HPP
