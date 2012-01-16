// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/service/Logger.hpp>
// SEvMgr
#include <sevmgr/command/EventQueueManager.hpp>

namespace SEVMGR {

  // //////////////////////////////////////////////////////////////////////
  void EventQueueManager::
  buildSampleBom (stdair::EventQueue& ioEventQueue) {
  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::reset (stdair::EventQueue& ioEventQueue) {

    /**
     * Reset the EventQueue object.
     */
    ioEventQueue.reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void EventQueueManager::addEvent (stdair::EventQueue& ioEventQueue,
                                    stdair::EventStruct& iEventStruct) {

    /**
     * Add the event to the event queue.
     */
    ioEventQueue.addEvent(iEventStruct);
  }


}
