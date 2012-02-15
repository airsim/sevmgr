#ifndef __SEVMGR_BOM_BOMJSONEXPORT_HPP
#define __SEVMGR_BOM_BOMJSONEXPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost Property Tree
#if BOOST_VERSION >= 104100
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif // BOOST_VERSION >= 104100
// StdAir
#include <stdair/stdair_service_types.hpp>

#if BOOST_VERSION >= 104100
  namespace bpt = boost::property_tree;
#else // BOOST_VERSION >= 104100
  namespace bpt {
    typedef char ptree;
  }
#endif // BOOST_VERSION >= 104100

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
     */
    static void jsonExportEventQueue (stdair::STDAIR_ServicePtr_T&,
				      std::ostream&, 
				      const EventQueue&);
    
  };

}
#endif // __SEVMGR_BOM_BOMJSONEXPORT_HPP
