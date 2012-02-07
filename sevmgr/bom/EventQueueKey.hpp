#ifndef __SEVMGR_BOM_EVENTQUEUEKEY_HPP
#define __SEVMGR_BOM_EVENTQUEUEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_event_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>
//SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>

namespace SEVMGR {
  
  /** Key of eventqueue. */
  struct EventQueueKey : public stdair::KeyAbstract {
    
  private:
    // /////////// Default constructor //////////
    EventQueueKey () { };
    
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    EventQueueKey (const EventQueueID_T&);
    EventQueueKey (const EventQueueKey&);
    /** Destructor. */
    ~EventQueueKey ();
    
    // /////////// Getters //////////
    /** Get the ID of the EventQueue object. */
    const EventQueueID_T& getEventQueueID() const {
      return _eventQueueID;
    }

    // /////////// Display support methods /////////
    /** Dump a Business Object Key into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;
    
    /** Read a Business Object Key from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);
      
   /** Get the serialised version of the Business Object Key.
       <br>That string is unique, at the level of a given Business Object,
       when among children of a given parent Business Object.
       <br>For instance, "H" and "K" allow to differentiate among two
       marketing classes for the same segment-date. */
    const std::string toString() const;

    
  private:
    // //////////// Attributes ////////////////
    /** ID for the EventQueue object. */
    EventQueueID_T _eventQueueID;
  };

}
#endif // __SEVMGR_BOM_EVENTQUEUEKEY_HPP
