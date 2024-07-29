#include <stdexcept>
#include <math.h>
#include <iostream>
#include "bs.h"

double normalCDF(double x) // Phi(-∞, x) aka N(x)
{
    return std::erfc(-x / std::sqrt(2)) / 2;
}
 
namespace qob
{
    namespace bs
    {
        Pricer::Pricer(double aUnderlyingPrice ,double aStrikePrice ,double aInterestRate ,double aDaysToExpiration, double aVolatility)
        {
            underlyingPrice = aUnderlyingPrice;
            strikePrice = aStrikePrice;
            interestRate = aInterestRate / 100;
            daysToExpiration = aDaysToExpiration / 365;
            volatility = aVolatility / 100;

            _a_ = volatility * std::pow(daysToExpiration, 0.5);
            _d1_ = (log(underlyingPrice / strikePrice) + (interestRate + (std::pow(volatility,2)) / 2) * daysToExpiration) / _a_;
            _d2_ = _d1_ - _a_;

        }
        
        double Pricer::getCallPrice()
        {
            /*
            Returns the call option price
            */
            if ((volatility == 0) || (daysToExpiration == 0))
                return std::max(0.0, underlyingPrice - strikePrice);
            
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return underlyingPrice * normalCDF(_d1_) - strikePrice * exp(-interestRate * daysToExpiration) * normalCDF(_d2_);
        }

        double Pricer::getPutPrice()
        {
            /*
            Returns the put option price
            */
            double put;
            if ((volatility == 0) || (daysToExpiration == 0))
                return std::max(0.0, strikePrice - underlyingPrice);
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
			    return strikePrice * exp(-1 * interestRate * daysToExpiration) * normalCDF(-1 * _d2_) - underlyingPrice * normalCDF(-_d1_);
        }

        double Pricer::getCallDelta()
        {
            return 0.0;
        }
        double Pricer::getPutDelta()
        {
            return 0.0;
        }
        double Pricer::getCallDelta2()
        {
            return 0.0;
        }
        double Pricer::getPutDelta2()
        {
            return 0.0;
        }
        double Pricer::getCallTheta()
        {
            return 0.0;
        }
        double Pricer::getPutTheta()
        {
            return 0.0;
        }
        double Pricer::getCallRho()
        {
            return 0.0;
        }
        double Pricer::getPutRho()
        {
            return 0.0;
        }
        double Pricer::getVega()
        {
            return 0.0;
        }
        double Pricer::getGamma()
        {
            return 0.0;
        }
    }
}


int main()
{
    auto pp = qob::bs::Pricer(100, 2, 3, 3, 200);
    std::cout << "Call BS price :" << pp.getCallPrice() << std::endl;
    std::cout << "Put BS price :" << pp.getPutPrice() << std::endl;
}

