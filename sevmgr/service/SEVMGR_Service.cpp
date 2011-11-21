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
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Sevmgr
#include <sevmgr/basic/BasConst_SEVMGR_Service.hpp>
#include <sevmgr/factory/FacSEVMGRServiceContext.hpp>
#include <sevmgr/command/EventQueueManager.hpp>
#include <sevmgr/service/SEVMGR_ServiceContext.hpp>
#include <sevmgr/SEVMGR_Service.hpp>

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

    // Add the StdAir service context to the SEVMGR service context
    // \note SEVMGR owns the STDAIR service resources here.
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

    // Add the StdAir service context to the SEVMGR service context
    // \note SEVMGR owns the STDAIR service resources here.
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

    // Add the StdAir service context to the SEVMGR service context
    // \note Sevmgr does not own the STDAIR service resources here.
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
    // Retrieve the Sevmgr service context
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Store the STDAIR service object within the (SEVMGR) service context
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
  void SEVMGR_Service::buildSampleBom() {

    // Retrieve the Sevmgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Sevmgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    //SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the STDAIR service object from the (Sevmgr) service context
    //stdair::STDAIR_Service& lSTDAIR_Service =
    // lSEVMGR_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue
    //stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string SEVMGR_Service::csvDisplay() const {

    // Retrieve the Sevmgr service context
    if (_sevmgrServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Sevmgr service has "
                                                    "not been initialised");
    }
    assert (_sevmgrServiceContext != NULL);

    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the STDAIR service object from the (Sevmgr) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSEVMGR_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue
    stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();

    // Delegate the BOM building to the dedicated service
    return stdair::BomDisplay::csvDisplay (lEventQueue);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet SEVMGR_Service::
  popEvent (stdair::EventStruct& ioEventStruct) const {

    // Retrieve the Sevmgr service context
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext = *_sevmgrServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSEVMGR_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Extract the next event from the queue
    return lQueue.popEvent (ioEventStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  bool SEVMGR_Service::isQueueDone() const {

    // Retrieve the Sevmgr service context
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSEVMGR_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    const stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Calculates whether the event queue has been fully emptied
    const bool isQueueDone = lQueue.isQueueDone();

    //
    return isQueueDone;
  }

  // ////////////////////////////////////////////////////////////////////
  void SEVMGR_Service::reset() const {

    // Retrieve the Sevmgr service context
    assert (_sevmgrServiceContext != NULL);
    SEVMGR_ServiceContext& lSEVMGR_ServiceContext =
      *_sevmgrServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSEVMGR_ServiceContext.getSTDAIR_Service();
    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Delegate the call to the dedicated command
    EventQueueManager::reset (lQueue);
  }
}