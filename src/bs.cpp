#include <stdexcept>
#include <math.h>
#include <iostream>
#include "bs.h"
//#include <boost/math/distributions/normal.hpp>

double normalCDF(double x) // Phi(-∞, x) aka N(x)
{
    return std::erfc(-x / std::sqrt(2)) / 2;
}

double normalPDF(double x)
{
    static const float inv_sqrt_2pi = 0.3989422804014327;
    return inv_sqrt_2pi / std::exp(-0.5f * pow(x,2));
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
            /*
            Returns the Call option delta 
            */
            if ((volatility == 0) || (daysToExpiration == 0))
            {
    			return underlyingPrice > strikePrice ? 1 : 0.0;
            }
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return normalCDF(_d1_);
        }
        double Pricer::getPutDelta()
        {
            /*
            Returns the Put option delta
            */
            if ((volatility == 0) || (daysToExpiration == 0))
            {
    			return underlyingPrice < strikePrice ? -1 : 0.0;
            }
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return -normalCDF(-_d1_);
        }
        double Pricer::getCallDelta2()
        {
	        /*
            Returns the dual call delta
            */
            if ((volatility == 0) || (daysToExpiration == 0))
            {
                return  underlyingPrice < strikePrice? 1.0: 0.0;

            }
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return -normalCDF(_d2_) * exp(-(interestRate * daysToExpiration));
        }
        double Pricer::getPutDelta2()
        {
            /*
            Returns the dual put delta
            */
            if ((volatility == 0) || (daysToExpiration == 0))
            {
                return  underlyingPrice > strikePrice? -1.0: 0.0;

            }
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return normalCDF(-_d2_) * exp(-(interestRate * daysToExpiration));
        }
        double Pricer::getCallTheta()
        {
            /*
            Returns the call option theta
            */
            auto _b_ = exp(-(interestRate * daysToExpiration));
            auto call = -underlyingPrice * normalPDF(_d1_) * volatility / 
                    (2 * pow(daysToExpiration,0.5)) - interestRate * 
                    strikePrice * _b_ * normalCDF(_d2_);
            return call / 365;
        }
        double Pricer::getPutTheta()
        {
            /*
            Returns the call option theta
            */
            auto _b_ = exp(-(interestRate * daysToExpiration));
            auto put = -underlyingPrice * normalPDF(_d1_) * volatility / 
                    (2 * pow(daysToExpiration,0.5)) + interestRate * 
                    strikePrice * _b_ * normalCDF(-_d2_);
            return put / 365;
        }
        double Pricer::getCallRho()
        {
            /*
            Returns the call option rho
            */
            auto _b_ = exp(-(interestRate * daysToExpiration));
            return strikePrice * daysToExpiration * _b_ * 
                normalCDF(_d2_) / 100;
        }
        double Pricer::getPutRho()
        {
            /*
            Returns the put option rho
            */
            auto _b_ = exp(-(interestRate * daysToExpiration));
            return -strikePrice * daysToExpiration * _b_ * 
                normalCDF(-_d2_) / 100;
        }
        double Pricer::getVega()
        {
            /*
            Returns the option vega
            */
            if ((volatility == 0) || (daysToExpiration == 0))
                return 0.0;
            if (strikePrice == 0)
                throw std::overflow_error("The strike price cannot be zero");
            else
                return underlyingPrice * normalPDF(_d1_) * 
                        pow(daysToExpiration,0.5) / 100;
        }
        double Pricer::getGamma()
        {
            /*
            Returns the option gamma
            */
            return normalPDF(_d1_) / (underlyingPrice * _a_);
        }
    }
}


int main()
{
    auto pp = qob::bs::Pricer(100, 2, 3, 3, 200);
    std::cout << "Call BS price :" << pp.getCallPrice() << std::endl;
    std::cout << "Put BS price :" << pp.getPutPrice() << std::endl;
}

