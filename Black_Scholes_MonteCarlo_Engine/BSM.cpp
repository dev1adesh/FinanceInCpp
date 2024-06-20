#include "BSM.h"
#include <math.h>

using namespace std;

BSM::BSM(float ass,
         float strk,
         float grwth,
         float volty,
         float yrs,
         long steps,
         long sims)
{
    bsmAsset = ass;
    bsmStrike = strk;
    bsmGrowth = grwth;
    bsmVol = volty;
    bsmYears = yrs;
    bsmSteps = steps;
    bsmMonteCarloSims = sims;
}

BSM::~BSM(){}

// S{PHI}(d1) - Ke-rT{PHI}(d2)

// Asset price 100, Call option, strike price 110

// 100->101->99->110->125  (15)

// 100->98->97->54->109    (0)

// 100->97->112->116->116  (6) +
//                        ----
//                        21/3 => 7

void BSM::logNormalRandomWalk(){
    
    srand((unsigned)time(0));
    
    double callPayoffPot = 0.0;
    double putPayoffPot = 0.0;
    
    double timeStep = getBsmYears()/getBsmSteps();
    double sqrtTs = sqrt(timeStep);
    
    for(long i = 1; i <= getBsmMonteCarloSims(); i++)
    {
        double ass = getBsmAsset();
        
        for(int j = 1; j <= getBsmSteps(); j++)
        {
            ass = ass * (1 + getBsmGrowth() * timeStep +
                         getBsmVol() * sqrtTs * (
                        rn() + rn() + rn() +
                        rn() + rn() + rn() +
                        rn() + rn() + rn() +
                        rn() + rn() + rn() - 6));
        }
        
        // cout << "Final ass, this run: " << ass << endl;
        
        if (ass > getBsmStrike()){
            
            callPayoffPot += (ass - getBsmStrike());
        }
        else if (ass < getBsmStrike()){
            
            putPayoffPot += (getBsmStrike() - ass);
        }
        
        // cout << "callPayoffPot: " << callPayoffPot << endl;
        // cout << "putPayoffPot: " << putPayoffPot << endl;
        
        if((i % 10000) == 0){
            cout << "." << flush;
            
            if((i % 100000) == 0){
                
                long iMess = i/1000;
                
                cout << iMess << "k" << flush;
            }
        }
    }
    
    bsmCallPrice = callPayoffPot / getBsmMonteCarloSims();
    bsmPutPrice = putPayoffPot / getBsmMonteCarloSims();
    
    return;
}

double BSM::rn(){
    return (double)rand()/(double)(RAND_MAX + 1.0);
}

float BSM::getBsmAsset(){
    return bsmAsset;
}

float BSM::getBsmStrike(){
    return bsmStrike;
}

float BSM::getBsmGrowth(){
    return bsmGrowth;
}

float BSM::getBsmVol(){
    return bsmVol;
}

float BSM::getBsmYears(){
    return bsmYears;
}

long BSM::getBsmSteps(){
    return bsmSteps;
}

long BSM::getBsmMonteCarloSims(){
    return bsmMonteCarloSims;
}

double BSM::getCallPrice(){
    return bsmCallPrice;
}

double BSM::getPutPrice(){
    return bsmPutPrice;
}
