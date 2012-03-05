// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/basic/BasConst_Event.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// SEvMgr
#include <sevmgr/bom/EventQueue.hpp>
#include <sevmgr/command/EventQueueManager.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  void EventQueueManager::
  buildSampleQueue (stdair::STDAIR_ServicePtr_T lSTDAIR_ServicePtr,
		    EventQueue& ioEventQueue) { 

    // Total number of booking requests into the queue
    stdair::Count_T lNbOfBookingRequests (2);   
    addStatus(ioEventQueue, stdair::EventType::BKG_REQ, lNbOfBookingRequests);

    // Create a shared pointer on a first booking request
    // Date of the request (15-MAY-2011)
    const stdair::BookingRequestStruct& lBookingRequest =
      buildSampleBookingRequest (lSTDAIR_ServicePtr);
  
    // TODO: understand why the following form does not work, knowing
    // that:
    // typedef boost::shared_ptr<stdair::BookingRequestStruct> stdair::BookingRequestPtr_T
    // stdair::BookingRequestPtr_T oBookingRequest_ptr =
    //   boost::make_shared<stdair::BookingRequestStruct> (lInteractiveBookingRequest);
    const stdair::BookingRequestPtr_T lBookingRequest_ptr =
      stdair::BookingRequestPtr_T(new stdair::BookingRequestStruct(lBookingRequest));
  
    // Create an event structure
    stdair::EventStruct lEventStruct (stdair::EventType::BKG_REQ,
				      lBookingRequest_ptr);

    // Add the event into the queue
    addEvent(ioEventQueue, lEventStruct);

    // Create a second shared pointer on a second booking request
    // Date of the request (22-JAN-2010)
    const bool isForCRS = true;
    const stdair::BookingRequestStruct& lBookingRequestForCRS =
      buildSampleBookingRequest (lSTDAIR_ServicePtr, isForCRS);
    
    // TODO: understand why the following form does not work, knowing
    // that:
    // typedef boost::shared_ptr<stdair::BookingRequestStruct> stdair::BookingRequestPtr_T
    // stdair::BookingRequestPtr_T oBookingRequest_ptr =
    //   boost::make_shared<stdair::BookingRequestStruct> (lInteractiveBookingRequest);
    const stdair::BookingRequestPtr_T lBookingRequestForCRS_ptr =
      stdair::BookingRequestPtr_T(new stdair::BookingRequestStruct(lBookingRequestForCRS));
  
    // Create an event structure
    stdair::EventStruct lEventStructForCRS (stdair::EventType::BKG_REQ,
					    lBookingRequestForCRS_ptr);

    // Add the event into the queue
    addEvent(ioEventQueue, lEventStructForCRS); 

    // Total number of break points into the queue
    stdair::Count_T lNbOfBreakPoints (2);   
    addStatus(ioEventQueue, stdair::EventType::BRK_PT, lNbOfBreakPoints);

    // Create a shared pointer on a second break point
    // Date of the break point (21-JAN-2010)
    const stdair::Date_T lBP1Date (2010, boost::gregorian::Jan, 21);
    // Time of the break point (00:00)
    const stdair::Duration_T lBP1Time (0, 0, 0);
    // Date-time of the break point  (made of the date and time above)
    const stdair::DateTime_T lBP1DateTime (lBP1Date, lBP1Time);

    // TODO: understand why the form above does not work.
    const stdair::BreakPointPtr_T lBreakPoint1_ptr =
      stdair::BreakPointPtr_T(new stdair::BreakPointStruct(lBP1DateTime));
   
    // Create an event structure
    stdair::EventStruct lEventBreakPoint1 (stdair::EventType::BRK_PT,
					   lBreakPoint1_ptr);

    // Add the event into the queue
    addEvent(ioEventQueue, lEventBreakPoint1); 

    // Create a shared pointer on a second break point
    // Date of the break point (14-MAY-2011)
    const stdair::Date_T lBP2Date (2011, boost::gregorian::May, 14);
    // Time of the break point (00:00)
    const stdair::Duration_T lBP2Time (0, 0, 0);
    // Date-time of the break point  (made of the date and time above)
    const stdair::DateTime_T lBP2DateTime (lBP2Date, lBP2Time);

    // TODO: understand why the form above does not work.
    const stdair::BreakPointPtr_T lBreakPoint2_ptr =
      stdair::BreakPointPtr_T(new stdair::BreakPointStruct(lBP2DateTime));
   
    // Create an event structure
    stdair::EventStruct lEventBreakPoint2 (stdair::EventType::BRK_PT,
					   lBreakPoint2_ptr); 

    // Add the event into the queue
    addEvent(ioEventQueue, lEventBreakPoint2);

  } 

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct EventQueueManager::
  buildSampleBookingRequest(stdair::STDAIR_ServicePtr_T lSTDAIR_ServicePtr,
			    const bool isForCRS) {
    
    // Delegate the booking request building to the dedicated service
    stdair::BookingRequestStruct oBookingRequest =
      lSTDAIR_ServicePtr->buildSampleBookingRequest (isForCRS);

    return oBookingRequest;
  }


  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::reset (EventQueue& ioEventQueue) {

    /**
     * Reset the EventQueue object.
     */
    ioEventQueue.reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::addEvent (EventQueue& ioEventQueue,
                                     stdair::EventStruct& iEventStruct) {

    /**
     * Add the event to the event queue.
     */
    ioEventQueue.addEvent(iEventStruct);
  } 

  // //////////////////////////////////////////////////////////////////////
  const std::string EventQueueManager::
  list (const EventQueue& iEventQueue) {

    /**
     * Describe the event queue key
     */
    const std::string& lEventListStr = iEventQueue.list();

    //
    return lEventListStr;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string EventQueueManager::
  describeKey (const EventQueue& iEventQueue) {

    /**
     * Describe the event queue key
     */
    const std::string& lEventQueueKeyStr = iEventQueue.describeKey();

    //
    return lEventQueueKeyStr;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet EventQueueManager::
  popEvent (EventQueue& ioEventQueue,
            stdair::EventStruct& iEventStruct) {

    try {
      /**
       * Extract the first event from the queue
       */
      const stdair::ProgressStatusSet& lProgressStatusSet
        = ioEventQueue.popEvent (iEventStruct);
      
      // DEBUG 
      std::ostringstream oEventStr;
      oEventStr << "Poped event: '" 
		<< iEventStruct.describe() << "'."; 
      STDAIR_LOG_DEBUG (oEventStr.str());

      //
      return lProgressStatusSet;

    } catch (EventQueueException& lEventQueueException) {
      // DEBUG 
      std::ostringstream oErrorMessage; 
      oErrorMessage << "The event queue is empty: no event can be popped out.";   
      std::cerr << oErrorMessage.str() << std::endl;
      STDAIR_LOG_DEBUG(oErrorMessage.str());

    }

    // 
    return stdair::ProgressStatusSet(stdair::EventType::BKG_REQ);
  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::run (EventQueue& ioEventQueue,
                               stdair::EventStruct& iEventStruct) {

    // Default event type
    stdair::EventType::EN_EventType lEventType = stdair::EventType::BKG_REQ;  

    // While no break point has been encountered, keep on extracting events
    while (ioEventQueue.isQueueDone() == false
           && lEventType != stdair::EventType::BRK_PT) {
      /**
       * Extract the first event from the queue
       */
      ioEventQueue.popEvent (iEventStruct);
      lEventType = iEventStruct.getEventType();
      
    }

  }

  // ////////////////////////////////////////////////////////////////////
  bool EventQueueManager::select (EventQueue& ioEventQueue,
                                  stdair::EventStruct& iEventStruct,
                                  const stdair::DateTime_T& iDateTime) {

    // Search if an event has the given key
    const bool hasResearchBeenSuccessful =
      ioEventQueue.hasEventDateTime (iDateTime);

    // If no event has the given hey, return
    if (hasResearchBeenSuccessful == false) {
      return hasResearchBeenSuccessful;
    }
    assert (hasResearchBeenSuccessful == true);

    // Default date time
    stdair::DateTime_T lDateTime = stdair::DEFAULT_EVENT_OLDEST_DATETIME;

    // While the event with the given key has not been retrieved, keep on
    // extracting events
    while (ioEventQueue.isQueueDone() == false
           && lDateTime != iDateTime) {
      ioEventQueue.popEvent (iEventStruct);
      lDateTime = iEventStruct.getEventTime ();
           
    }

    assert (lDateTime == iDateTime);
    return hasResearchBeenSuccessful;

  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::
  updateStatus (EventQueue& ioEventQueue,
                const stdair::EventType::EN_EventType& iEventType,
                const stdair::Count_T& iEventCount) {
    
    /**
     * Update the status of the given event type with the given count
     */
    ioEventQueue.updateStatus (iEventType, iEventCount);
  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::
  addStatus (EventQueue& ioEventQueue,
             const stdair::EventType::EN_EventType& iEventType,
             const stdair::Count_T& iEventCount) {
    
    /**
     * Add the status of the given event type and initialize it
     * with the given count
     */
    ioEventQueue.addStatus (iEventType, iEventCount);
  }

  // ////////////////////////////////////////////////////////////////////
  bool EventQueueManager::
  isQueueDone (const EventQueue& iEventQueue) {
    
    /**
     * Check if the event queue is done
     */
    const bool isQueueDone = iEventQueue.isQueueDone();

    //
    return isQueueDone;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueueManager::
  getQueueSize (const EventQueue& iEventQueue) {

    /**
     * Calculates the size of the event queue
     */
    const stdair::Count_T& lQueueSize = iEventQueue.getQueueSize();

    //
    return lQueueSize;
  }  

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueueManager::
  getExpectedTotalNumberOfEventsToBeGenerated (const EventQueue& ioEventQueue) {

    /**
     * Calculate the expected total number of events with the given type
     */
    const stdair::Count_T& lExpectedTotalNumberOfEvents =
      ioEventQueue.getExpectedTotalNbOfEvents ();

    //
    return lExpectedTotalNumberOfEvents;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueueManager::
  getExpectedTotalNumberOfEventsToBeGenerated (const EventQueue& ioEventQueue,
                                               const stdair::EventType::EN_EventType& iEventType) {

    /**
     * Calculate the expected total number of events with the given type
     */
    const stdair::Count_T& lExpectedTotalNumberOfEvents =
      ioEventQueue.getExpectedTotalNbOfEvents (iEventType);

    //
    return lExpectedTotalNumberOfEvents;
  } 

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueueManager::
  getActualTotalNumberOfEventsToBeGenerated (const EventQueue& ioEventQueue) {

    /**
     * Calculate the actual total number of events with the given type
     */
    const stdair::Count_T& lActualTotalNumberOfEvents =
      ioEventQueue.getActualTotalNbOfEvents ();

    //
    return lActualTotalNumberOfEvents;

  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueueManager::
  getActualTotalNumberOfEventsToBeGenerated (const EventQueue& ioEventQueue,
                                             const stdair::EventType::EN_EventType& iEventType) {

    /**
     * Calculate the actual total number of events with the given type
     */
    const stdair::Count_T& lActualTotalNumberOfEvents =
      ioEventQueue.getActualTotalNbOfEvents (iEventType);

    //
    return lActualTotalNumberOfEvents;

  }

  //////////////////////////////////////////////////////////////////////   

  stdair::ProgressPercentage_T EventQueueManager::
  calculateProgress (const EventQueue& iEventQueue,
		     const stdair::EventType::EN_EventType& iEventType) { 

    /**
     * Calculate the progress status.
     */
    return iEventQueue.calculateProgress(iEventType);
    
  }
 
    /////////////////////////////////////////////////////////////////////   
  stdair::ProgressPercentage_T EventQueueManager::
  calculateProgress (const EventQueue& iEventQueue) {  

    /**
     * Calculate the progress status.
     */
    return iEventQueue.calculateProgress();

  }
 
}
