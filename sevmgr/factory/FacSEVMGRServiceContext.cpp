// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// Sevmgr
#include <sevmgr/factory/FacSEVMGRServiceContext.hpp>
#include <sevmgr/service/SEVMGR_ServiceContext.hpp>

namespace SEVMGR {

  FacSEVMGRServiceContext* FacSEVMGRServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSEVMGRServiceContext::~FacSEVMGRServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSEVMGRServiceContext& FacSEVMGRServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacSEVMGRServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  SEVMGR_ServiceContext& FacSEVMGRServiceContext::create () {
    SEVMGR_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new SEVMGR_ServiceContext ();
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
