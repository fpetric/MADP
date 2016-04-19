/* REPLACE_MADP_HEADER */
/* REPLACE_CONTRIBUTING_AUTHORS_START
 * Frans Oliehoek 
 * Matthijs Spaan 
 * REPLACE_CONTRIBUTING_AUTHORS_END
 */

#include "ActionObservationHistory.h"
#include "Action.h"
#include "Observation.h"
#include "PlanningUnitMADPDiscrete.h"
#include "ActionHistory.h"
#include "ObservationHistoryTree.h"

using namespace std;

#define DEBUG_AOH 0

//Default constructor
ActionObservationHistory::ActionObservationHistory(PlanningUnitMADPDiscrete& pu, Index agentI) :
    Referrer<PlanningUnitMADPDiscrete>(pu),
    IndividualHistory(agentI)
{    
    SetLength(0);
    _m_pred = 0;
    _m_ahI = 0; //HARDCODED
    _m_ohI = 0; //HARDCODED
}
ActionObservationHistory::ActionObservationHistory(Index aI, Index oI, 
                ActionObservationHistory* pred) :
    Referrer<PlanningUnitMADPDiscrete>(pred->GetReferred()),
    IndividualHistory(pred->_m_agentI)
{
    SetLength(pred->GetLength() + 1);
    _m_pred = pred;
    _m_ahI = 
        CastLIndexToIndex(GetReferred()->GetActionHistoryTree(_m_agentI, pred->_m_ahI) //=aht
                          ->GetSuccessor(aI) //=aht'
                          ->GetIndex()); //=ahI'
    _m_ohI = 
       CastLIndexToIndex(GetReferred()->GetObservationHistoryTree(_m_agentI, pred->_m_ohI) //=oht
                         ->GetSuccessor(oI) //=oht'
                         ->GetIndex()); //=ohI'
}

string ActionObservationHistory::SoftPrint() const
{
    stringstream ss;
    if(_m_pred != 0)
    {
        if(_m_length >= 1)
            ss << _m_pred->SoftPrint();
        else
        {
            ss << "ObservationHistory:Print() - Warning:_m_pred != "<<
            "null, but lenght < 1 !"<<endl;
            throw E(ss);
        }
    }

    ActionHistory* ah = GetReferred()->GetActionHistoryTree(
            _m_agentI, _m_ahI) //=aht
        ->GetContainedElement(); //=ah
    if (!ah->IsEmpty()) // don't print the empty observation
    {
        ss << ", ";
        Index aI = ah->GetLastActionIndex(); // aI
        ss << GetReferred()->GetAction(_m_agentI, aI)->SoftPrintBrief();
    }
    else
        ss << "EMPTY_AH";

    ss << ", ";

    ObservationHistory* oh = GetReferred()->GetObservationHistoryTree(
            _m_agentI, _m_ohI) //=oht
        ->GetContainedElement(); //=oh
    if (!oh->ContainsEmptyOI()) // don't print the empty observation
    {
        ss << ", ";
        Index oI = oh->GetLastObservationIndex(); // oI
        ss << GetReferred()->GetObservation(_m_agentI, oI)->SoftPrintBrief();
    }
    else
        ss << "Oempty";

    return(ss.str());
}
