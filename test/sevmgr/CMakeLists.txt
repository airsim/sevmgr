#
# SEvMgr Tests
#

##
# Specify each test suite configuration

# * EventQueueManagementTestSuite Test Suite
module_test_add_suite (sevmgr EventQueueManagementTestSuite 
  EventQueueManagementTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

