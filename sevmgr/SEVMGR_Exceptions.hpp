#ifndef __SEVMGR_SEVMGR_EXCEPTIONS_HPP
#define __SEVMGR_SEVMGR_EXCEPTIONS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace SEVMGR {

  // ///////// Exceptions ///////////
  /**
   * Root exception for the Sevmgr component
   */
  class SEvMgrException : public stdair::RootException {
  public:
    /**
     * Constructor.
     */
    SEvMgrException (const std::string& iWhat)
      : stdair::RootException (iWhat) {}
  };

}
#endif // __SEVMGR_SEVMGR_EXCEPTIONS_HPP

