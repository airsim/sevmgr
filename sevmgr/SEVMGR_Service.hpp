#ifndef __SEVMGR_SEVMGR_SERVICE_HPP
#define __SEVMGR_SEVMGR_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/EventTypes.hpp>

// Forward declarations
namespace stdair {
  class EventQueue;
  struct ProgressStatusSet;
  struct BasLogParams;
  struct BasDBParams;
  struct EventStruct;
}

namespace SEVMGR {

  /// Forward declarations
  class SEVMGR_ServiceContext;
  
  /**
   * @brief class holding the services related to Travel Demand Generation.
   */
  class SEVMGR_Service {
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * @brief Constructor.
     *
     * The initSevmgrService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    SEVMGR_Service (const stdair::BasLogParams&, const stdair::BasDBParams&);

    /**
     * Constructor.
     *
     * The initSevmgrService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     */
    SEVMGR_Service (const stdair::BasLogParams&);

    /**
     * Constructor.
     *
     * The initSevmgrService() method is called; see the
     * corresponding documentation for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * neither any database access parameter is given, it is assumed
     * that the StdAir log service has already been initialised with
     * the proper log output stream by some other methods in the calling
     * chain (for instance, when the SEVMGR_Service is itself being
     * initialised by another library service such as DSIM_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Handler on the STDAIR_Service.
     */
    SEVMGR_Service (stdair::STDAIR_ServicePtr_T);
    
    /**
     * Destructor.
     */
    ~SEVMGR_Service();


  public:
    // ////////////////// Business support methods //////////////////    
    /**
     * Build a sample BOM tree.
     */
    void buildSampleBom();

    /**
     * Pop the next coming (in time) event, and remove it from the
     * event queue.
     * <ul>
     *   <li>The next coming (in time) event corresponds to the event
     *     having the earliest date-time stamp. In other words, it is
     *     the first/front element of the event queue.</li>
     *   <li>That (first) event/element is then removed from the event
     *     queue</li>
     *   <li>The progress status is updated for the corresponding
     *     demand stream.</li>
     * </ul>
     *
     * @return stdair::EventStruct A copy of the event structure,
     *   which comes first in time from within the event queue.
     */
    stdair::ProgressStatusSet popEvent (stdair::EventStruct&) const;

    /**
     * States whether the event queue has reached the end.
     *
     * For now, that method states whether the event queue is empty.
     */
    bool isQueueDone() const;

    /**
     * Reset the context of the demand streams for another demand generation
     * without having to reparse the demand input file.
     */
    void reset() const;


  public:
    // //////////////// Display support methods /////////////////
    /**
     * Recursively display (dump in the returned string) the objects
     * of the BOM tree.
     *
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay() const;


  private:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Default Constructors, which must not be used.
     */
    SEVMGR_Service();
    
    /**
     * Default copy constructor.
     */
    SEVMGR_Service (const SEVMGR_Service&);

    /**
     * Initialise the (SEVMGR) service context (i.e., the
     * SEVMGR_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *        log stream.
     * @param const stdair::BasDBParams& Parameters for the database
     *        access.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&,
                                                   const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *        log stream.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the SEVMGR_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not Sevmgr owns the STDAIR
     *        service resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const bool iOwnStdairService);
    
    /**
     * Initialise.
     *
     * Nothing is being done at that stage. The buildSampleBom() method may
     * be called later.
     */
    void initSevmgrService();

    /**
     * Finalise.
     */
    void finalise();

    
  private:
    // ///////// Service Context /////////
    /**
     * Sevmgr context.
     */
    SEVMGR_ServiceContext* _sevmgrServiceContext;
  };

}
#endif // __SEVMGR_SEVMGR_SERVICE_HPP
