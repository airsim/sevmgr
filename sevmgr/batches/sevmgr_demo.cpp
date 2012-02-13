// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
//  //// Boost (Extended STL) ////
// Boost Program Options
#include <boost/program_options.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/bom/EventStruct.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/config/sevmgr-paths.hpp>

// //////// Constants //////
/** Default name and location for the log file. */
const stdair::Filename_T K_SEVMGR_DEFAULT_LOG_FILENAME ("sevmgr_demo.log");

/** Early return status (so that it can be differentiated from an error). */
const int K_SEVMGR_EARLY_RETURN_STATUS = 99;


// ///////// Parsing of Options & Configuration /////////
/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[],
                       stdair::Filename_T& ioLogFilename) {

  // Declare a group of options that will be allowed only on command line
  boost::program_options::options_description generic ("Generic options");
  generic.add_options()
    ("prefix", "print installation prefix")
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be allowed both on command
  // line and in config file
  boost::program_options::options_description config ("Configuration");
  config.add_options()
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_SEVMGR_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ;

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  boost::program_options::options_description hidden ("Hidden options");
  hidden.add_options()
    ("copyright",
     boost::program_options::value< std::vector<std::string> >(),
     "Show the copyright (license)");
        
  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  boost::program_options::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  boost::program_options::options_description visible ("Allowed options");
  visible.add(generic).add(config);
        
  boost::program_options::positional_options_description p;
  p.add ("copyright", -1);
        
  boost::program_options::variables_map vm;
  boost::program_options::
    store (boost::program_options::command_line_parser (argc, argv).
           options (cmdline_options).positional(p).run(), vm);

  std::ifstream ifs ("sevmgr.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_SEVMGR_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_SEVMGR_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_SEVMGR_EARLY_RETURN_STATUS;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }
  
  return 0;
}


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // Output log File
  stdair::Filename_T lLogFilename;

  // Call the command-line option parser
  const int lOptionParserStatus = readConfiguration (argc, argv, lLogFilename);

  if (lOptionParserStatus == K_SEVMGR_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Set up the log parameters
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);

  /**
   * Initialise the Sevmgr service object:
   */
  SEVMGR::SEVMGR_Service sevmgrService (lLogParams);

  // Build the default sample queue.  
  STDAIR_LOG_DEBUG ("Build the default sample queue.");
  sevmgrService.buildSampleQueue();

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
      
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Poped booking request: '"
                      << lPoppedRequest.describe() << "'.");

    // Iterate
    ++idx;
  }

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the simulation");
  
  // Close the Log outputFile
  logOutputFile.close();

  /*
    Note: as that program is not intended to be run on a server in
    production, it is better not to catch the exceptions. When it
    happens (that an exception is throwned), that way we get the
    call stack.
  */

  return 0;
}
