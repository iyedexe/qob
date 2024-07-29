#ifndef QOB_H
#define QOB_H

namespace qob
{
    namespace bs
    {
        class Pricer
        {
            public:
            double underlyingPrice, strikePrice, interestRate, daysToExpiration;

            double getCallPrice();       //Returns the call price
            double getPutPrice();        //Returns the put price
            double getCallDelta();       //Returns the call delta
            double getPutDelta();        //Returns the put delta
            double getCallDelta2();      //Returns the call dual delta
            double getPutDelta2();       //Returns the put dual delta
            double getCallTheta();       //Returns the call theta
            double getPutTheta();        //Returns the put theta
            double getCallRho();         //Returns the call rho
            double getPutRho();      //Returns the put rho
            double getVega();        //Returns the option vega
            double getGamma();       //Returns the option gamma
        }
    }

}

#endif