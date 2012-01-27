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
     */
    static void reset (stdair::EventQueue&);

    /**
     * Add an event the event queue.
     */
    static void addEvent (stdair::EventQueue&, stdair::EventStruct&);

    /**
     * Describe the event queue key
     */
    static const std::string describeKey(const stdair::EventQueue&);

    /**
     * Extract the first event fron the queue
     */
    static stdair::ProgressStatusSet popEvent (stdair::EventQueue&,
                                               stdair::EventStruct&);
    /**
     * Update the status of the given event type with the given count
     */
    static void updateStatus (stdair::EventQueue&,
                              const stdair::EventType::EN_EventType&,
                              const stdair::Count_T&);
     /**
     * Add the status of the given event type and initialize it
     * with the given count
     */
    static void addStatus (stdair::EventQueue&,
                           const stdair::EventType::EN_EventType&,
                           const stdair::Count_T&);

    /**
     * Check if the event queue is done
     */
    static bool isQueueDone(const stdair::EventQueue&);


    /**
     * Calculateswhether the size of the event queue
     */
    static const stdair::Count_T& getQueueSize(const stdair::EventQueue&); 

    /**
     * Calculate the expected total number of events
     */
    static const stdair::Count_T& 
    getExpectedTotalNumberOfEventsToBeGenerated(const stdair::EventQueue&);

    /**
     * Calculate the expected total number of events with the given type
     */
    static const stdair::Count_T&
    getExpectedTotalNumberOfEventsToBeGenerated(const stdair::EventQueue&,
                                                const stdair::EventType::EN_EventType&); 
     
    /**
     * Calculate the actual total number of events
     */
    static const stdair::Count_T&
    getActualTotalNumberOfEventsToBeGenerated(const stdair::EventQueue&);

    /**
     * Calculate the actual total number of events with the given type
     */
    static const stdair::Count_T&
    getActualTotalNumberOfEventsToBeGenerated(const stdair::EventQueue&,
                                              const stdair::EventType::EN_EventType&);  
  
    /**
     * Set the actual total number of events in the queue
     */
    static void setActualTotalNbOfEvents (stdair::EventQueue&,
                                          const stdair::Count_T& iActualTotalNbOfEvents);

  };

}
#endif // __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
