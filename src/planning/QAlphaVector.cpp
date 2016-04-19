/* REPLACE_MADP_HEADER */
/* REPLACE_CONTRIBUTING_AUTHORS_START
 * Frans Oliehoek 
 * Matthijs Spaan 
 * REPLACE_CONTRIBUTING_AUTHORS_END
 */

#include "QAlphaVector.h"
#include "PlanningUnitDecPOMDPDiscrete.h"
#include "JointBeliefInterface.h"

using namespace std;

void QAlphaVector::Initialize()
{
    size_t nrS=GetPU()->GetNrStates();
    size_t nrA=GetPU()->GetNrJointActions();
    size_t h=GetPU()->GetHorizon();

#if 0 // this was code to convert alpha vectors coming from an infinite-horizon method

    ValueFunctionPOMDPDiscrete V=AlphaVectorPlanning::ImportValueFunction(_m_filename);
    cout << SoftPrint(V) << endl;
    QFunctionsDiscrete Q = AlphaVectorPlanning::ValueFunctionToQ(V,nrA,nrS);

    _m_Q.resize(h);
    for(Index t=0;t!=h;++t)
    {
        _m_Q.at(t).resize(nrA);
        for(Index a=0;a!=nrA;++a)
            for(Index i=0;i!=Q.at(a).size();++i)
            {
                AlphaVector alpha(nrS);
                alpha.SetAction(a);
                for(Index s=0;s!=nrS;++s)
                {
                    double value=Q.at(a).at(i).GetValue(s+(nrS*t));
                    alpha.SetValue(value,s);
                }
                _m_Q.at(t).at(a).push_back(alpha);
                cout << t << " " << a << " " << alpha.SoftPrint() << endl;
            }
    }

#else

    for(Index t=0;t!=h;++t)
    {
        stringstream ss;
        ss << _m_filename << "_t" << t;
        ValueFunctionPOMDPDiscrete V=AlphaVectorPlanning::ImportValueFunction(ss.str());
        _m_Q.push_back(AlphaVectorPlanning::ValueFunctionToQ(V,nrA,nrS));
    }

#endif
}
