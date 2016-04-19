/* REPLACE_MADP_HEADER */
/* REPLACE_CONTRIBUTING_AUTHORS_START
 * Frans Oliehoek 
 * Matthijs Spaan 
 * REPLACE_CONTRIBUTING_AUTHORS_END
 */

/* Only include this header file once. */
#ifndef _JOINTPOLICYVALUEPAIR_H_
#define _JOINTPOLICYVALUEPAIR_H_ 1

/* the include directives */
#include <iostream>
#include "Globals.h"
#include "PolicyPoolItemInterface.h"
#include "boost/shared_ptr.hpp"

class JointPolicyValuePair;
typedef boost::shared_ptr<JointPolicyValuePair> JointPolicyValuePair_sharedPtr;

/**\brief JointPolicyValuePair is a wrapper for a partial joint policy
 * and its heuristic value.
 *
 * JointPolicyValuePair is a class that represents a (joint
 * *policy,value) pair the operator less is defined for these pairs so
 * *they can be but in ordered containers.
 *
 * JointPolicyValuePair is a PolicyPoolItem (i.e., it implements
 * PolicyPoolItemInterface)
 */
class JointPolicyValuePair 
    : public PolicyPoolItemInterface                    
{   
    private:
        double _m_val;

    protected:
    public:
        /// (default) Constructor
        JointPolicyValuePair(double val)
        {
            _m_val = val;
        }                

        /// Destructor.
        ~JointPolicyValuePair(){};

        /// Copy assignment operator
        JointPolicyValuePair& operator= (const JointPolicyValuePair& o)
        {
            throw E("Assigning JointPolicyValuePair - but operator= not def'd");
        }

        double GetValue() const
            {return(_m_val);}   

};

namespace std{
    /**\brief Overload the less<Type> template for JointPolicyValuePair* (we want less
     * to give an ordering according to values, not addresses...).*/
    template <> 
    struct less< JointPolicyValuePair* > //struct, so operator() is public by def. 
    {
        bool operator()(const JointPolicyValuePair* x, const JointPolicyValuePair* y) const
        { 
            //cout << "specialized less<JointPolicyValuePair> called!"<<endl;
            return( x->GetValue() < y->GetValue() );
        }

    };
    template <> 
    struct less< JointPolicyValuePair_sharedPtr > //struct, so operator() is public by def. 
    {
        bool operator()(const JointPolicyValuePair_sharedPtr x,
                        const JointPolicyValuePair_sharedPtr y) const
        { 
            //cout << "specialized less<JointPolicyValuePair> called!"<<endl;
            return( x->GetValue() < y->GetValue() );
        }

    };
}

#endif /* !_JPOLVALPAIR_H_ */

// Local Variables: ***
// mode:c++ ***
// End: ***
