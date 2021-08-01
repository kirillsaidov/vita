#include "vita/logger/flogger.h"

// file logger struct with number of files opened
struct FLogger {
    FILE** handle;
    size_t nfiles;
};

// file loggers
flogger_t floggers;

// file logger save directory
str_t floggerSaveDirectory; // path_build("", "")






















