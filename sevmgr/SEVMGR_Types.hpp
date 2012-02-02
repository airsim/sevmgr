#ifndef __SEVMGR_SEVMGR_TYPES_HPP
#define __SEVMGR_SEVMGR_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost
#include <boost/shared_ptr.hpp>
// Sevmgr
#include <sevmgr/SEVMGR_Exceptions.hpp>

namespace SEVMGR {

  // Forward declarations
  class SEVMGR_Service;

  // //////// Type definitions specific to to Sevmgr /////////
  /**
   * (Smart) Pointer on the SEvMgr service handler.
   */
  typedef boost::shared_ptr<SEVMGR_Service> SEVMGR_ServicePtr_T;

  /** Define an ID for an EventQueue object. */
  typedef std::string EventQueueID_T;
  
}
#endif // __SEVMGR_SEVMGR_TYPES_HPP

