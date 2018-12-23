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
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
// GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/config/sevmgr-paths.hpp>

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
  Completers.push_back ("list");  
  Completers.push_back ("list BookingRequest");
  Completers.push_back ("list BreakPoint");
  Completers.push_back ("select %date %time");
  Completers.push_back ("display");
  Completers.push_back ("next");
  Completers.push_back ("run");
  Completers.push_back ("json_list");
  Completers.push_back ("json_display");
  Completers.push_back ("quit");


  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
void parseEventDateTime (const TokenList_T& iTokenList,
                         stdair::Date_T& ioEventDate,
                         stdair::Duration_T& ioEventTime) {
  //
  const std::string kMonthStr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  //
  unsigned short ioEventDateYear = ioEventDate.year();
  unsigned short ioEventDateMonth = ioEventDate.month();
  std::string ioEventDateMonthStr = kMonthStr[ioEventDateMonth-1];
  unsigned short ioEventDateDay = ioEventDate.day();
  //
  unsigned short ioEventTimeHours = ioEventTime.hours();
  unsigned short ioEventTimeMinutes = ioEventTime.minutes();
  unsigned short ioEventTimeSeconds = ioEventTime.seconds();

  // Interpret the user input
  if (iTokenList.empty() == false) {
    
    // Read the date year
    TokenList_T::const_iterator itTok = iTokenList.begin();

    // Read the year for the event date
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {
          
          ioEventDateYear = boost::lexical_cast<unsigned short> (*itTok);
          if (ioEventDateYear < 100) {
            ioEventDateYear += 2000;
          }
          
        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The year of the event date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventDateYear << ") is kept. " << std::endl;
          return;
        }
      }
      
    } else {
      return;
    }
  
    // Read the month for the event date
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {

          const boost::regex lMonthRegex ("^(\\d{1,2})$");
          const bool isMonthANumber = regex_match (*itTok, lMonthRegex);
        
          if (isMonthANumber == true) {
            const unsigned short lMonth =
              boost::lexical_cast<unsigned short> (*itTok);
            if (lMonth > 12) {
              throw boost::bad_lexical_cast();
            }
            ioEventDateMonthStr = kMonthStr[lMonth-1];

        } else {
            const std::string lMonthStr (*itTok);
            if (lMonthStr.size() < 3) {
              throw boost::bad_lexical_cast();
            }
            std::string lMonthStr1 (lMonthStr.substr (0, 1));
            boost::algorithm::to_upper (lMonthStr1);
            std::string lMonthStr23 (lMonthStr.substr (1, 2));
            boost::algorithm::to_lower (lMonthStr23);
            ioEventDateMonthStr = lMonthStr1 + lMonthStr23;
          }

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The month of the event date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventDateMonthStr << ") is kept. " << std::endl;
          return;
        }
      }
    }

    // Read the day for the event date
    ++itTok;
    if (itTok != iTokenList.end()) {
    
      if (itTok->empty() == false) {
        try {
          
          ioEventDateDay = boost::lexical_cast<unsigned short> (*itTok);

        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The day of the event date ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventDateDay << ") is kept. " << std::endl;
          return;
        }
      }

    } else {
      return;
    }

    // Re-compose the event date
    std::ostringstream lEventDateStr;
    lEventDateStr << ioEventDateYear << "-" << ioEventDateMonthStr
                  << "-" << ioEventDateDay;

    try {

      ioEventDate =
        boost::gregorian::from_simple_string (lEventDateStr.str());
    
    } catch (boost::gregorian::bad_month& eCast) {
      std::cerr << "The event date ('" << lEventDateStr.str()
                << "') cannot be understood. The default value ("
                << ioEventDate << ") is kept. " << std::endl;
      return;
    }

    // Read the hours of the event time
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {
          
          ioEventTimeHours = boost::lexical_cast<unsigned short> (*itTok);
          
        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The hours of the event time ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventTimeHours << ") is kept. " << std::endl;
          return;
        }
      }
      
    } else {
      return;
    }

    // Read the minutes of the event time
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {
          
          ioEventTimeMinutes = boost::lexical_cast<unsigned short> (*itTok);
          
        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The minutes of the event time ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventTimeMinutes << ") is kept. " << std::endl;
          return;
        }
      }
      
    } else {
      return;
    }

    // Read the seconds of the event time
    ++itTok;
    if (itTok != iTokenList.end()) {

      if (itTok->empty() == false) {
        try {
          
          ioEventTimeSeconds = boost::lexical_cast<unsigned short> (*itTok);
          
        } catch (boost::bad_lexical_cast& eCast) {
          std::cerr << "The seconds of the event time ('" << *itTok
                    << "') cannot be understood. The default value ("
                    << ioEventTimeSeconds << ") is kept. " << std::endl;
          return;
        }
      }
      
    } else {
      return;
    }

    // Re-compose the event time
    std::ostringstream lEventTimeStr;
    lEventTimeStr << ioEventTimeHours << ":" << ioEventTimeMinutes
                  << ":" << ioEventTimeSeconds;

    try {

      ioEventTime =
        boost::posix_time::duration_from_string (lEventTimeStr.str());
    
    } catch (boost::gregorian::bad_month& eCast) {
      std::cerr << "The event time ('" << lEventTimeStr.str()
                << "') cannot be understood. The default value ("
                << ioEventTime << ") is kept. " << std::endl;
      return;
    }
    
  }
 
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

// /////////////////////////////////////////////////////////
std::string toString (const TokenList_T& iTokenList) {
  std::ostringstream oStr;

  // Re-create the string with all the tokens, trimmed by read-line
  unsigned short idx = 0;
  for (TokenList_T::const_iterator itTok = iTokenList.begin();
       itTok != iTokenList.end(); ++itTok, ++idx) {
    if (idx != 0) {
      oStr << " ";
    }
    oStr << *itTok;
  }

  return oStr.str();
}

// /////////////////////////////////////////////////////////
TokenList_T extractTokenList (const TokenList_T& iTokenList,
                              const std::string& iRegularExpression) {
  TokenList_T oTokenList;

  // Re-create the string with all the tokens (which had been trimmed
  // by read-line)
  const std::string lFullLine = toString (iTokenList);

  // See the caller for the regular expression
  boost::regex expression (iRegularExpression);
  
  std::string::const_iterator start = lFullLine.begin();
  std::string::const_iterator end = lFullLine.end();

  boost::match_results<std::string::const_iterator> what;
  boost::match_flag_type flags = boost::match_default | boost::format_sed; 
  regex_search (start, end, what, expression, flags);
  
  // Put the matched strings in the list of tokens to be returned back
  // to the caller
  const unsigned short lMatchSetSize = what.size();
  for (unsigned short matchIdx = 1; matchIdx != lMatchSetSize; ++matchIdx) {
    const std::string lMatchedString (std::string (what[matchIdx].first,
                                                   what[matchIdx].second));
    //if (lMatchedString.empty() == false) {
    oTokenList.push_back (lMatchedString);
    //}
  }

  // DEBUG
  // std::cout << "After (token list): " << oTokenList << std::endl;

  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForDateTime (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:    date time
   *   date:    year[/- ]?month[/- ]?day
   *   year:    number (digit{2,4})
   *   month:   (number (digit{1,2}) | word (alpha{3}))
   *   day:     number (digit{1,2})
   *   time:    hour[:- ]?minute[:- ]?second
   *   hour:    number (digit{1,2})
   *   minute:  (number (digit{1,2}) | word (alpha{3}))
   *   seconde: number (digit{1,2})
   */
  const std::string lRegEx("^([[:digit:]]{2,4})?[/-]?[[:space:]]*"
                           "([[:alpha:]]{3}|[[:digit:]]{1,2})?[/-]?[[:space:]]*"
                           "([[:digit:]]{1,2})?[[:space:]]*"
                           "([[:digit:]]{1,2})?[:-]?[[:space:]]*"
                           "([[:alpha:]]{3}|[[:digit:]]{1,2})?[:-]?[[:space:]]*"
                           "([[:digit:]]{1,2})?[[:space:]]*$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
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
      std::cout << " list" << "\t\t" << "List events in the queue. It is "
		<< "possible to filter events according to their types" 
		<< std::endl	
		<< "\t\t\t\t  'list_event BookingRequest' "
		<< "list all the booking requests" << std::endl
		<< "\t\t\t\t  'list_event BreakPoint' "
		<< "list all the break points" << std::endl;
      std::cout << " select" << "\t\t"
                << "Select an event into the 'list' to become the current one. For instance, try the command:\n"
                << "\t\t  'select 2011-May-14 00:00:00'"
                << std::endl;
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
                << "List events in the queue in a JSON format"
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
      std::ostringstream oEventListStr;

      if (lTokenListByReadline.empty() == true) { 

	// If no parameter is given, list all the events in the queue
	oEventListStr << sevmgrService.list ();

      } else if (lTokenListByReadline.size() == 1) { 

	assert (lTokenListByReadline.empty() == false);
	const std::string lEventTypeStr (lTokenListByReadline[0]);

	// If exactly one parameter is given, try to convert it into
	// an event type
	try {
	  
	  const stdair::EventType lEventType (lEventTypeStr);
	  const stdair::EventType::EN_EventType& lActualEventType =
	    lEventType.getType();
	  oEventListStr << sevmgrService.list (lActualEventType);	
	  
	} catch (stdair::CodeConversionException& e) {
	  oEventListStr << "The event type '" << lEventTypeStr
			<< "' is not known. Try 'help' for "
			<< "more information on the 'list_event' command."
			<< std::endl;
	}
      } else {
	
	// If more than one parameter is given, display an error message	
	oEventListStr << "The event type is not understood: try 'help' for "
		      << "more information on the 'list_event' command."
		      << std::endl;
      }	
      std::cout << oEventListStr.str() << std::endl;   
      STDAIR_LOG_DEBUG (oEventListStr.str());

      //
      break;
    }

      // ////////////////////////////// Select ////////////////////////
    case Command_T::SELECT: {

      //
      TokenList_T lTokenList = extractTokenListForDateTime (lTokenListByReadline);
      stdair::Date_T lUserDate = lCurrentInteractiveDateTime.date();
      stdair::Duration_T lUserTime = lCurrentInteractiveDateTime.time_of_day();
      parseEventDateTime (lTokenList, lUserDate, lUserTime);
      
      std::cout << "Try to select event: "
                << lUserDate << " " << lUserTime
                << std::endl;

      const stdair::DateTime_T lUserDateTime =
        boost::posix_time::ptime (lUserDate, lUserTime);

      const bool hasSelectBeenSuccessful =
        sevmgrService.select (lCurrentInteractiveEventStruct,
                              lUserDateTime);

      std::cout << "Selection successful: "
                << hasSelectBeenSuccessful << std::endl;

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

      // Delegate the call to the dedicated service
      sevmgrService.run (lCurrentInteractiveEventStruct);
      lCurrentInteractiveEventType = lCurrentInteractiveEventStruct.getEventType ();

      // DEBUG 
      if (lCurrentInteractiveEventType == stdair::EventType::BRK_PT) {
        std::ostringstream oBreakPointStr;
        oBreakPointStr << "Break point found. Stop at: '" 
                       << lCurrentInteractiveEventStruct.describe() << "'.";
        std::cout << oBreakPointStr.str() << std::endl;
        STDAIR_LOG_DEBUG (oBreakPointStr.str());
      } else {
        std::ostringstream oNoBreakPointStr;
        oNoBreakPointStr << "No break point found. All the events have been played.\n"
                         << "The current event is the last one.";
        std::cout << oNoBreakPointStr.str() << std::endl;
        STDAIR_LOG_DEBUG (oNoBreakPointStr.str());
      }
                
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
