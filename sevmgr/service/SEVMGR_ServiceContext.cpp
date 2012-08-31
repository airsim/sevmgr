// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/factory/FacBom.hpp>
// SEvMgr
#include <sevmgr/basic/BasConst_EventQueueManager.hpp>
#include <sevmgr/bom/EventQueue.hpp>
#include <sevmgr/service/SEVMGR_ServiceContext.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::SEVMGR_ServiceContext() 
    : _eventQueue (NULL) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::
  SEVMGR_ServiceContext (const SEVMGR_ServiceContext& iServiceContext)
    : _eventQueue (iServiceContext._eventQueue) {
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::~SEVMGR_ServiceContext() {
  } 

  // //////////////////////////////////////////////////////////////////////
  void SEVMGR_ServiceContext::init() {
    //
    initEventQueue();
  }  

  // //////////////////////////////////////////////////////////////////////
  void SEVMGR_ServiceContext::initEventQueue() {
    
    // The event queue key is just a string. For now, it is not used.
    const EventQueueKey lKey ("EQ01");

    // Create an EventQueue object instance
    EventQueue& lEventQueue = stdair::FacBom<EventQueue>::instance().create (lKey);

    // Store the event queue object
    _eventQueue = &lEventQueue;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SEVMGR_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SEVMGR_ServiceContext -- Owns StdAir service: "
         << _ownStdairService;
    if (_eventQueue != NULL) {
      oStr << " -- Queue: " << _eventQueue->toString();
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SEVMGR_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SEVMGR_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void SEVMGR_ServiceContext::reset() {
    
    // The shared_ptr<>::reset() method drops the refcount by one.
    // If the count result is dropping to zero, the resource pointed to
    // by the shared_ptr<> will be freed.

    // Reset the stdair shared pointer
    _stdairService.reset();
  }  

  // //////////////////////////////////////////////////////////////////////
  EventQueue& SEVMGR_ServiceContext::getEventQueue() const {
    assert (_eventQueue != NULL);
    return *_eventQueue;
  }

}
