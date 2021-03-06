#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#define CONFIGURATION_RISCV 1
#define CONFIGURATION_ARM 2
#define CONFIGURATION_HARDFLOAT 4
#define CONFIGURATION_OPTIMALIZATION_0 8
#define CONFIGURATION_OPTIMALIZATION_1 16
#define CONFIGURATION_OPTIMALIZATION_2 32
#define CONFIGURATION_OPTIMALIZATION_3 64

extern unsigned int get_configuration_state();

#endif /* TEST_UTILS_H_ */
