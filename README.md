#### Derivatives-models-short rate model calibration framework

#### C++ Short-Rate Model Calibration Framework ⚙️

This project provides a C++ framework for calibrating one-factor short-rate models to current market conditions. It's designed to be a clear, practical, and extensible tool for quantitative finance applications.

#### Overview

Short-rate models are fundamental in finance for pricing interest rate derivatives. Before they can be used, they must be **calibrated**—their parameters must be adjusted so that they accurately price basic market instruments, like zero-coupon bonds.

This project demonstrates the entire calibration workflow:
1.  **Market Data**: Defining a yield curve from market observations.
2.  **Model Pricing**: Using the analytical formulas of a model (like Hull-White) to price bonds.
3.  **Error Calculation**: Measuring the difference between the model's prices and market prices.
4.  **Optimization**: Finding the model parameters that minimize this difference.

---

#### Models Implemented

This framework is built around an abstract base class `ShortRateModel`, making it easy to add new models.

| Model                  | Key Features                                       | Implementation Status                                       |
| ---------------------- | -------------------------------------------------- | ----------------------------------------------------------- |
| **Hull-White** | Mean Reversion, Normal Distribution (allows negative rates) | **Fully Implemented** (Analytical Solution)              |
| **Ho-Lee** | No Mean Reversion, Normal Distribution             | **Fully Implemented** (Analytical Solution)              |
| **Black-Derman-Toy (BDT)** | Lognormal Distribution (no negative rates)         | **Structure Only** (Requires numerical tree) |
| **Black-Karasinski** | Mean Reversion, Lognormal Distribution             | **Structure Only** (Requires numerical tree) |

*The lognormal models (BDT, Black-Karasinski) do not have closed-form solutions for bond prices and require numerical methods like binomial or trinomial trees for implementation.*

---
#### Project architecture

Defining the Project Structure

I'm currently crafting a detailed markdown block that maps out the project's structure. My focus is on creating a clear, hierarchical representation using backticks to format directory and file names. I'm aiming for a visually clean and easily understandable format.

Drafting the Markdown Structure

I've just finalized the initial markdown block outlining the project's structure. It's designed to be instantly usable within a GitHub README.md file. I've opted for a straightforward directory and file layout, using backticks for clear formatting. The goal is easy understanding at a glance.

Refining the Markdown Block

I've just finished the markdown block for the project structure. Now I'm meticulously adding file descriptions to the README to make it super clear. I'm focusing on concise explanations for each file, ensuring it's easy to understand at a glance. I've also incorporated the .gitignore recommendation.

Of course. Here is a clean, copy-and-paste ready markdown block for your GitHub README.md that details the project structure.

Markdown

## 📁 Project Structure

Here is the file and directory structure for the project. The layout separates the interface (`.hpp` files) from the implementation (`.cpp` files) for clean and modular code, making it easy to navigate and extend.

ShortRateModelCalibration/
├── .gitignore
├── CMakeLists.txt
├── README.md
├── include/
│   ├── ShortRateModel.hpp       # Abstract base class for all models
│   ├── HoLeeModel.hpp           # Ho-Lee model interface
│   ├── HullWhiteModel.hpp       # Hull-White model interface
│   ├── BDTModel.hpp             # BDT model interface
│   └── BlackKarasinskiModel.hpp # Black-Karasinski model interface
└── src/
├── HoLeeModel.cpp           # Ho-Lee model implementation
├── HullWhiteModel.cpp       # Hull-White model implementation
├── BDTModel.cpp             # BDT model placeholder implementation
├── BlackKarasinskiModel.cpp # Black-Karasinski placeholder implementation
└── main.cpp                 # Main calibration



#### How to Build and Run

You'll need a C++ compiler (supporting C++20) and CMake.

```bash
# 1. Clone the repository
git clone <your-repo-url>
cd ShortRateModelCalibration

# 2. Create a build directory
mkdir build && cd build

# 3. Run CMake and build the project
cmake ..
make

# 4. Run the calibrator executable
./Calibrator
```

### Example Output

```
Setting up market data...
Initial short rate (r0) set to: 0.01

--- Hull-White Model Calibration ---
Initial Guess: a = 0.1, sigma = 0.01

Prices with initial (uncalibrated) parameters:
Maturity | Market Price | Model Price | Difference
---------|--------------|-------------|-----------
0.25     | 0.997503     | 0.997505    | -0.000002
0.50     | 0.994018     | 0.994042    | -0.000024
1.00     | 0.985112     | 0.985310    | -0.000199
...

--- Optimization Step (Conceptual) ---
To calibrate, you would feed the 'objectiveFunction' into an optimizer.
The optimizer (e.g., Levenberg-Marquardt, Nelder-Mead) would search for
the values of 'a' and 'sigma' that minimize the sum of squared errors.

Initial Sum of Squared Errors: 1.2345e-06
...
```

---

#### Calibration Process Explained

The `main.cpp` file walks through the calibration process conceptually:

1.  **Market Data Setup**: We start with a `std::map` representing the market zero-coupon yield curve and convert it to bond prices. This is our "ground truth".

2.  **Objective Function**: The heart of calibration is the `objectiveFunction`. It takes model parameters (`a`, `sigma`) as input and returns a single number: the **Sum of Squared Errors (SSE)** between the prices from our model and the actual market prices.

3.  **Optimization**: In a real-world application, you would use a numerical optimization library (like [NLopt](https://nlopt.readthedocs.io/en/latest/), [Ceres Solver](http://ceres-solver.org/), or Boost.Math) to find the parameters `a` and `sigma` that **minimize** the value returned by the `objectiveFunction`. The code demonstrates this conceptually by showing that a "calibrated" set of parameters results in a lower error.

#### Next Steps & Extensions

* **Implement a Numerical Optimizer**: Integrate a library like NLopt to perform the minimization automatically.
* **Build Tree Pricers**: Implement binomial trees for the BDT and Black-Karasinski models.
* **Price Derivatives**: Extend the models to price interest rate caps, floors, and swaptions.
* **Use Real Market Data**: Connect the application to a data source to pull in real-time yield curves.

