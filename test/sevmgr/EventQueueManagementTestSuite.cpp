/*!
 * \page EventQueueManagementTestSuite_cpp Command-Line Test to Demonstrate How To Use Sevmgr elements
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <map>
#include <cmath>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE EventQueueManagementTest
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/BasConst_DefaultObject.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/service/Logger.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/config/sevmgr-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("EventQueueManagementTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }
  
  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// Specific type definitions
typedef std::pair<stdair::Count_T, stdair::Count_T> NbOfEventsPair_T;
typedef std::map<const stdair::DemandStreamKeyStr_T,
                 NbOfEventsPair_T> NbOfEventsByDemandStreamMap_T;


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (sevmgr_simple_simulation_test) {
  
  // Output log File
  const stdair::Filename_T lLogFilename ("EventQueueManagementTestSuite.log");
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the Sevmgr service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SEVMGR::SEVMGR_Service sevmgrService (lLogParams);

  /** Build a sample BOM tree.
   * \note Do nothing for now.
   */
  sevmgrService.buildSampleBom();

  /** Is the queue empty? */
  const bool isQueueDone = sevmgrService.isQueueDone();
  BOOST_REQUIRE_MESSAGE (isQueueDone == true,
                         "The event queue should be empty at this step. No "
                         << "insertion done.");

  // Total number of events
  stdair::Count_T lNbOfEvents (2);

  // Create a shared pointer on a first booking request
  // Date of the request (15-MAY-2011)
  const stdair::BookingRequestStruct& lBookingRequest =
    sevmgrService.buildBookingRequest ();

  // TODO: understand why the following form does not work, knowing
  // that:
  // typedef boost::shared_ptr<stdair::BookingRequestStruct> stdair::BookingRequestPtr_T
  // stdair::BookingRequestPtr_T oBookingRequest_ptr =
  //   boost::make_shared<stdair::BookingRequestStruct> (lInteractiveBookingRequest);
  const stdair::BookingRequestPtr_T lBookingRequest_ptr =
    stdair::BookingRequestPtr_T(new stdair::BookingRequestStruct(lBookingRequest));
  
  // Create an event structure
  stdair::EventStruct lEventStruct (stdair::EventType::BKG_REQ,
                                    lBookingRequest_ptr);

  // Add the event into the queue
  sevmgrService.addEvent(lEventStruct);

  // Create a second shared pointer on a second booking request
  // Date of the request (22-JAN-2010)
  const bool isForCRS = true;
  const stdair::BookingRequestStruct& lBookingRequestForCRS =
    sevmgrService.buildBookingRequest (isForCRS);

  // TODO: understand why the following form does not work, knowing
  // that:
  // typedef boost::shared_ptr<stdair::BookingRequestStruct> stdair::BookingRequestPtr_T
  // stdair::BookingRequestPtr_T oBookingRequest_ptr =
  //   boost::make_shared<stdair::BookingRequestStruct> (lInteractiveBookingRequest);
  const stdair::BookingRequestPtr_T lBookingRequestForCRS_ptr =
    stdair::BookingRequestPtr_T(new stdair::BookingRequestStruct(lBookingRequestForCRS));
  
  // Create an event structure
  stdair::EventStruct lEventStructForCRS (stdair::EventType::BKG_REQ,
                                          lBookingRequestForCRS_ptr);

  // Add the event into the queue
  sevmgrService.addEvent(lEventStructForCRS);

  /** Is the queue empty? */
  BOOST_REQUIRE_MESSAGE (sevmgrService.isQueueDone() == false,
                         "The event queue should not be empty at this step. "
                         << "Two insertions done.");

  /**
     Main loop.
     <ul>
     <li>Pop a request and describes it.</li>
     </ul>
  */
  stdair::Count_T idx = 1;
  while (sevmgrService.isQueueDone() == false) {
    
    // Pop the next event out of the event queue
    stdair::EventStruct lEventStruct;
    const stdair::ProgressStatusSet lPPS =
      sevmgrService.popEvent (lEventStruct);

    // DEBUG
    STDAIR_LOG_DEBUG ("Poped event "<< idx << ": '"
                      << lEventStruct.describe() << "'.");
      
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Poped booking request: '"
                      << lPoppedRequest.describe() << "'.");

    // Iterate
    ++idx;
  }
  
  // Compensate for the last iteration
  --idx;
  // Compared the actual number of popped events with the expected one.
  BOOST_REQUIRE_MESSAGE (idx == lNbOfEvents,
                         "Actual number of requests in the queue: "
                         << idx << ". Expected value: " << lNbOfEvents);

  /** Is the queue empty? */
  BOOST_REQUIRE_MESSAGE (sevmgrService.isQueueDone() == true,
                         "The event queue should be empty at this step: "
                         "the two events have been popped.");

  STDAIR_LOG_DEBUG ("Re-added the events into the queue");

  // Add again the two events into the queue
  // Date of the request (15-MAY-2011)
  sevmgrService.addEvent(lEventStruct);
  // Date of the request (22-JAN-2010)
  sevmgrService.addEvent(lEventStructForCRS);

  // Pop the next event out of the event queue
  stdair::EventStruct lFirstEventStruct;
  const stdair::ProgressStatusSet lFirstPS =
    sevmgrService.popEvent (lFirstEventStruct);
  
  // Extract the corresponding demand/booking request
  const stdair::BookingRequestStruct& lFirstRequest =
    lFirstEventStruct.getBookingRequest();
  
  // Extract the corresponding booking request date
  const stdair::DateTime_T& lFirstRequestDateTime =
    lFirstRequest.getRequestDateTime();
  const stdair::Date_T& lFirstRequestDate =
    lFirstRequestDateTime.date();

  /** Is the first event popped the first event in time? */
  const stdair::Date_T lExpectedDate (2010, boost::gregorian::Jan, 22);
  BOOST_REQUIRE_MESSAGE (lFirstRequestDate == lExpectedDate,
                         "Date of the first event popped from the queue: "
                         << lFirstRequestDate << ". Should be: "
                         << lExpectedDate << " which is earlier in time.");
                
  /** Reset the context of the demand streams for another demand generation
      without having to reparse the demand input file. */
  STDAIR_LOG_DEBUG ("Reset the queue");
  sevmgrService.reset();

  /** Is the queue empty? */
  BOOST_REQUIRE_MESSAGE (sevmgrService.isQueueDone() == true,
                         "The event queue has been reset: it should be empty "
                         << "at this step.");

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the simulation");

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
