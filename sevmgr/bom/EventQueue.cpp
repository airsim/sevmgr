// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasConst_Event.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/service/Logger.hpp>
// SEvMgr
#include <sevmgr/basic/BasConst_EventQueueManager.hpp>
#include <sevmgr/bom/EventQueue.hpp>

namespace SEVMGR {
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::EventQueue()
    : _key (DEFAULT_EVENT_QUEUE_ID), _parent (NULL),
      _progressStatus (stdair::DEFAULT_PROGRESS_STATUS,
                       stdair::DEFAULT_PROGRESS_STATUS) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::EventQueue (const Key_T& iKey)
    : _key (iKey), _parent (NULL),
      _progressStatus (stdair::DEFAULT_PROGRESS_STATUS,
                       stdair::DEFAULT_PROGRESS_STATUS) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::EventQueue (const EventQueue& iEventQueue)
    : _key (DEFAULT_EVENT_QUEUE_ID), _parent (NULL),
      _progressStatus (stdair::DEFAULT_PROGRESS_STATUS,
                       stdair::DEFAULT_PROGRESS_STATUS) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::~EventQueue() {
    _eventList.clear();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string EventQueue::toString() const {
    std::ostringstream oStr;
    oStr << "(" << _eventList.size() << ") "
         << _progressStatus.getCurrentNb() << "/{"
         << _progressStatus.getExpectedNb() << ","
         << _progressStatus.getActualNb() << "}";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string EventQueue::display() const {
    std::ostringstream oStr;

    oStr << toString();

    return oStr.str();
  } 

  // //////////////////////////////////////////////////////////////////////
  std::string EventQueue::list () const {
    std::ostringstream oStr;  
    oStr << describeKey () << "\n" 
	 << toString() << "\n";

    // Browse the events
    for (stdair::EventList_T::const_iterator itEvent = _eventList.begin();
	 itEvent != _eventList.end(); ++itEvent) {
      const stdair::EventListElement_T* lEventListElement_ptr = &(*itEvent);
      assert (lEventListElement_ptr != NULL);
      const stdair::EventListElement_T& lEventListElement = 
	*lEventListElement_ptr;
      const stdair::EventStruct& lEvent = lEventListElement.second;
 
      // Delegate the JSON export to the dedicated service
      oStr << lEvent.describe() << "\n";
    }
    
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Count_T EventQueue::getQueueSize () const {
    return _eventList.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool EventQueue::isQueueEmpty () const {
    return _eventList.empty();
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool EventQueue::isQueueDone () const {
    const bool isQueueEmpty = _eventList.empty();
    return isQueueEmpty;
  }

  // //////////////////////////////////////////////////////////////////////
  void EventQueue::reset () {
    // Reset only the current number of events, not the expected one
    _progressStatus.reset();
    
    // Empty the list of events
    _eventList.clear();

    // Reset the progress statuses for all the event types
    for (ProgressStatusMap_T::iterator itProgressStatus =
           _progressStatusMap.begin();
         itProgressStatus != _progressStatusMap.end(); ++itProgressStatus) {
      stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
      lProgressStatus.reset();
    }

  }
  
  // //////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueue::
  getCurrentNbOfEvents (const stdair::EventType::EN_EventType& iType) const {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::const_iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) {
      //
      STDAIR_LOG_ERROR ("No ProgressStatus structure can be retrieved in the "
                        << "EventQueue: " << display());
      assert (false);
    }
    
    const stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
    return lProgressStatus.getCurrentNb();
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueue::
  getExpectedTotalNbOfEvents (const stdair::EventType::EN_EventType& iType) const {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::const_iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) {
      std::ostringstream oStr;
      oStr << "No ProgressStatus structure can be retrieved in the EventQueue '"
           << display() << "'. The EventQueue should be initialised, e.g., by "
           << "calling a buildSampleBom() method.";
      //
      STDAIR_LOG_ERROR (oStr.str());
      throw EventQueueException (oStr.str());
    }
    
    const stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
    return lProgressStatus.getExpectedNb();
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Count_T& EventQueue::
  getActualTotalNbOfEvents (const stdair::EventType::EN_EventType& iType) const {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::const_iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) {
      //
      STDAIR_LOG_ERROR ("No ProgressStatus structure can be retrieved in the "
                        << "EventQueue: " << display());
      assert (false);
    }
    
    const stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
    return lProgressStatus.getActualNb();
  }

  // //////////////////////////////////////////////////////////////////////
  void EventQueue::updateStatus (const stdair::EventType::EN_EventType& iType,
                                 const stdair::ProgressStatus& iProgressStatus) {

    // Retrieve, if existing, the ProgressStatus structure
    // corresponding to the given event type
    ProgressStatusMap_T::iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) {
      const bool hasInsertBeenSuccessful =
        _progressStatusMap.insert (ProgressStatusMap_T::
                                   value_type (iType, iProgressStatus)).second;
      
      if (hasInsertBeenSuccessful == false) {
        STDAIR_LOG_ERROR ("No progress_status can be inserted "
                          << "for the following event type: "
                          << stdair::EventType::getLabel(iType)
                          << ". EventQueue: " << toString());
        throw stdair::EventException ("No progress_status can be inserted for the "
				      "following event type: "
				      + stdair::EventType::getLabel(iType)
				      + ". EventQueue: " + toString());
      }

      return;
    }
    
    stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;

    // Update the progress status
    const stdair::Count_T& lCurrentNb = iProgressStatus.getCurrentNb();
    lProgressStatus.setCurrentNb (lCurrentNb);

    const stdair::Count_T& lExpectedNb = iProgressStatus.getExpectedNb();
    lProgressStatus.setExpectedNb(lProgressStatus.getExpectedNb() + lExpectedNb);

    const stdair::Count_T& lActualNb = iProgressStatus.getActualNb();
    lProgressStatus.setActualNb (lProgressStatus.getActualNb() + lActualNb);
  }

  // //////////////////////////////////////////////////////////////////////
  void EventQueue::
  addStatus (const stdair::EventType::EN_EventType& iType,
             const stdair::NbOfEvents_T& iExpectedTotalNbOfEvents) {
    
    // Initialise the progress status object
    const stdair::Count_T lExpectedTotalNbOfEventsInt =
      static_cast<const stdair::Count_T> (std::floor (iExpectedTotalNbOfEvents));
    const stdair::ProgressStatus lProgressStatus (lExpectedTotalNbOfEventsInt);
      
    // Update the progress status for the given event type
    updateStatus (iType, lProgressStatus);
    
    // Update the overall progress status
    const stdair::Count_T lExpectedNb = 
      static_cast<const stdair::Count_T> (_progressStatus.getExpectedNb()
				  + iExpectedTotalNbOfEvents);
    _progressStatus.setExpectedNb (lExpectedNb);

    const stdair::Count_T lActualNb = 
      static_cast<const stdair::Count_T> (_progressStatus.getActualNb()
				  + iExpectedTotalNbOfEvents);
    _progressStatus.setActualNb (lActualNb);
  }

  // //////////////////////////////////////////////////////////////////////
  void EventQueue::updateStatus (const stdair::EventType::EN_EventType& iType,
                                 const stdair::NbOfEvents_T& iActualNbOfEvents) {

    // Initialise the progress status object for the type key
    stdair:: Count_T lActualNbOfEventsInt =
      static_cast<const  stdair::Count_T> (std::floor (iActualNbOfEvents));
      
    // Update the progress status for the corresponding content type key
    ProgressStatusMap_T::iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus != _progressStatusMap.end()) {
       stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
      //
      lActualNbOfEventsInt += lProgressStatus.getActualNb();
      //
      lProgressStatus.setActualNb (lActualNbOfEventsInt);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void EventQueue::setStatus (const stdair::EventType::EN_EventType& iType,
                              const stdair::ProgressStatus& iProgressStatus) {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::iterator itProgressStatus =
      _progressStatusMap.find (iType);
    // assert (itProgressStatus != _progressStatusMap.end());
    if (itProgressStatus != _progressStatusMap.end()) {
      // Update the ProgressStatus structure
      itProgressStatus->second = iProgressStatus;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::ProgressStatus EventQueue::
  getStatus (const stdair::EventType::EN_EventType& iType) const {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::const_iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus != _progressStatusMap.end()) {
      const stdair::ProgressStatus& oProgressStatus = itProgressStatus->second;
      return oProgressStatus;
    }

    return stdair::ProgressStatus();
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::ProgressPercentage_T EventQueue::
  calculateProgress (const stdair::EventType::EN_EventType& iType) const {

    // Retrieve the ProgressStatus structure corresponding to the
    // given event type
    ProgressStatusMap_T::const_iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) {
      //
      STDAIR_LOG_ERROR ("No ProgressStatus structure can be retrieved in the "
                        << "EventQueue: " << display());
      assert (false);
    }
    
    const stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
    return lProgressStatus.progress();
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet EventQueue::popEvent (stdair::EventStruct& ioEventStruct) {

    if (_eventList.empty() == true) {  
      std::ostringstream oStr;
      oStr << "The event queue '" << describeKey() << "' is empty. "
	   << "No event can be popped.";
      //
      STDAIR_LOG_ERROR (oStr.str());
      throw EventQueueException (oStr.str());
    }

    /**
     * 1. Update the event queue itself.
     */
    // Get an iterator on the first event (sorted by date-time stamps)
    stdair::EventList_T::iterator itEvent = _eventList.begin();

    /**
     * Extract (a copy of) the corresponding Event structure. We make
     * a copy here, as the original EventStruct structure is removed
     * from the list (and erased). Moreover, the resulting EventStruct
     * structure will be returned by this method.
     */
    ioEventStruct = itEvent->second;
    // Retrieve the event type
    const stdair::EventType::EN_EventType& lEventType = ioEventStruct.getEventType();
    stdair::ProgressStatusSet oProgressStatusSet (lEventType);
  
    // Update the (current number part of the) overall progress status,
    // to account for the event that is being popped out of the event
    // queue.
    ++_progressStatus;
    
    // Remove the event, which has just been retrieved
    _eventList.erase (itEvent);


    /**
     * 2. Update the progress statuses held by the EventStruct structure.
     *
     * 2.1. Update the progress status specific to the event type (e.g.,
     *      booking request, optimisation notification).
     */

    // Retrieve the progress status specific to that event type
    stdair::ProgressStatus lEventTypeProgressStatus = getStatus (lEventType);

    // Increase the current number of events
    ++lEventTypeProgressStatus;

    // Store back the progress status
    setStatus (lEventType, lEventTypeProgressStatus);

    // Update the progress status of the progress status set, specific to
    // the event type.
    oProgressStatusSet.setTypeSpecificStatus (lEventTypeProgressStatus);

    /**
     * 2.2. Update the overall progress status.
     */
    // Update the overall progress status of the progress status set.
    oProgressStatusSet.setOverallStatus (_progressStatus);

    //
    return oProgressStatusSet;
  }

  // //////////////////////////////////////////////////////////////////////
  bool EventQueue::addEvent (stdair::EventStruct& ioEventStruct) {
    bool insertionSucceeded =
      _eventList.insert (stdair::EventListElement_T (ioEventStruct.getEventTimeStamp(),
						     ioEventStruct)).second;

    /**
     * If the insertion has not been successful, try repeatedly until
     * the insertion becomes successful.
     *
     * The date-time is counted in milliseconds (1e-3 second). Hence,
     * one thousand (1e3) of attempts correspond to 1 second.
     *     
     * The check on one thousand (1e3) is made in order to avoid
     * potential infinite loops. In such case, however, an assertion
     * will fail: it is always better that an assertion fails rather
     * than entering an infinite loop.
     */
    const unsigned int idx = 0;
    while (insertionSucceeded == false && idx != 1e3) {
      // Increment the date-time stamp (expressed in milliseconds)
      ioEventStruct.incrementEventTimeStamp();

      // Retry to insert into the event queue
      insertionSucceeded =
        _eventList.insert (stdair::EventListElement_T (ioEventStruct.getEventTimeStamp(),
						       ioEventStruct)).second;
    }
    assert (idx != 1e3);

    return insertionSucceeded;
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool EventQueue::hasEventDateTime (const stdair::DateTime_T& iDateTime) {

    bool hasSearchEventBeenSucessful = true;

    /**
     * Compute the number of milliseconds between the
     * date-time of the event and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const stdair::Duration_T lDuration =
      iDateTime - stdair::DEFAULT_EVENT_OLDEST_DATETIME;
    const stdair::LongDuration_T lDateTimeStamp =
      lDuration.total_milliseconds();

    // Searches the container for an element with iDateTime as key
    stdair::EventList_T::iterator itEvent =
      _eventList.find (lDateTimeStamp);

    // An iterator to map::end means the specified key has not found in the
    // container.
    if (itEvent == _eventList.end()) {
      hasSearchEventBeenSucessful = false;
    }

    return hasSearchEventBeenSucessful;

  }

}
