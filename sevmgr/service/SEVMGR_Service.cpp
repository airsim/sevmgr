// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/JSonCommand.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BomJSONImport.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Sevmgr
#include <sevmgr/basic/BasConst_SEVMGR_Service.hpp>
#include <sevmgr/factory/FacSEVMGRServiceContext.hpp>
#include <sevmgr/command/EventQueueManager.hpp>
#include <sevmgr/service/SEVMGR_ServiceContext.hpp>
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/bom/EventQueue.hpp>
#include <sevmgr/bom/BomJSONExport.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_Service::SEVMGR_Service() : _sevmgrServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_Service::SEVMGR_Service (const SEVMGR_Service& iService) 
    : _sevmgrServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_Service::SEVMGR_Service (const stdair::BasLogParams& iLogParams,
                                  const stdair::BasDBParams& iDBParams)
    : _sevmgrServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the SEvMgr service context
    // \note SEvMgr owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initSevmgrService();
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_Service::SEVMGR_Service (const stdair::BasLogParams& iLogParams)
    : _sevmgrServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the SEvMgr service context
    // \note SEvMgr owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initSevmgrService();
  }

  // ////////////////////////////////////////////////////////////////////
  SEVMGR_Service::
  SEVMGR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _sevmgrServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the SEvMgr service context
    // \note SEvMgr does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initialise the context
    initSevmgrService();
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_Service::~SEVMGR_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::finalise() {
    assert (_sevmgrServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _sevmgrServiceContext->reset();
  }

  // //////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::initServiceContext() {
    // Initialise the service context
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = 
      FacSEVMGRServiceContext::instance().create();
    _sevmgrServiceContext = &lSEVMGR_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {
    // Retrieve the SEvMgr service context
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Store the STDAIR service object within the (SEvMgr) service context
    lSEVMGR_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                              iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SEVMGR_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the STDAIR service handler.
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SEVMGR_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the STDAIR service handler.
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::initSevmgrService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::buildSampleQueue() {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);   

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext; 

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSEVMGR_ServiceContext.getSTDAIR_ServicePtr(); 

    // Retrieve the EventQueue
    EventQueue& lEventQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the building process to the dedicated command
    EventQueueManager::buildSampleQueue (lSTDAIR_Service_ptr, lEventQueue);

  }
 
  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct SEVMGR_Service::
  buildSampleBookingRequest(const bool isForCRS) {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext; 

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSEVMGR_ServiceContext.getSTDAIR_ServicePtr(); 
    
    // Delegate the booking request building to the dedicated service
    stdair::BookingRequestStruct oBookingRequest =
      EventQueueManager::buildSampleBookingRequest (lSTDAIR_Service_ptr, 
						    isForCRS);

    return oBookingRequest;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::describeKey() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;  

    // Retrieve the event queue
    EventQueue& lEventQueue = lSEVMGR_ServiceContext.getEventQueue();

    // Delegate the key display to the dedicated command
    return EventQueueManager::describeKey(lEventQueue);
  }  

  // //////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::list () const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;  

    // Retrieve the event queue
    EventQueue& lEventQueue = lSEVMGR_ServiceContext.getEventQueue ();

    // Delegate the key display to the dedicated command
    return EventQueueManager::list (lEventQueue);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::
  jsonHandler (const stdair::JSONString& iJSONString) const {

    //
    // Extract from the JSON-ified string the command
    //
    stdair::JSonCommand::EN_JSonCommand lEN_JSonCommand;
    const bool hasCommandBeenRetrieved =
      stdair::BomJSONImport::jsonImportCommand (iJSONString,
                                                lEN_JSonCommand);
    
    if (hasCommandBeenRetrieved == false) {
      // Return an error JSON-ified string
      std::ostringstream oErrorStream;
      oErrorStream << "{\"error\": \"Wrong JSON-ified string: "
                   << "the command is not understood.\"}";
      return oErrorStream.str();
    }
    assert (hasCommandBeenRetrieved == true);

    //
    // Dispatch the command to the right JSon service handler
    // 
    switch (lEN_JSonCommand) {
    case stdair::JSonCommand::EVENT_LIST:{

    return jsonExportEventQueue ();
    }
    default: {
        // Return an Error string
        std::ostringstream lErrorCmdMessage;
        const std::string& lCommandStr =
          stdair::JSonCommand::getLabel(lEN_JSonCommand);
        lErrorCmdMessage << "{\"error\": \"The command '" << lCommandStr
                         << "' is not handled by the DSim service.\"}";
        return lErrorCmdMessage.str();
        break;
      }
    }
    
    // Return an error JSON-ified string
    assert (false);
    std::string lJSONDump ("{\"error\": \"Wrong JSON-ified string\"}");
    return lJSONDump;
     
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::
  jsonExportEventQueue () const {  

    std::ostringstream oStr;  

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;   

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSEVMGR_ServiceContext.getSTDAIR_ServicePtr(); 

    // Retrieve the event queue
    const EventQueue& lEventQueue = 
      lSEVMGR_ServiceContext.getEventQueue();
 
    // Delegate the JSON export to the dedicated command
    BomJSONExport::jsonExportEventQueue (lSTDAIR_Service_ptr, oStr, 
					 lEventQueue);
    return oStr.str();
  
  }  

  // ////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::
  jsonExportEvent (const stdair::EventStruct& iEvent) const {  

    std::ostringstream oStr;  

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext; 

    // Retrieve the STDAIR service object from the (SEvMgr) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSEVMGR_ServiceContext.getSTDAIR_Service();

    // Delegate the JSON export to the dedicated service
    oStr << lSTDAIR_Service.jsonExportEventObject (iEvent);

    return oStr.str();
  
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet SEVMGR_Service::
  popEvent (stdair::EventStruct& iEventStruct) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    return EventQueueManager::popEvent(lQueue, iEventStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::
  run (stdair::EventStruct& iEventStruct) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    EventQueueManager::run (lQueue, iEventStruct);
    
  }

  // ////////////////////////////////////////////////////////////////////
  bool SEVMGR_Service::
  select (stdair::EventStruct& iEventStruct,
          const stdair::DateTime_T& iEventDateTime) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    return EventQueueManager::select (lQueue, iEventStruct, iEventDateTime);
    
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::
  updateStatus (const stdair::EventType::EN_EventType& iEventType,
                const stdair::Count_T& iEventCount) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    EventQueueManager::updateStatus (lQueue, iEventType, iEventCount);
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::
  addStatus (const stdair::EventType::EN_EventType& iEventType,
             const stdair::Count_T& iEventCount) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated function
    EventQueueManager::addStatus (lQueue, iEventType, iEventCount);
  }

  // ////////////////////////////////////////////////////////////////////
  bool SEVMGR_Service::isQueueDone() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Calculates whether the event queue has been fully emptied
    const bool isQueueDone = EventQueueManager::isQueueDone(lQueue);

    //
    return isQueueDone;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& SEVMGR_Service::getQueueSize() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    return EventQueueManager::getQueueSize(lQueue);
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::reset() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    EventQueueManager::reset (lQueue);
  }  

  // //////////////////////////////////////////////////////////////////////
  EventQueue& SEVMGR_Service::getEventQueue() const {  

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);  

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    return lSEVMGR_ServiceContext.getEventQueue();
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::addEvent(stdair::EventStruct& iEventStruct) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated command
    EventQueueManager::addEvent (lQueue, iEventStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& SEVMGR_Service::
  getExpectedTotalNumberOfEventsToBeGenerated() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;
    
    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated function
    return EventQueueManager::getExpectedTotalNumberOfEventsToBeGenerated (lQueue);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& SEVMGR_Service::
  getExpectedTotalNumberOfEventsToBeGenerated(const stdair::EventType::EN_EventType& iEventType) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;
    
    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated function
    return EventQueueManager::getExpectedTotalNumberOfEventsToBeGenerated (lQueue,
                                                                           iEventType);
  } 

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& SEVMGR_Service::
  getActualTotalNumberOfEventsToBeGenerated() const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;
    
    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated function
    return EventQueueManager::getActualTotalNumberOfEventsToBeGenerated(lQueue);

  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& SEVMGR_Service::
  getActualTotalNumberOfEventsToBeGenerated(const stdair::EventType::EN_EventType& iEventType) const {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;
    
    // Retrieve the event queue object instance
    const EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();
    
    // Delegate the call to the dedicated function
    return EventQueueManager::getActualTotalNumberOfEventsToBeGenerated(lQueue,
                                                                        iEventType);

  }
  
  //////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::
  setActualTotalNbOfEvents (const stdair::Count_T& iActualTotalNbOfEvents) {

    // Retrieve the SEvMgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;
    
    // Retrieve the event queue object instance
    EventQueue& lQueue = lSEVMGR_ServiceContext.getEventQueue();

    // Delegate the call to the dedicated function
    EventQueueManager::setActualTotalNbOfEvents (lQueue,
                                                 iActualTotalNbOfEvents);

  }

  //////////////////////////////////////////////////////////////////////
  const stdair::STDAIR_Service& SEVMGR_Service::getSTDAIR_Service() const {

      // Retrieve the StdAir service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SEvMgr service "
                                                    "has not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);
    const stdair::STDAIR_Service& lSTDAIR_Service =
      _sevmgrServiceContext->getSTDAIR_Service();

    //
    return lSTDAIR_Service;
  }
 
}
