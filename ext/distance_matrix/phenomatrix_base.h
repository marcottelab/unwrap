#ifndef PHENOMATRIX_BASE_H_
# define PHENOMATRIX_BASE_H_

typedef unsigned int uint;
#ifdef RICE
# include <rice/Object.hpp>
# include <rice/Data_Type.hpp>
# include <rice/Constructor.hpp>
#endif
#include <map>
#include <iostream>
using std::map;
using std::cerr;
using std::cout;
using std::endl;


class PhenomatrixBase {
public:

    PhenomatrixBase(uint id) : id_(id) {
        cerr << "PhenomatrixBase CONSTRUCTOR " << this << endl;
    }

    // Copy constructor
    PhenomatrixBase(const PhenomatrixBase& rhs) : id_(rhs.id_) {
        cerr << "PhenomatrixBase COPY CONSTRUCTOR " << this << " from " << &rhs << endl;
    }

    virtual ~PhenomatrixBase() { cerr << "PhenomatrixBase DTOR " << this << endl; }

    uint id() const { return id_; }
    
protected:
    uint id_;

};

#endif // PHENOMATRIX_BASE_H_
