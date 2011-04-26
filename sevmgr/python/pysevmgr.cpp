// STL
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost String
#include <boost/python.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>

namespace SEVMGR {

  struct PYEventQueueManager {
  public:
    /** Wrapper around the travel demand generation use case. */
    std::string sevmgr() {
      std::ostringstream oStream;

      // Sanity check
      if (_logOutputStream == NULL) {
        oStream << "The log filepath is not valid." << std::endl;
        return oStream.str();
      }
      assert (_logOutputStream != NULL);
      
      try {

        // DEBUG
        *_logOutputStream << "Default service" << std::endl;
      
        if (_sevmgrService == NULL) {
          oStream << "The Sevmgr service has not been initialised, "
                  << "i.e., the init() method has not been called "
                  << "correctly on the PYEventQueueManager object. Please "
                  << "check that all the parameters are not empty and "
                  << "point to actual files.";
          *_logOutputStream << oStream.str();
          return oStream.str();
        }
        assert (_sevmgrService != NULL);
        
        // Do the sevmgr
        _sevmgrService->buildSampleBom();

        // DEBUG
        *_logOutputStream << "Default service returned" << std::endl;

        // DEBUG
        *_logOutputStream << "Sevmgr output: " << oStream.str() << std::endl;

      } catch (const stdair::RootException& eSevmgrError) {
        *_logOutputStream << "Sevmgr error: "  << eSevmgrError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      return oStream.str();
    }

  public:
    /** Default constructor. */
    PYEventQueueManager() : _sevmgrService (NULL), _logOutputStream (NULL) {
    }
    
    /** Default copy constructor. */
    PYEventQueueManager (const PYEventQueueManager& iPYEventQueueManager)
      : _sevmgrService (iPYEventQueueManager._sevmgrService),
        _logOutputStream (iPYEventQueueManager._logOutputStream) {
    }

    /** Default constructor. */
    ~PYEventQueueManager() {
      _sevmgrService = NULL;
      _logOutputStream = NULL;
    }
    
    /** Wrapper around the search use case. */
    bool init (const std::string& iLogFilepath,
               const std::string& iDBUser, const std::string& iDBPasswd,
               const std::string& iDBHost, const std::string& iDBPort,
               const std::string& iDBDBName) {
      bool isEverythingOK = true;

      try {
        
        // Check that the file path given as input corresponds to an actual file
        const bool isWriteable = (iLogFilepath.empty() == false);
        // stdair::BasFileMgr::isWriteable (iLogFilepath);
        if (isWriteable == false) {
          isEverythingOK = false;
          return isEverythingOK;
        }
        
        // Set the log parameters
        _logOutputStream = new std::ofstream;
        assert (_logOutputStream != NULL);
        
        // Open and clean the log outputfile
        _logOutputStream->open (iLogFilepath.c_str());
        _logOutputStream->clear();
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialisation" << std::endl;
        const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG,
                                               *_logOutputStream);
        
        // Initialise the context
        stdair::BasDBParams lDBParams (iDBUser, iDBPasswd, iDBHost, iDBPort,
                                       iDBDBName);
        _sevmgrService = new SEVMGR_Service (lLogParams, lDBParams);
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;
        
      } catch (const stdair::RootException& eSevmgrError) {
        *_logOutputStream << "Sevmgr error: "  << eSevmgrError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }
      
      return isEverythingOK;
    }

  private:
    /** Handle on the Sevmgr services (API). */
    SEVMGR_Service* _sevmgrService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(libpysevmgr) {
  boost::python::class_<SEVMGR::PYEventQueueManager> ("PYEventQueueManager")
    .def ("sevmgr", &SEVMGR::PYEventQueueManager::sevmgr)
    .def ("init", &SEVMGR::PYEventQueueManager::init);
}
