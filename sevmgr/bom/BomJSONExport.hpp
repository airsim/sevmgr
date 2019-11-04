#ifndef __SEVMGR_BOM_BOMJSONEXPORT_HPP
#define __SEVMGR_BOM_BOMJSONEXPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// Boost Property Tree
#if BOOST_VERSION_MACRO >= 104100
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif // BOOST_VERSION_MACRO >= 104100
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/EventTypes.hpp>

#if BOOST_VERSION_MACRO >= 104100
  namespace bpt = boost::property_tree;
#else // BOOST_VERSION_MACRO >= 104100
  namespace boost {
    namespace property_tree {
      typedef std::string ptree;
    }
  }
#endif // BOOST_VERSION_MACRO >= 104100

namespace SEVMGR {

  class EventQueue;

  /**
   * @brief Utility class to export StdAir objects in a JSON format.
   */
  
  class BomJSONExport {
  public:
    // //////////////// Export support methods /////////////////   

    /**
     * Export (dump in the underlying output log stream and in JSON format) 
     * the event struct objects contained in the event queue.
     *
     * @stdair::STDAIR_ServicePtr_T& Pointer on the StdAir service handler.
     * @param std::ostream& Output stream in which the events should be
     *        logged/dumped.
     * @param const EventQueue& Events queue to be stored in JSON-ified 
     *        format.
     * @param const stdair::EventType::EN_EventType& Filter to select objects
     *        with a certain event type.
     */
    static void jsonExportEventQueue (stdair::STDAIR_ServicePtr_T&,
				      std::ostream&, 
				      const EventQueue&,
				      const stdair::EventType::EN_EventType&);
    
  };

}
#endif // __SEVMGR_BOM_BOMJSONEXPORT_HPP
