#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <memory>
#include <concepts> // C++20: For creating concepts
#include <format>   // C++20: For modern string formatting

#include "HullWhiteModel.hpp"
#include "HoLeeModel.hpp" // Include other models for potential use

// C++20 Concept: Ensures a type T is a valid short-rate model derived from our base class.
// This provides compile-time checking and better error messages.
template<typename T>
concept IsShortRateModel = std::is_base_of_v<ShortRateModel, T>;

// This is the function an optimizer would seek to minimize.
// It calculates the Sum of Squared Errors (SSE) between market and model prices.
// We use our C++20 concept to ensure this function only works with valid model types.
double calculateSSE(const IsShortRateModel auto& model, const std::map<double, double>& marketPrices) {
    double sum_squared_errors = 0.0;

    // Use a C++17 structured binding, which is already excellent practice.
    for (const auto& [maturity, market_price] : marketPrices) {
        if (maturity < 0.001) continue; // Skip helper points not used for error calculation

        double model_price = model.priceZeroCouponBond(maturity);
        sum_squared_errors += std::pow(market_price - model_price, 2);
    }
    return sum_squared_errors;
}


int main() {
    // =========================================================================
    // 1. SETUP: Define current market conditions (a sample yield curve)
    // =========================================================================
    std::cout << "Setting up market data...\n";
    std::map<double, double> marketYields{ // Maturity -> Zero-Coupon Yield
        {0.25, 0.010}, {0.5, 0.012}, {1.0, 0.015}, {2.0, 0.020},
        {5.0, 0.025}, {10.0, 0.030}
    };

    // Convert yields to Zero-Coupon Bond prices: P(T) = exp(-yield * T)
    std::map<double, double> marketPrices;
    for (const auto& [maturity, yield] : marketYields) {
        marketPrices[maturity] = exp(-yield * maturity);
    }
    // Add a very short-term price for forward rate calculations
    marketPrices[0.001] = exp(-marketYields[0.25] * 0.001);

    // The initial short rate r(0) is the instantaneous rate at t=0.
    const double r0 = marketYields[0.25];
    std::cout << std::format("Initial short rate (r0) set to: {}\n\n", r0);

    // =========================================================================
    // 2. CALIBRATION PROCESS: Using the Hull-White Model
    // =========================================================================
    std::cout << "--- Hull-White Model Calibration ---\n";

    // Initial guesses for model parameters
    double initial_a = 0.1;
    double initial_sigma = 0.01;
    
    std::cout << std::format("Initial Guess: a = {}, sigma = {}\n", initial_a, initial_sigma);

    HullWhiteModel model(r0, initial_a, initial_sigma);
    model.setTermStructure(marketPrices);

    // Calculate and display prices with initial guesses using std::format
    std::cout << "\nPrices with initial (uncalibrated) parameters:\n";
    std::cout << std::format("{:<8} | {:<12} | {:<11} | {}\n", "Maturity", "Market Price", "Model Price", "Difference");
    std::cout << "---------|--------------|-------------|-----------\n";
    for(const auto& [maturity, yield] : marketYields) {
        double market_p = marketPrices.at(maturity);
        double model_p = model.priceZeroCouponBond(maturity);
        std::cout << std::format("{:<8.2f} | {:<12.6f} | {:<11.6f} | {:<10.6f}\n",
                                 maturity, market_p, model_p, market_p - model_p);
    }

    // =========================================================================
    // 3. OPTIMIZATION (Conceptual)
    // =========================================================================
    std::cout << "\n--- Optimization Step (Conceptual) ---\n";
    std::cout << "To calibrate, you would feed a lambda wrapping 'calculateSSE' into an optimizer.\n";
    std::cout << "The optimizer (e.g., Levenberg-Marquardt, Nelder-Mead) would search for\n";
    std::cout << "the values of 'a' and 'sigma' that minimize the sum of squared errors.\n";
    
    // Example: Calculate the error for the initial guess
    double initial_error = calculateSSE(model, marketPrices);
    std::cout << std::format("\nInitial Sum of Squared Errors: {:.6e}\n", initial_error);
    
    std::cout << "\nLet's assume an optimizer found the best-fit parameters:\n";
    double calibrated_a = 0.08;
    double calibrated_sigma = 0.012;
    std::cout << std::format("Calibrated Guess: a = {}, sigma = {}\n", calibrated_a, calibrated_sigma);
    
    // The error with these 'calibrated' parameters should be lower.
    HullWhiteModel calibrated_model(r0, calibrated_a, calibrated_sigma);
    calibrated_model.setTermStructure(marketPrices);
    double calibrated_error = calculateSSE(calibrated_model, marketPrices);
    std::cout << std::format("Calibrated Sum of Squared Errors: {:.6e}\n", calibrated_error);

    return 0;
}
