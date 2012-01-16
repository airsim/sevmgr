#ifndef __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
#define __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>

// Forward declarations
namespace stdair {
  class EventQueue;
  struct ProgressStatusSet;
}

namespace SEVMGR {

  /**
   * @brief Utility class for Demand and DemandStream objects.
   */
  class EventQueueManager : public stdair::CmdAbstract {
    friend class SEVMGR_Service;
    
  private:
    // //////// Business methodes //////////
    /**
     * Generate a sample BOM tree.
     */
    static void buildSampleBom (stdair::EventQueue&);

    /**
     * Empty the event queue.
     *
     * @param stdair::EventQueue& Reference on the top of the BOM tree.
     */
    static void reset (stdair::EventQueue&);

    /**
     * Add an event the event queue.
     *
     * @param stdair::EventQueue& Reference on the top of the BOM tree.
     * @param stdair::EventStruct& Reference on the event to add to the queue.
     */
    static void addEvent (stdair::EventQueue&, stdair::EventStruct&);
  };

}
#endif // __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
