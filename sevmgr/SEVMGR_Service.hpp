#ifndef __SEVMGR_SEVMGR_SERVICE_HPP
#define __SEVMGR_SEVMGR_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/EventTypes.hpp>
#include <stdair/bom/EventStruct.hpp>

// Forward declarations
namespace stdair {
  struct ProgressStatusSet;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
}

namespace SEVMGR {

  /// Forward declarations
  class SEVMGR_ServiceContext;
  class EventQueue; 
  //struct EventStruct;
  
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
     * Build a sample event queue.
     */
    void buildSampleQueue();

    /**
     * Build a sample booking request structure.
     *
     * As of now (March 2011), the sample booking request is made of the
     * following parameters:
     * <ul>
     *  <li>Return trip (inbound): LHR-SYD (POS: LHR, Channel: DN), </li>
     *  <li>Departing 10-JUN-2011 around 8:00, staying 7 days</li>
     *  <li>Requested on 15-MAY-2011 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * As of now (March 2011), the CRS-related booking request is made
     * of the following parameters:
     * <ul>
     *  <li>Return trip (inbound): SIN-BKK (POS: SIN, Channel: IN), </li>
     *  <li>Departing 30-JAN-2010 around 10:00, staying 7 days</li>
     *  <li>Requested on 22-JAN-2010 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * \see stdair::CmdBomManager for more details.
     *
     * @param const bool isForCRS Whether the sample booking request is for CRS.
     * @return BookingRequestStruct& Sample booking request structure.
     */ 
    stdair::BookingRequestStruct buildSampleBookingRequest(const bool isForCRS = false);

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
     *     event generator.</li>
     * </ul>
     *
     * @return stdair::EventStruct A copy of the event structure,
     *   which comes first in time from within the event queue.
     */
    stdair::ProgressStatusSet popEvent (stdair::EventStruct&) const;


    /**
     * Played all events and stopped when the first break point was encountered.
     *
     * @return stdair::EventStruct A copy of the break point which came first
     *   in time within the event queue. If no break point was
     *   encountered, return a copy of the last event within the event queue.
     */
    void run (stdair::EventStruct&) const;

    /**
     * Add an event generator to the map holding the children of the queue.
     * Be careful, this method is not implemented: its implementation is
     * left to the appelant according the EventGenerator type.
     *
     * \note An instance of implementation of that method can be found in the
     *       TraDemGen service.
     */
    template<class EventGenerator>
    void addEventGenerator(EventGenerator& iEventGenerator) const;
    
    /**
     * Add an event to the queue.
     */
    void addEvent(stdair::EventStruct&) const;

    /**
     * Reset the context of the event generators for another event generation
     * without having to reparse the demand input file.
     */
    void reset() const;

    /**
     * Update the progress status for the given event type (e.g., booking
     * request, optimisation notification, schedule change, break point).
     *
     * @param const stdair::EventType::EN_EventType& Type of the events for
     * which the actual total count is updated.
     * @return const stdair::Count_T& Expected Actual count of such events
     * already generated
     */
    void updateStatus (const stdair::EventType::EN_EventType&,
                       const stdair::Count_T&) const;

    /**
     * Initialise the progress statuses for the given event type
     * (e.g., request, snapshot).
     *
     * @param const stdair::EventType::EN_EventType& Type of the events for
     * which the actual total count is updated.
     * @return const stdair::Count_T& Expected Actual count of such events
     * already generated
     */
     void addStatus (const stdair::EventType::EN_EventType&,
                     const stdair::Count_T&) const;

    /**
     * States whether the event queue has reached the end.
     *
     * For now, that method states whether the event queue is empty.
     */
    bool isQueueDone() const;  

    /* @brief Get a reference on the EventQueue object.
     *
     * @return EventQueue& Reference on the EventQueue.
     */
    EventQueue& getEventQueue() const;

    /**
     * Get the size of the queue.
     */
    const stdair::Count_T& getQueueSize() const;

    /**
     * Extract an event generator from the map holding the children of the
     * queue.
     * Be careful, this method is not implemented: its implementation is
     * left to the appelant according the EventGenerator type.
     *
     * \note An instance of implementation of that method can be found in the
     *       TraDemGen service.
     */
    template<class EventGenerator, class Key>
    EventGenerator& getEventGenerator(const Key& iKey) const;

    /**
     * Extract the event generator list from the map holding the children of the
     * queue.
     * Be careful, this method is not implemented: its implementation is
     * left to the appelant according the EventGenerator type.
     *
     * \note An instance of implementation of that method can be found in the
     *       TraDemGen service.
     */
    template<class EventGenerator>
    const std::list<EventGenerator*> getEventGeneratorList() const;

    /**
     * Check whether there are DemandStream objects.
     *
     * Be careful, this method is not implemented: its implementation is
     * left to the appelant according the EventGenerator type.
     *
     * \note An instance of implementation of that method can be found in the
     *       TraDemGen service.
     */
    template<class EventGenerator>
    bool hasEventGeneratorList() const;   

    /**
     * Get the expected number of events to be generated.
     *
     * The getExpectedTotalNbOfEvents() method is called on the
     * underlying EventQueue object, which keeps track of that number.
     *
     * \note That number usually corresponds to an expectation (i.e.,
     *       the mean value of a random distribution), and may not be
     *       accurate. The actual number will be known after calling
     *       the generateFirstEvents() method for each event type
     *       (e.g., booking request, optimisation notification, etc).
     *
     * @return const Count_T& Expected number of events to be generated.
     */
    const stdair::Count_T& getExpectedTotalNumberOfEventsToBeGenerated() const;

    /**
     * Get the expected number of events to be generated for the given
     * event type.
     *
     * The getExpectedTotalNbOfEvents() method is called on the
     * underlying EventQueue object, which keeps track of that number.
     *
     * \note That number usually corresponds to an expectation (i.e.,
     *       the mean value of a random distribution), and may not be
     *       accurate. The actual number will be known after calling
     *       the generateFirstEvents() method for each event type
     *       (e.g., booking request, optimisation notification, etc).
     *
     * @param const EventType_T& Event type for which the number is calculated.
     * @return const Count_T& Expected number of events to be generated.
     */  
    const stdair::Count_T& 
    getExpectedTotalNumberOfEventsToBeGenerated(const stdair::EventType::EN_EventType&) const;  

    /**
     * Get the actual number of events to be generated for all the
     * demand streams.
     *
     * The getActualTotalNbOfEvents() method is called on the
     * underlying EventQueue object, which keeps track of that number.
     *
     * \note That number is being known after calling the
     *       generateFirstEvents() method.
     *
     * @return const Count_T& Expected number of events to be generated.
     */
    const stdair::Count_T& getActualTotalNumberOfEventsToBeGenerated() const;

    /**
     * Get the expected number of events to be generated for the given
     * event type.
     *
     * The getExpectedTotalNbOfEvents() method is called on the
     * underlying EventQueue object, which keeps track of that number.
     *
     * \note That number usually corresponds to an expectation (i.e.,
     *       the mean value of a random distribution), and may not be
     *       accurate. The actual number will be known after calling
     *       the generateFirstEvents() method for each event type
     *       (e.g., booking request, optimisation notification, etc).
     *
     * @param const EventType_T& Event type for which the number is calculated.
     * @return const Count_T& Expected number of events to be generated.
     */
    const stdair::Count_T& 
    getActualTotalNumberOfEventsToBeGenerated(const stdair::EventType::EN_EventType&) const;

    /**
     * Set the actual total number of events (for the whole event queue).
     *
     * @param const stdair::Count_T& Total number of events.
     */
    void setActualTotalNbOfEvents (const stdair::Count_T&);

  public:
    // //////////////// Display support methods /////////////////

    /**
     * Display (dump in the returned string) the key of the event queue.
     *
     * @return std::string Output string in which the key is
     *        logged/dumped.
     */
    std::string describeKey() const;   

    /**
     * Display (dump in the returned string) the event list of the event queue.
     *
     * @return std::string Output string in which the events are
     *        logged/dumped.
     */
    std::string list () const; 

    /**
     * Dump in the returned string and in JSON format the whole list of events 
     * queue. 
     */
    std::string jsonExportEventQueue () const; 

    /**
     * Dump in the returned string and in JSON format the given event. 
     */
    std::string jsonExportEvent (const stdair::EventStruct&) const;

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
    // ////////////////// Getters //////////////////    
    /**
     * Get the STDAIR service.
     *
     * \note Needed for the implementation of the addEventGenerator method
     * method in the TraDemGen service.
     */
    const stdair::STDAIR_Service& getSTDAIR_Service() const;
    
  private:
    // ///////// Service Context /////////
    /**
     * Sevmgr context.
     */
    SEVMGR_ServiceContext* _sevmgrServiceContext;
  };

}
#endif // __SEVMGR_SEVMGR_SERVICE_HPP
