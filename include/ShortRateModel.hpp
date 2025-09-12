#ifndef SHORT_RATE_MODEL_HPP
#define SHORT_RATE_MODEL_HPP

#include <vector>
#include <map>

// Abstract base class for all one-factor short rate models.
class ShortRateModel {
public:
    virtual ~ShortRateModel() = default;

    // The core function: prices a zero-coupon bond maturing at time T.
    // P(t, T) where t is today (t=0).
    virtual double priceZeroCouponBond(double T) const = 0;

    // A helper function to get the initial term structure (market prices).
    // This is needed for arbitrage-free models.
    virtual void setTermStructure(const std::map<double, double>& marketPrices) {
        termStructure_ = marketPrices;
    }

protected:
    double r0_; // Initial short rate
    std::map<double, double> termStructure_; // Market ZCB prices P(0, T)
};

#endif // SHORT_RATE_MODEL_HPP
