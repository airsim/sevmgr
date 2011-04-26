#ifndef __SEVMGR_FAC_FACSEVMGRSERVICECONTEXT_HPP
#define __SEVMGR_FAC_FACSEVMGRSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/service/FacServiceAbstract.hpp>
// Sevmgr
#include <sevmgr/SEVMGR_Types.hpp>

namespace SEVMGR {

  /** Forward declarations. */
  class SEVMGR_ServiceContext;

  /** Factory for Bucket. */
  class FacSEVMGRServiceContext : public stdair::FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacSEVMGRServiceContext& */
    static FacSEVMGRServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacSEVMGRServiceContext::instance(). */
    ~FacSEVMGRServiceContext();

    /** Create a new SEVMGR_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return SEVMGR_ServiceContext& The newly created object. */
    SEVMGR_ServiceContext& create ();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacSEVMGRServiceContext () {}

  private:
    /** The unique instance.*/
    static FacSEVMGRServiceContext* _instance;
  };

}
#endif // __SEVMGR_FAC_FACSEVMGRSERVICECONTEXT_HPP
