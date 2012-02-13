/*!
 * \page batch_sevmgr_cpp Command-Line Utility to Demonstrate Typical SEvMgr Usage
 * \code
 */
// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/swap.hpp>
#include <boost/algorithm/string/case_conv.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/config/sevmgr-paths.hpp>
// GNU Readline Wrapper
#include <sevmgr/ui/cmdline/SReadline.hpp>

// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_SEVMGR_DEFAULT_LOG_FILENAME ("sevmgr.log");

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_SEVMGR_EARLY_RETURN_STATUS = 99;

/**
 * List of strings, representing the tokens as entered by the user on
 * a command-line.
 */
typedef std::vector<std::string> TokenList_T;

/**
 * Enumeration representing the command entered by the user on the command-line.
 */
struct Command_T {
  typedef enum {
    NOP = 0,
    QUIT,
    HELP,
    LIST,
    DISPLAY,
    SELECT,
    NEXT,
    RUN,
    JSON_LIST,
    JSON_DISPLAY,
    LAST_VALUE
  } Type_T;
};

// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/**
 * Read and parse the command line options.
 */
int readConfiguration (int argc, char* argv[], std::string& ioLogFilename) {
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
     "Filename for the logs")
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

// //////////////////////////////////////////////////////////////////
void initReadline (swift::SReadline& ioInputReader) {

  // Prepare the list of my own completers
  std::vector<std::string> Completers;

  // The following is supported:
  // - "identifiers"
  // - special identifier %file - means to perform a file name completion
  Completers.push_back ("help");
  Completers.push_back ("list %airline_code %flight_number");
  Completers.push_back ("select %airline_code %flight_number %flight_date");
  Completers.push_back ("display");
  Completers.push_back ("next");
  Completers.push_back ("run");
  Completers.push_back ("quit");


  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
Command_T::Type_T extractCommand (TokenList_T& ioTokenList) {
  Command_T::Type_T oCommandType = Command_T::LAST_VALUE;

  // Interpret the user input
  if (ioTokenList.empty() == false) {
    TokenList_T::iterator itTok = ioTokenList.begin();
    std::string lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "list") {
      oCommandType = Command_T::LIST;

    } else if (lCommand == "display") {
      oCommandType = Command_T::DISPLAY;

    } else if (lCommand == "select") {
      oCommandType = Command_T::SELECT;

    } else if (lCommand == "next") {
      oCommandType = Command_T::NEXT;

    } else if (lCommand == "run") {
      oCommandType = Command_T::RUN;

    } else if (lCommand == "json_list") {
      oCommandType = Command_T::JSON_LIST;
    
    } else if (lCommand == "json_display") {
      oCommandType = Command_T::JSON_DISPLAY;

    } else if (lCommand == "quit") {
      oCommandType = Command_T::QUIT;
    }

    // Remove the first token (the command), as the corresponding information
    // has been extracted in the form of the returned command type enumeration
    ioTokenList.erase (itTok);

  } else {
    oCommandType = Command_T::NOP;
  }

  return oCommandType;
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("sevmgr.hist");
  const std::string lHistoryBackupFilename ("sevmgr.hist.bak");  

  // Default parameters for the interactive session
  stdair::EventStruct lCurrentInteractiveEventStruct; 
  stdair::DateTime_T lCurrentInteractiveDateTime; 
  stdair::EventType::EN_EventType lCurrentInteractiveEventType;

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

  // Initialise the inventory service
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SEVMGR::SEVMGR_Service sevmgrService (lLogParams);

  // DEBUG
  STDAIR_LOG_DEBUG ("Welcome to SEvMgr");

  // Build the sample event queue.
  sevmgrService.buildSampleQueue();  

  // Pop out the first event from the queue.
  sevmgrService.popEvent(lCurrentInteractiveEventStruct);

  // DEBUG
  STDAIR_LOG_DEBUG ("====================================================");
  STDAIR_LOG_DEBUG ("=       Beginning of the interactive session       =");
  STDAIR_LOG_DEBUG ("====================================================");

  // Initialise the GNU readline wrapper
  swift::SReadline lReader (lHistoryFilename, lHistorySize);
  initReadline (lReader);

  // Now we can ask user for a line
  std::string lUserInput;
  bool EndOfInput (false);
  Command_T::Type_T lCommandType (Command_T::NOP);
  
  while (lCommandType != Command_T::QUIT && EndOfInput == false) {  

    // Update the interactive parameters which have not been updated yet
    lCurrentInteractiveDateTime = lCurrentInteractiveEventStruct.getEventTime ();
    lCurrentInteractiveEventType = lCurrentInteractiveEventStruct.getEventType ();

    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "sevmgr " 
	       << stdair::EventType::getTypeLabelAsString(lCurrentInteractiveEventType)
	       << " / " << lCurrentInteractiveDateTime << "> " ;
    // Call read-line, which will fill the list of tokens
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history can be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    // The end-of-input typically corresponds to a CTRL-D typed by the user
    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret the user input
    lCommandType = extractCommand (lTokenListByReadline);

    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl;
      std::cout << " help" << "\t\t" << "Display this help" << std::endl;
      std::cout << " quit" << "\t\t" << "Quit the application" << std::endl;
      std::cout << " list" << "\t\t" << "List events" << std::endl;
      std::cout << " select" << "\t\t"
                << "Select an event to become the current one" << std::endl;
      std::cout << " display" << "\t"
                << "Display the current event" << std::endl;
      std::cout << " next" << "\t\t"
                << "Play the current event and pop the next one from the queue"
                << std::endl;
      std::cout << " run" << "\t\t"
                << "Play all the events until the next break-point, if any"
                << std::endl;
      std::cout << " \nDebug Commands" << std::endl;
      std::cout << " json_list" << "\t"
                << "List events in a JSON format"
                << std::endl;
      std::cout << " json_display" << "\t"
                << "Display the current event in a JSON format"
                << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// List /////////////////////////
    case Command_T::LIST: {
      //
      std::cout << "List" << std::endl;   

      std::ostringstream oEventListStr;
      oEventListStr << sevmgrService.list ();	
      std::cout << oEventListStr.str() << std::endl;   
      STDAIR_LOG_DEBUG (oEventListStr.str());

      //
      break;
    }

      // ////////////////////////////// Select ////////////////////////
    case Command_T::SELECT: {
      //
      std::cout << "Select" << std::endl;

      //
      break;
    }

      // ////////////////////////////// Display ////////////////////////
    case Command_T::DISPLAY: {
      //
      std::cout << "Display" << std::endl;     

      // DEBUG 
      std::ostringstream oEventStr;
      oEventStr << lCurrentInteractiveEventStruct.describe(); 
      std::cout << oEventStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oEventStr.str());

      //
      break;
    }

      // ////////////////////////////// Next ////////////////////////
    case Command_T::NEXT: {
      //
      std::cout << "Next" << std::endl;  

      if (sevmgrService.isQueueDone() == true) { 

	// DEBUG 
	std::ostringstream oEmptyQueueStr;
	oEmptyQueueStr << "The event queue is empty: no event can be popped out.";
	std::cout << oEmptyQueueStr.str() << std::endl;   
	STDAIR_LOG_DEBUG (oEmptyQueueStr.str());

	//
	break;

      }

      // Get the next event from the event queue
      stdair::ProgressStatusSet lPPS = 
	sevmgrService.popEvent (lCurrentInteractiveEventStruct);
      
      // DEBUG 
      std::ostringstream oEventStr;
      oEventStr << "Poped event: '" 
		<< lCurrentInteractiveEventStruct.describe() << "'."; 
      std::cout << oEventStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oEventStr.str());

      //
      break;
    }     

      // ////////////////////////////// Run ////////////////////////
    case Command_T::RUN: {
      //
      std::cout << "Run" << std::endl;

      //
      break;
    }     

      // ////////////////////////////// JSon List ////////////////////////

    case Command_T::JSON_LIST: { 
      //
      std::cout << "JSON List" << std::endl;

      // Delegate the call to the dedicated service
      const std::string& lCSVEventQueueDumpAfter =
        sevmgrService.jsonExportEventQueue ();

      // DEBUG: Display the events queue JSON string
      std::cout << lCSVEventQueueDumpAfter << std::endl;
      STDAIR_LOG_DEBUG (lCSVEventQueueDumpAfter);
      
      break;
    } 

      // ////////////////////////////// JSon Display ////////////////////////

    case Command_T::JSON_DISPLAY: {      
      //
      std::cout << "JSON Display" << std::endl; 

      // Delegate the call to the dedicated service
      const std::string& lCSVEventDumpAfter =
        sevmgrService.jsonExportEvent (lCurrentInteractiveEventStruct);

      // DEBUG: Display the event JSON string
      std::cout << lCSVEventDumpAfter << std::endl;
      STDAIR_LOG_DEBUG (lCSVEventDumpAfter);

      //
      break;
    }

      // /////////////////////////// Default / No value ///////////////////////
    case Command_T::NOP: {
      break;
    }
 
    case Command_T::LAST_VALUE:
    default: {
      // DEBUG
      std::ostringstream oStr;
      oStr << "That command is not yet understood: '" << lUserInput
           << "' => " << lTokenListByReadline;
      STDAIR_LOG_DEBUG (oStr.str());
      std::cout << oStr.str() << std::endl;
    }
    }
  }

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the session. Exiting.");
  std::cout << "End of the session. Exiting." << std::endl;

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
