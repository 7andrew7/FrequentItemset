#pragma once

#include <boost/timer/timer.hpp>

// Crazy-ass pre-processor macros for timing code regions
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define SCOPED_TIMING(tag) boost::timer::auto_cpu_timer __TIMER__{std::string{tag} + \
 " [line " + TOSTRING(__LINE__)  + "] %t sec CPU, %w sec real\n"}

#define FUNCTION_TIMING SCOPED_TIMING(__func__)

#define BEGIN_TIMING(tag) { SCOPED_TIMING(tag);
#define END_TIMING }
