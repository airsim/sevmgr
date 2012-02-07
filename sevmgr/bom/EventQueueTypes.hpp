// //////////////////////////////////////////////////////////////////////
#ifndef __SEVMGR_BOM_EVENTQUEUETYPES_HPP
#define __SEVMGR_BOM_EVENTQUEUETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace SEVMGR {

  // Forward declarations.
  class EventQueue;

  /** Define the EventQueue list. */
  typedef std::list<EventQueue*> EventQueueList_T;

  /** Define the EventQueue map. */
  typedef std::map<const stdair::MapKey_T, EventQueue*> EventQueueMap_T;
  
}
#endif // __SEVMGR_BOM_EVENTQUEUETYPES_HPP
