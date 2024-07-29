namespace qob
{
    namespace bs
    {
        class Pricer
        {
            /*
            Black-Scholes
            Used for pricing European options on stocks without dividends
            */
            private:
            double _a_, _d1_, _d2_;
            public:
            Pricer(double underlyingPrice ,double strikePrice ,double interestRate ,double daysToExpiration, double volatility);
            
            double underlyingPrice, strikePrice, interestRate, daysToExpiration, volatility;

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
        };
    }
}