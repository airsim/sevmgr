// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
#if BOOST_VERSION_MACRO >= 104100
// Boost Property Tree
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/regex.hpp>
#endif // BOOST_VERSION_MACRO >= 104100
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/EventStruct.hpp>
// SEVMGR
#include <sevmgr/bom/EventQueue.hpp>
#include <sevmgr/bom/BomJSONExport.hpp>

#if BOOST_VERSION_MACRO >= 104100
namespace bpt = boost::property_tree;
#else // BOOST_VERSION_MACRO >= 104100
namespace bpt {
  typedef char ptree;
}
#endif // BOOST_VERSION_MACRO >= 104100

namespace SEVMGR { 

  // ////////////////////////////////////////////////////////////////////
  void BomJSONExport::
  jsonExportEventQueue (stdair::STDAIR_ServicePtr_T& ioSTDAIR_ServicePtr,
			std::ostream& oStream,
			const EventQueue& iEventQueue,
			const stdair::EventType::EN_EventType& iEventType) { 

    // Retrieve the event list
    const stdair::EventList_T& lEventList = iEventQueue.getEventList();
 
#if BOOST_VERSION_MACRO >= 104100  
    // Create empty property tree objects
    bpt::ptree ptEvents;   
    bpt::ptree pt;  

    // Browse the events
    for (stdair::EventList_T::const_iterator itEvent = lEventList.begin();
	 itEvent != lEventList.end(); ++itEvent) {
      const stdair::EventStruct& lEvent = itEvent->second;   
      const stdair::EventType::EN_EventType& lEventType = 
	lEvent.getEventType();

      const bool isEventTypeLastValue = 
	(iEventType == stdair::EventType::LAST_VALUE);
      if (lEventType == iEventType || isEventTypeLastValue == true) {
 
	// Delegate the JSON export to the dedicated service
	const std::string lCurrentEvent = 
	  ioSTDAIR_ServicePtr->jsonExportEventObject (lEvent);  
	
	// Load the JSON formatted string into the property tree.
	// If reading fails (cannot open stream, parse error), an
	// exception is thrown. 
	if (lCurrentEvent.empty () == false) {
	  bpt::ptree ptCurrentEvent;  
	  std::istringstream lStrCurrentEvent(lCurrentEvent);
	  read_json (lStrCurrentEvent, ptCurrentEvent);	
	
	  // Put the current inventory tree in the events array
	  ptEvents.push_back(std::make_pair("", ptCurrentEvent));
	}
      }
    }

    // Store the events array tree into the global tree
    pt.add_child ("events", ptEvents); 
    
    // Write the property tree into the JSON stream.
    write_json (oStream, pt);

#endif // BOOST_VERSION_MACRO >= 104100  
  }

}
