#ifndef __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
#define __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
#include <stdair/stdair_service_types.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>

// Forward declarations
namespace stdair {
  struct ProgressStatusSet;
}

namespace SEVMGR {

  // Forward declarations
  class EventQueue;

  /**
   * @brief Utility class for Demand and DemandStream objects.
   */
  class EventQueueManager : public stdair::CmdAbstract {
    friend class SEVMGR_Service;
    
  private:
    // //////// Business methodes //////////
    /**
     * Generate a sample event queue.
     */
    static void buildSampleQueue (stdair::STDAIR_ServicePtr_T,
				  EventQueue&); 

    /**
     * Build a sample booking request.
     */
    static stdair::BookingRequestStruct buildSampleBookingRequest(stdair::STDAIR_ServicePtr_T,
								  const bool isForCRS = false);
    
    /**
     * Empty the event queue.
     */
    static void reset (EventQueue&);

    /**
     * Add an event the event queue.
     */
    static void addEvent (EventQueue&, stdair::EventStruct&);

    /**
     * Describe the event queue key
     */
    static const std::string describeKey (const EventQueue&);   

    /**
     * Describe the event list
     */
    static const std::string list (const EventQueue&);

    /**
     * Extract the first event fron the queue
     */
    static stdair::ProgressStatusSet popEvent (EventQueue&,
                                               stdair::EventStruct&);

    /**
     * Played all events and stopped when the first break point was encountered
     */
    static void run (EventQueue&, stdair::EventStruct&);
    /**
     * Update the status of the given event type with the given count
     */
    static void updateStatus (EventQueue&,
                              const stdair::EventType::EN_EventType&,
                              const stdair::Count_T&);
     /**
     * Add the status of the given event type and initialize it
     * with the given count
     */
    static void addStatus (EventQueue&,
                           const stdair::EventType::EN_EventType&,
                           const stdair::Count_T&);

    /**
     * Check if the event queue is done
     */
    static bool isQueueDone (const EventQueue&);


    /**
     * Calculateswhether the size of the event queue
     */
    static const stdair::Count_T& getQueueSize(const EventQueue&); 

    /**
     * Calculate the expected total number of events
     */
    static const stdair::Count_T& 
    getExpectedTotalNumberOfEventsToBeGenerated(const EventQueue&);

    /**
     * Calculate the expected total number of events with the given type
     */
    static const stdair::Count_T&
    getExpectedTotalNumberOfEventsToBeGenerated(const EventQueue&,
                                                const stdair::EventType::EN_EventType&); 
     
    /**
     * Calculate the actual total number of events
     */
    static const stdair::Count_T&
    getActualTotalNumberOfEventsToBeGenerated(const EventQueue&);

    /**
     * Calculate the actual total number of events with the given type
     */
    static const stdair::Count_T&
    getActualTotalNumberOfEventsToBeGenerated(const EventQueue&,
                                              const stdair::EventType::EN_EventType&);  
  
    /**
     * Set the actual total number of events in the queue
     */
    static void setActualTotalNbOfEvents (EventQueue&,
                                          const stdair::Count_T& iActualTotalNbOfEvents);

  };

}
#endif // __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
