// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/bom/BomChildrenHolderImp.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/LegCabinMap.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  LegCabinMap_T::
  LegCabinMap_T (const LegCabinHolder_T& iLegCabinHolder)
    : _legCabinHolder (iLegCabinHolder) {
  }

  // ////////////////////////////////////////////////////////////////////
  LegCabinMap_T::
  LegCabinMap_T (const LegCabinMap_T& iLCMap)
    : _legCabinHolder (iLCMap._legCabinHolder) {
  }

  // ////////////////////////////////////////////////////////////////////
  LegCabinMap_T::~LegCabinMap_T () {
  }

  // //////////////////////////////////////////////////////////////////////
  LegCabinMap_T::iterator LegCabinMap_T::begin () const {
    return _legCabinHolder.mapBegin ();
  }

  // //////////////////////////////////////////////////////////////////////
  LegCabinMap_T::iterator LegCabinMap_T::end () const {
    return _legCabinHolder.mapEnd ();
  }

  // //////////////////////////////////////////////////////////////////////
  LegCabinMap_T::reverse_iterator LegCabinMap_T::rbegin () const {
    return _legCabinHolder.mapRBegin ();
  }

  // //////////////////////////////////////////////////////////////////////
  LegCabinMap_T::reverse_iterator LegCabinMap_T::rend () const {
    return _legCabinHolder.mapREnd ();
  }

}

