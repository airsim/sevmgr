// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/BasConst_General.hpp>
// SEvMgr
#include <sevmgr/basic/BasConst_EventQueueManager.hpp>
#include <sevmgr/service/SEVMGR_ServiceContext.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::SEVMGR_ServiceContext() {
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::
  SEVMGR_ServiceContext (const std::string& iServiceName) {
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext::~SEVMGR_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SEVMGR_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SEVMGR_ServiceContext -- Owns StdAir service: "
         << _ownStdairService;
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
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
