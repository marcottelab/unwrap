#ifndef PHENOMATRIX_PAIR_H_
# define PHENOMATRIX_PAIR_H_

#ifdef RICE
#include <rice/Object.hpp>
#include <rice/Symbol.hpp>
#include <rice/Data_Type.hpp>
#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/Data_Object.hpp>
using Rice::Data_Object;
using Rice::Hash;
using Rice::Array;
#endif

#include <boost/foreach.hpp>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <iostream>
#include <stack>
#include <list>
using std::string;
using std::set;
using std::endl;
using std::map;
using std::set_intersection;
typedef std::set<uint> id_set;
using std::list;
using std::stack;
using std::cerr;
using std::endl;
#include "hypergeometric.h"
#include "phenomatrix.h"

#ifndef MATRIX_LIST_DEFINED
# define MATRIX_LIST_DEFINED
class PhenomatrixPair;
typedef std::list<PhenomatrixPair>          matrix_list;
#endif

// Handles creation of phenomatrices for pairs of species such that they have the
// correct number of rows.
//
// Also allows "temporary" removal of rows for the purpose of calculating
// distances and such (see push_mask and pop_mask).
class PhenomatrixPair {
public:
#ifdef RICE
    // Specialty constructor just for Ruby. Allows Ruby to pass in
    PhenomatrixPair(Rice::Object predict_or_id, Rice::Object source_or_id)
    : s(create_phenomatrix_list(source_or_id)),
      p(create_phenomatrix_stack(predict_or_id, s.back().id())),
      distance_function(switch_distance_function("hypergeometric"))
    {
        cerr << "PhenomatrixPair RUBY CONSTRUCTOR " << this << endl;
        cerr << "s id is " << s.back().id() << endl;
        cerr << "p id is " << p.top().id() << endl;
        cerr << "p source id is " << p.top().source_id() << endl;
    }
#endif
    PhenomatrixPair(uint id, PhenomatrixBase given_phenomatrix)
    : s(create_phenomatrix_list(given_phenomatrix)),
      p(create_phenomatrix_stack(id, given_phenomatrix.id())),
      distance_function(switch_distance_function("hypergeometric"))
    {
        cerr << "PhenomatrixPair (uint,Pb) CONSTRUCTOR " << this << endl;
        cerr << "s id is " << s.back().id() << endl;
        cerr << "p id is " << p.top().id() << endl;
        cerr << "p source id is " << p.top().source_id() << endl;
    }

    PhenomatrixPair(uint id, uint given_id)
    : s(create_phenomatrix_list(given_id)),            // source species matrix
      p(create_phenomatrix_stack(id, given_id)),       // predict species matrix
      distance_function(switch_distance_function("hypergeometric"))
    {
        cerr << "PhenomatrixPair CONSTRUCTOR " << this << endl;
        cerr << "s id is " << s.back().id() << endl;
        cerr << "p id is " << p.top().id() << endl;
        cerr << "p source id is " << p.top().source_id() << endl;
    }

    // Copy constructor
    PhenomatrixPair(const PhenomatrixPair& rhs)
    : s(rhs.s), p(rhs.p), distance_function(rhs.distance_function) {
        cerr << "PhenomatrixPair COPY CONSTRUCTOR " << this << " from " << &rhs << endl;
        cerr << "s id is " << s.back().id() << endl;
        cerr << "p id is " << p.top().id() << endl;
        cerr << "p source id is " << p.top().source_id() << endl;
    }
//
//    static stack<Phenomatrix> copy_stack(const stack<Phenomatrix>& rhs) {
//        cerr << "copy_stack " << &rhs << endl;
//        cerr << "size: " << rhs.size() << endl;
//        cerr << "top id: " << rhs.top().id() << endl;
//        cerr << "top source id: " << rhs.top().source_id() << endl;
//        return rhs;
//    }
//
//    static list<PhenomatrixBase> copy_list(const list<PhenomatrixBase>& rhs) {
//        cerr << "copy_list " << &rhs << endl;
//        cerr << "size: " << rhs.size() << endl;
//        cerr << "back id: " << rhs.back().id() << endl;
//        return rhs;
//    }


    ~PhenomatrixPair() { cerr << "PhenomatrixPair DTOR " << this << endl; }
    
    // Return the ID of the source matrix
    uint id() const {
        return s.back().id();
      }
    
    void push_mask(const std::set<uint>& m) {
        s.push_back(s.back());
        p.push(p.top());
    }
    
    bool pop_mask() {
        if (s.size() == 0) return false;
        s.pop_back();
        p.pop();
        return true;
    }


#ifdef RICE

    // Take a symbol, e.g., :hypergeometric, and use it to set the distance function.
    void set_distance_function(Rice::Object dfn) {
        distance_function = switch_distance_function( from_ruby<Rice::Symbol>(dfn).str() );
    }
#endif
    
    // Take a string, e.g., "hypergeometric", and use it to set the distance function.
    void set_distance_function_str(const string& dfn) {
        distance_function = switch_distance_function(dfn);
    }
protected:

    // Returns a function pointer to a distance function based on a request made via
    // a string.
    static double (*switch_distance_function(const std::string& distance_measure))(size_t,size_t,size_t,size_t) {
        std::map<std::string, double(*)(size_t,size_t,size_t,size_t)> choices;
        choices["hypergeometric"] = &hypergeometric;

        return choices[distance_measure];
    }

    // Create the predict matrix stack
    static stack<Phenomatrix> create_phenomatrix_stack(uint id, uint given_id) {
        stack<Phenomatrix> new_stack; new_stack.push(Phenomatrix(id, given_id));
        return new_stack;
    }

    // Create the source matrix stack (well, actually a list)
    static list<PhenomatrixBase> create_phenomatrix_list(uint id) {
        list<PhenomatrixBase> new_list; new_list.push_back(PhenomatrixBase(id));
        return new_list;
    }
    static list<PhenomatrixBase> create_phenomatrix_list(const PhenomatrixBase& pb) {
        list<PhenomatrixBase> new_list; new_list.push_back(pb);
        return new_list;
    }
    
#ifdef RICE
    static stack<Phenomatrix> create_phenomatrix_stack(Rice::Object predict_or_id, uint given_id) {
        if (predict_or_id.is_a( rb_cFixnum ))
            return create_phenomatrix_stack( from_ruby<uint>(predict_or_id), given_id );

        else if (predict_or_id.is_a( Rice::Data_Type<Phenomatrix>::klass() )) {
            stack<Phenomatrix> new_stack;
            new_stack.push( from_ruby<Phenomatrix>(predict_or_id) );

            // Check that the predict matrix has the source matrix as a given
            if (new_stack.top().source_id() != given_id)
                throw Rice::Exception(rb_eArgError, "phenomatrix_pair.h: create_phenomatrix_stack: Source matrix does not match the source id given to the predict matrix");

            return new_stack;

        } else
          throw Rice::Exception(rb_eArgError, "phenomatrix_pair.h: create_phenomatrix_stack: Argument must be a Phenomatrix");  
    }

    static list<PhenomatrixBase> create_phenomatrix_list(Rice::Object source_or_id) {
        if (source_or_id.is_a( rb_cFixnum ))
            return create_phenomatrix_list(from_ruby<uint>(source_or_id));

        else if (source_or_id.is_instance_of(Rice::Data_Type<PhenomatrixBase>::klass())) {
            list<PhenomatrixBase> new_list;
            new_list.push_back(from_ruby<PhenomatrixBase>(source_or_id));
            return new_list;
            
        } else
          throw Rice::Exception(rb_eArgError, "phenomatrix_pair.h: create_phenomatrix_list: Argument must be a PhenomatrixBase");
    }
#endif

    list<PhenomatrixBase> s;
    stack<Phenomatrix> p;

    // Allow different distance functions to be subbed in.
    double (*distance_function)(size_t, size_t, size_t, size_t);
};

#ifndef MATRIX_LIST_DEFINED
# define MATRIX_LIST_DEFINED
typedef std::list<PhenomatrixPair>          matrix_list;
#endif

#endif
