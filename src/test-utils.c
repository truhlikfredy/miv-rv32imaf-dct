#include "test-utils.h"

// this variable can be set by different defines when using different compile
// configurations and then it can detected with GDB on runtime and distinguish
// what results the tests should use as the GDB test needs to have different
// expected results for soft/hard floats as they slightly differ in the implementation.

unsigned int get_configuration_state() {
	unsigned int ret = 0;

	ret |= CONFIGURATION_RISCV;
	ret |= CONFIGURATION_OPTIMALIZATION_0;

#ifdef NDEBUG
  ret |= CONFIGURATION_OPTIMALIZATION_3;
#else
  ret |= CONFIGURATION_OPTIMALIZATION_0;
#endif

#ifdef __riscv_flen
  ret |= CONFIGURATION_HARDFLOAT;
#endif

	return ret;
}


