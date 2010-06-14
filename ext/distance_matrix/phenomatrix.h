#ifndef PHENOMATRIX_H_
# define PHENOMATRIX_H_

#include <string>
#include <boost/lexical_cast.hpp>
using std::string;
using boost::lexical_cast;

#include "phenomatrix_base.h"

class Phenomatrix : public PhenomatrixBase {
public:
    // id = predict species, given_id = source species
    Phenomatrix(uint id, uint given_id) : PhenomatrixBase(id), given_id_(given_id) {
        cerr << "Phenomatrix CONSTRUCTOR " << this << endl;
    }

    Phenomatrix(const Phenomatrix& rhs)
    : PhenomatrixBase(rhs), given_id_(rhs.given_id_)
    {
        cerr << "Phenomatrix COPY CONSTRUCTOR " << this << " from " << &rhs << endl;
    }

    ~Phenomatrix() { cerr << "Phenomatrix DTOR " << this << endl;}

    // Get the source matrix id
    uint source_id() const { return given_id_; }
protected:

    uint given_id_;
};

#endif
