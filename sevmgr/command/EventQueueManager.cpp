// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/service/Logger.hpp>
// SEvMgr
#include <sevmgr/bom/EventQueue.hpp>
#include <sevmgr/command/EventQueueManager.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  void EventQueueManager::
  buildSampleBom (EventQueue& ioEventQueue) {
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
  describeKey(const EventQueue& iEventQueue) {

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

    /**
     * Extract the first event from the queue
     */
    const stdair::ProgressStatusSet& lProgressStatusSet =
      ioEventQueue.popEvent (iEventStruct);

    //
    return lProgressStatusSet;
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
  void EventQueueManager::
  setActualTotalNbOfEvents (EventQueue& iEventQueue,
                            const stdair::Count_T& iActualTotalNbOfEvents) {

    /**
     * Calculate the actual total number of events in the queue
     */
    iEventQueue.setActualTotalNbOfEvents (iActualTotalNbOfEvents);

  }
 
}
