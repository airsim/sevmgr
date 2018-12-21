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
     * list the events with a given event type
     */
    static const std::string list (const EventQueue&,
				   const stdair::EventType::EN_EventType&);

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
     * 
     * Selected the event with the given date time, if such event existed.
     * 
     * @param EventQueue& Queue containing all the events.
     * @return stdair::EventStruct A copy of the event with the given date time.
     *   If no event with the given DateTime was encountered, no copy are
     *   returned.
     * @param const stdair::DateTime_T Date time of the event to be returned. 
     * @return bool States whether an event with the given date time had been
     *   encountered and thus returned.
     *
     * /Note All events occuring before the selected one are played.
     *   Thus, the copy returned is the copy of the current first event of
     *   the queue.
     */
    static bool select (EventQueue&, stdair::EventStruct&, const stdair::DateTime_T&); 
    
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
     * Check if the event queue has already a progress status for the given 
     * event type
     */
    static bool hasProgressStatus (const EventQueue&,
				   const stdair::EventType::EN_EventType&);

    /**
     * Check if the event queue is done
     */
    static bool isQueueDone (const EventQueue&);


    /**
     * Calculate whether the size of the event queue
     */
    static const stdair::Count_T getQueueSize(const EventQueue&); 

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
     * Get the overall progress status (for the whole event queue).
     */
    static const stdair::ProgressStatus& getStatus (const EventQueue&,
						    const stdair::EventType::EN_EventType&); 
    
    /**
     * Get the progress status for the given event type (e.g., booking
     * request, optimisation notification, schedule change, break point).
     */
    static const stdair::ProgressStatus& getStatus (const EventQueue&);

  };

}
#endif // __SEVMGR_CMD_EVENTQUEUEMANAGER_HPP
