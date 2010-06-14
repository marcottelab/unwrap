#ifndef PHENOMATRIX_H_
# define PHENOMATRIX_H_

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


class Phenomatrix {
public:

    PhenomatrixBase(uint id) : id_(id) {
        cerr << "Phenomatrix CONSTRUCTOR " << this << endl;
    }

    // Copy constructor
    Phenomatrix(const PhenomatrixBase& rhs) : id_(rhs.id_) {
        cerr << "Phenomatrix COPY CONSTRUCTOR " << this << " from " << &rhs << endl;
    }

    virtual ~Phenomatrix() { cerr << "Phenomatrix DTOR " << this << endl; }

    uint id() const { return id_; }
    
protected:
    uint id_;

};

#endif // PHENOMATRIX_H_
