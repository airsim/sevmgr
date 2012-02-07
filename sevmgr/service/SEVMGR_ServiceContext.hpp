#ifndef __SEVMGR_SVC_SEVMGRSERVICECONTEXT_HPP
#define __SEVMGR_SVC_SEVMGRSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>

/// Forward declarations
namespace stdair {
  class FacBomManager;
  template <typename BOM> class FacBom;
}

namespace SEVMGR { 

  /// Forward declarations
  class EventQueue;

  /**
   * @brief Class holding the context of the Sevmgr services.
   */
  class SEVMGR_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The SEVMGR_Service class should be the sole class to get
     * access to ServiceContext content: general users do not want to
     * bother with a context interface.
     */
    friend class SEVMGR_Service;
    friend class FacSEVMGRServiceContext;
    
  private:
    // ///////// Getters //////////
    /**
     * Get the pointer on the STDAIR service handler.
     */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr() const {
      return _stdairService;
    }

    /**
     * Get the STDAIR service handler.
     */
    stdair::STDAIR_Service& getSTDAIR_Service() const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }   

    /**
     * Get the pointer on the EventQueue instance.
     */
    EventQueue& getEventQueue() const;


  private:
    // ///////// Setters //////////
    /**
     * Set the pointer on the STDAIR service handler.
     */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                            const bool iOwnStdairService) {
      _stdairService = ioSTDAIR_ServicePtr;
      _ownStdairService = iOwnStdairService;
    }

    
  private:
    // ///////// Display Methods //////////
    /**
     * Display the short SEVMGR_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full SEVMGR_ServiceContext content.
     */
    const std::string display() const;
    
    /**
     * Display of the structure.
     */
    const std::string describe() const;

    
  private:
    /// //////////////// Constructors and destructors /////////////
    /**
     * Main constructor.
     */
    SEVMGR_ServiceContext();
    /**
     * Copy constructor (not to be used).
     */
    SEVMGR_ServiceContext (const SEVMGR_ServiceContext&);

    /**
     * Destructor.
     */
    ~SEVMGR_ServiceContext();

    /**
     * Clear the context (cabin capacity, bucket holder).
     */
    void reset();    

    /**
     * Initialisation.
     *
     * That method is invoked from the constructors. It triggers, in
     * turn, the initialisation of the BOM tree root and of the event
     * queue.
     */
    void init();
   
    /**
     * Initialisation.
     *
     * The EventQueue object is created by that method, and then
     * stored within the service context.
     */
    void initEventQueue();

    
  private:
    // /////////////// Children ///////////////
    /**
     * Standard Airline (StdAir) Service Handler.
     */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    bool _ownStdairService;


  private:
    // ////////////// Attributes ////////////////
    /**
     * @brief EventQueue.
     */
    EventQueue* _eventQueue;
  };

}
#endif // __SEVMGR_SVC_SEVMGRSERVICECONTEXT_HPP
