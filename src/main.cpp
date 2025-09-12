#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <memory>
#include "HullWhiteModel.hpp"

// This is the function an optimizer would seek to minimize.
// It calculates the Sum of Squared Errors (SSE) between market and model prices.
double objectiveFunction(
    double a,
    double sigma,
    double r0,
    const std::map<double, double>& marketPrices,
    const HullWhiteModel& model)
{
    HullWhiteModel tempModel(r0, a, sigma);
    tempModel.setTermStructure(marketPrices);

    double sum_squared_errors = 0.0;
    for (const auto& pair : marketPrices) {
        double T = pair.first;
        double market_price = pair.second;
        double model_price = tempModel.priceZeroCouponBond(T);
        sum_squared_errors += pow(market_price - model_price, 2);
    }
    return sum_squared_errors;
}

int main() {
    // =========================================================================
    // 1. SETUP: Define current market conditions (a sample yield curve)
    // =========================================================================
    std::cout << "Setting up market data..." << std::endl;
    std::map<double, double> marketYields; // Maturity -> Zero-Coupon Yield
    marketYields[0.25] = 0.010; // 3m
    marketYields[0.5]  = 0.012; // 6m
    marketYields[1.0]  = 0.015; // 1y
    marketYields[2.0]  = 0.020; // 2y
    marketYields[5.0]  = 0.025; // 5y
    marketYields[10.0] = 0.030; // 10y

    // Convert yields to Zero-Coupon Bond prices: P(T) = exp(-yield * T)
    std::map<double, double> marketPrices;
    for (const auto& pair : marketYields) {
        marketPrices[pair.first] = exp(-pair.second * pair.first);
    }
    // Add P(0, 0.001) for forward rate calculation
    marketPrices[0.001] = exp(-marketYields[0.25] * 0.001);


    // The initial short rate r(0) is the instantaneous rate at t=0.
    // We can approximate it from the first point on the curve.
    double r0 = marketYields[0.25];
    std::cout << "Initial short rate (r0) set to: " << r0 << std::endl << std::endl;

    // =========================================================================
    // 2. CALIBRATION PROCESS: Using the Hull-White Model
    // =========================================================================
    std::cout << "--- Hull-White Model Calibration ---" << std::endl;

    // Initial guesses for model parameters
    double initial_a = 0.1;
    double initial_sigma = 0.01;
    
    std::cout << "Initial Guess: a = " << initial_a << ", sigma = " << initial_sigma << std::endl;

    HullWhiteModel model(r0, initial_a, initial_sigma);
    model.setTermStructure(marketPrices);

    // Calculate and display prices with initial guesses
    std::cout << "\nPrices with initial (uncalibrated) parameters:" << std::endl;
    std::cout << "Maturity | Market Price | Model Price | Difference" << std::endl;
    std::cout << "---------|--------------|-------------|-----------" << std::endl;
    for(const auto& pair : marketYields) { // Iterate yields to keep order
        double T = pair.first;
        double market_p = marketPrices.at(T);
        double model_p = model.priceZeroCouponBond(T);
        printf("%-8.2f | %-12.6f | %-11.6f | %-10.6f\n", T, market_p, model_p, market_p - model_p);
    }

    // =========================================================================
    // 3. OPTIMIZATION (Conceptual)
    // =========================================================================
    std::cout << "\n--- Optimization Step (Conceptual) ---" << std::endl;
    std::cout << "To calibrate, you would feed the 'objectiveFunction' into an optimizer." << std::endl;
    std::cout << "The optimizer (e.g., Levenberg-Marquardt, Nelder-Mead) would search for" << std::endl;
    std::cout << "the values of 'a' and 'sigma' that minimize the sum of squared errors." << std::endl;
    
    // Example: Calculate the error for the initial guess
    double initial_error = objectiveFunction(initial_a, initial_sigma, r0, marketPrices, model);
    std::cout << "\nInitial Sum of Squared Errors: " << initial_error << std::endl;
    
    std::cout << "\nLet's assume an optimizer found the best-fit parameters:" << std::endl;
    double calibrated_a = 0.08;
    double calibrated_sigma = 0.012;
    std::cout << "Calibrated Guess: a = " << calibrated_a << ", sigma = " << calibrated_sigma << std::endl;
    
    // The error with these 'calibrated' parameters should be lower.
    double calibrated_error = objectiveFunction(calibrated_a, calibrated_sigma, r0, marketPrices, model);
    std::cout << "Calibrated Sum of Squared Errors: " << calibrated_error << std::endl;


    return 0;
}
