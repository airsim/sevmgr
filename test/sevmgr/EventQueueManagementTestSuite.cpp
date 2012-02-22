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
#include <stdair/bom/EventStruct.hpp>
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

  /** Is the queue empty? */
  const bool isQueueDone = sevmgrService.isQueueDone();
  BOOST_REQUIRE_MESSAGE (isQueueDone == true,
                         "The event queue should be empty at this step. No "
                         << "insertion done.");  

  /**
   * Build a sample event queue.
   */
  sevmgrService.buildSampleQueue (); 

  /**
   * Get the size of the queue.
   */
  stdair::Count_T lNbOfEvents (sevmgrService.getQueueSize());

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
    STDAIR_LOG_DEBUG ("Progresss status: " << lPPS.describe());
    STDAIR_LOG_DEBUG ("Poped event: '"
                      << lEventStruct.describe() << "'.");

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

  // Add again the four events into the queue thanks to 
  // sevmgrService.buildSampleQueue(). 
  // Dates of the break points: 21-JAN-2010 and 14-MAY-2011.
  // Dates of the booking requests: 22-JAN-2010 and 15-MAY-2011.
  sevmgrService.buildSampleQueue ();

  // Pop the next event out of the event queue
  stdair::EventStruct lFirstEventStruct;
  const stdair::ProgressStatusSet lFirstPS =
    sevmgrService.popEvent (lFirstEventStruct);
  
  // Extract the corresponding date
  const stdair::DateTime_T& lFirstEventDateTime =
    lFirstEventStruct.getEventTime ();
  const stdair::Date_T& lFirstRequestDate =
    lFirstEventDateTime.date();

  /** Is the first event popped the first event in time? */
  const stdair::Date_T lExpectedDate (2010, boost::gregorian::Jan, 21);
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

  STDAIR_LOG_DEBUG ("Re-added the events into the queue one more time");

  // Add again the four events into the queue thanks to 
  // sevmgrService.buildSampleQueue(). 
  // Dates of the break points: 21-JAN-2010 and 14-MAY-2011.
  // Dates of the booking requests: 22-JAN-2010 and 15-MAY-2011.
  sevmgrService.buildSampleQueue ();

  /** Run to find the two break points: the default sample queue contains
   *  two break points.*/
  stdair::EventStruct lBreakPointStruct;
  sevmgrService.run(lBreakPointStruct);
  stdair::EventType::EN_EventType lBreakPointType =
    lBreakPointStruct.getEventType();

  /** Is it a break point? */
  BOOST_REQUIRE_MESSAGE (lBreakPointType == stdair::EventType::BRK_PT,
                         "The last event poppped from the queue should be a "
                         << "break point.");

  sevmgrService.run(lBreakPointStruct);
  lBreakPointType = lBreakPointStruct.getEventType();
  
  /** Is it a break point? */
  BOOST_REQUIRE_MESSAGE (lBreakPointType == stdair::EventType::BRK_PT,
                         "The last event poppped from the queue should be a "
                         << "break point.");

  // Extract the corresponding date
  const stdair::DateTime_T& lBPDateTime =
    lBreakPointStruct.getEventTime ();
  const stdair::Date_T& lBPDate =
    lBPDateTime.date();

  /** Is it the expected break-point? */
  const stdair::Date_T lExpectedBPDate (2011, boost::gregorian::May, 14);
  BOOST_REQUIRE_MESSAGE (lBPDate == lExpectedBPDate,
                         "Date of the second break point popped from the queue: "
                         << lBPDate << ". Should be: "
                         << lExpectedBPDate << ".");

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
