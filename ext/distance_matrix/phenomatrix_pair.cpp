#include "phenomatrix_pair.h"

#ifdef RICE
#include "ruby.h"


using namespace Rice;

Data_Type<PhenomatrixPair> phenomatrix_pair_type;

//template<>
//PhenomatrixPair from_ruby<PhenomatrixPair>(Rice::Object x) {
//    Rice::Data_Object<PhenomatrixPair> d(x, phenomatrix_pair_type);
//    return *d;
//}

template <>
Object to_ruby<set<uint> >(set<uint> const & d) {
    Array ary;
    for (set<uint>::const_iterator i = d.begin(); i != d.end(); ++i)
        ary.push( to_ruby<uint>(*i) );
    return ary;
}


// Convert from Rice::Array to std::set
template <>
set<uint> from_ruby<set<uint> >(Object x) {
    Array ary(x);
    set<uint> result;
    for (Array::iterator i = ary.begin(); i != ary.end(); ++i)
        result.insert(from_ruby<uint>(*i));
    return result;
}


template<>
Rice::Object to_ruby<map<uint, set<uint> > >(map<uint,set<uint> > const & d) {
    Hash h;
    for (map<uint,set<uint> >::const_iterator dt = d.begin(); dt != d.end(); ++dt)
        h[ to_ruby<uint>(dt->first) ] = to_ruby<set<uint> >(dt->second);
    return h;
}


// IT IS CRITICAL THAT Rice:: TYPES LEAVE OFF THE NAMESPACE BEYOND THIS POINT!
// Remember, this is extern "C", and C doesn't understand namespaces!
// If you don't leave off the namespace, you'll get bizarre errors like "Can't convert from Hash to Rice::Hash."
extern "C"
void Init_distance_matrix() {

    Rice::Module rb_mFastknn = define_module("Fastknn");

    Data_Type<Phenomatrix> rb_cPhenomatrix =
            define_class_under<Phenomatrix>(rb_mFastknn, "Phenomatrix")
            .define_constructor(Constructor<Phenomatrix,uint>())
            .define_method("id", &Phenomatrix::id);

    Data_Type<PhenomatrixPair> rb_cPhenomatrixPair =
            define_class_under<PhenomatrixPair>(rb_mFastknn, "PhenomatrixPair")
            .define_constructor(Constructor<PhenomatrixPair, Object, Object>())
            .define_method("push_mask", &PhenomatrixPair::push_mask)
            .define_method("pop_mask", &PhenomatrixPair::pop_mask);
}

#endif
