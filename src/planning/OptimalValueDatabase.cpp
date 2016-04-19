/* REPLACE_MADP_HEADER */
/* REPLACE_CONTRIBUTING_AUTHORS_START
 * Frans Oliehoek 
 * Matthijs Spaan 
 * REPLACE_CONTRIBUTING_AUTHORS_END
 */

#include "OptimalValueDatabase.h"
#include "PlanningUnitDecPOMDPDiscrete.h"
#include "directories.h"
#include <fstream>

using namespace std;

//Default constructor
OptimalValueDatabase::OptimalValueDatabase(
    const PlanningUnitDecPOMDPDiscrete* pu) :
    _m_pu(pu)
{
    Load();
//     double discount=1.0;
//     AddEntry("DecTiger", discount, 2, -4.000000);
//     AddEntry("DecTiger", discount, 3, 5.190812);
//     AddEntry("DecTiger", discount, 4, 4.802755);
//     AddEntry("DecTiger", discount, 5, 7.026451);
//     AddEntry("dectiger", discount, 2, -4.000000);
//     AddEntry("dectiger", discount, 3, 5.190812);
//     AddEntry("dectiger", discount, 4, 4.802755);
//     AddEntry("dectiger", discount, 5, 7.026451);
}

void OptimalValueDatabase::AddEntry(const std::string &problemName,
                                    double discount,
                                    size_t horizon,
                                    double value)
{
    nameDiscountT pairNameDiscount(problemName,discount);
    nameDiscountHorizonT nameDiscountHorizon(pairNameDiscount,
                                             horizon);
    _m_optimalValues.insert(
        pair<nameDiscountHorizonT, double>(nameDiscountHorizon,value));
}

void OptimalValueDatabase::SetOptimalValue(double value)
{
    AddEntry(_m_pu->GetDPOMDPD()->GetUnixName(),
             _m_pu->GetDiscount(),
             _m_pu->GetHorizon(),
             value);
    Save();
}

double OptimalValueDatabase::GetEntry(const std::string &problemName,
                                      double discount,
                                      size_t horizon) const
{
    nameDiscountT pairNameDiscount(problemName,discount);
    nameDiscountHorizonT nameDiscountHorizon(pairNameDiscount,
                                             horizon);

    map<nameDiscountHorizonT,double>::const_iterator iter=
        _m_optimalValues.find(nameDiscountHorizon);
    if( iter != _m_optimalValues.end() )
        return(iter->second);
    else
    {
        stringstream ss;
        ss << "OptimalValueDatabase::GetEntry() no entry exists for "
           << problemName << " discount " << discount << " horizon "
           << horizon;
        throw(E(ss));
    }

    // should not get here
    return(0);
}
double
OptimalValueDatabase::GetOptimalValue() const
{
    return(GetEntry(_m_pu->GetDPOMDPD()->GetUnixName(),
                    _m_pu->GetDiscount(),
                    _m_pu->GetHorizon()));
}

bool OptimalValueDatabase::IsOptimal(const std::string &problemName,
                                     double discount,
                                     size_t horizon,
                                     double value) const
{
    if(EqualReward(value,
                   GetEntry(problemName,discount,horizon)))
        return(true);
    else
        return(false);
}
bool OptimalValueDatabase::IsOptimal(double value) const
{
    return(IsOptimal(_m_pu->GetDPOMDPD()->GetUnixName(),
                     _m_pu->GetDiscount(),
                     _m_pu->GetHorizon(),
                     value));
}


bool OptimalValueDatabase::IsInDatabase(const std::string &problemName,
                                        double discount,
                                        size_t horizon) const
{
    nameDiscountT pairNameDiscount(problemName,discount);
    nameDiscountHorizonT nameDiscountHorizon(pairNameDiscount,
                                             horizon);

    map<nameDiscountHorizonT,double>::const_iterator iter=
        _m_optimalValues.find(nameDiscountHorizon);
    if( iter == _m_optimalValues.end() )
        return(false);
    else
        return(true);
}
bool 
OptimalValueDatabase::IsInDatabase() const
{
    return(IsInDatabase(_m_pu->GetDPOMDPD()->GetUnixName(),
                        _m_pu->GetDiscount(),
                        _m_pu->GetHorizon()));
}

string OptimalValueDatabase::SoftPrint() const
{
    stringstream ss;
    ss.precision(16);

    map<nameDiscountHorizonT,double>::const_iterator iter=
        _m_optimalValues.begin();
    while( iter != _m_optimalValues.end() )
    {
        double value=iter->second;
        nameDiscountHorizonT x=iter->first;
        size_t horizon=x.second;
        nameDiscountT y=x.first;
        string problemName=y.first;
        double discount=y.second;
        ss << problemName << " " << discount << " "
           << horizon << " " << value << endl;

        ++iter;
    }

    return(ss.str());
}

void OptimalValueDatabase::Load()
{
    _m_optimalValues.clear();
    stringstream filename;
    filename << directories::MADPGetResultsDir()
             << "/GMAA/optimalValueDatabase";
    ifstream fp(filename.str().c_str());
    if(!fp)
        return;

    string problemName;
    double discount, value;
    size_t horizon;
    bool first=true;
    string buffer;
    while(!getline(fp,buffer).eof())
    {
        // first line is comment
        if(first)
        {
            first=false;
            continue;
        }
        istringstream is(buffer);
        is >> problemName;
        is >> discount;
        is >> horizon;
        is >> value;

        AddEntry(problemName,discount,horizon,value);
    }
}

void OptimalValueDatabase::Save() const
{
    stringstream filename;
    filename << directories::MADPGetResultsDir()
             << "/GMAA/optimalValueDatabase";

    ofstream fp(filename.str().c_str());
    if(!fp)
    {
        stringstream ss;
        ss << "OptimalValueDatabase::Save() failed to open file "
           << filename.str() << endl;
        throw E(ss.str());
    }

    fp.precision(16);
    
    fp << "problem discount horizon value" << endl;
    fp << SoftPrint();
}
