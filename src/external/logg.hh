#ifndef LOGG_LOGG_HH__
#define LOGG_LOGG_HH__


#include "logg/StreamOutput.hh" // warning: transitive include
#include "logg/Timer.hh" // warning: transitive include


#define STREAM_LOG_INFO(Output)    (Output).log(logg::Info,    __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define STREAM_LOG_LOG(Output)     (Output).log(logg::Log,     __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define STREAM_LOG_WARNING(Output) (Output).log(logg::Warning, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define STREAM_LOG_ERROR(Output)   (Output).log(logg::Error,   __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define STREAM_LOG_FATAL(Output)   (Output).log(logg::Fatal,   __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define STREAM_LOG_USER(Output)    (Output).log(logg::User,    __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define COUT_INFO    STREAM_LOG_INFO(logg::cout)
#define COUT_LOG     STREAM_LOG_LOG(logg::cout)
#define COUT_WARNING STREAM_LOG_WARNING(logg::cout)
#define COUT_ERROR   STREAM_LOG_ERROR(logg::cout)
#define COUT_FATAL   STREAM_LOG_FATAL(logg::cout)
#define COUT_USER    STREAM_LOG_USER(logg::cout)

#define CERR_INFO    STREAM_LOG_INFO(logg::cerr)
#define CERR_LOG     STREAM_LOG_LOG(logg::cerr)
#define CERR_WARNING STREAM_LOG_WARNING(logg::cerr)
#define CERR_ERROR   STREAM_LOG_ERROR(logg::cerr)
#define CERR_FATAL   STREAM_LOG_FATAL(logg::cerr)
#define CERR_USER    STREAM_LOG_FATAL(logg::cerr)

#define CLOG_INFO    STREAM_LOG_INFO(logg::clog)
#define CLOG_LOG     STREAM_LOG_LOG(logg::clog)
#define CLOG_WARNING STREAM_LOG_WARNING(logg::clog)
#define CLOG_ERROR   STREAM_LOG_ERROR(logg::clog)
#define CLOG_FATAL   STREAM_LOG_FATAL(logg::clog)
#define CLOG_USER    STREAM_LOG_USER(logg::clog)


#define TIMER(Output, name) logg::Timer(Output, __FILE__, __LINE__, __PRETTY_FUNCTION__, name)
#define GLOBAL_TIMER(Output, name) logg::Timer(Output, __FILE__, __LINE__, "top level", name)

#define COUT_TIMER(name)        TIMER(logg::cout, name)
#define CERR_TIMER(name)        TIMER(logg::cerr, name)
#define CLOG_TIMER(name)        TIMER(logg::clog, name)

#define COUT_GLOBAL_TIMER(name) GLOBAL_TIMER(logg::cout, name)
#define CERR_GLOBAL_TIMER(name) GLOBAL_TIMER(logg::cerr, name)
#define CLOG_GLOBAL_TIMER(name) GLOBAL_TIMER(logg::clog, name)

#endif // LOGG_LOGG_HH__
