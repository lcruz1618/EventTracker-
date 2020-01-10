#ifndef PLUGIN_API
    #ifdef _WIN32
        #ifdef EVENTTRACKERBANK_EXPORTS
            #define PLUGIN_API __declspec(dllexport)
        #else
            #define PLUGIN_API __declspec(dllimport)
        #endif
    #elif __unix__
        #ifndef PLUGIN_API
            #define PLUGIN_API
        #endif
    #endif
#endif // PLUGIN_API


#define PLUGIN_NAME "EventTracker"
#define PLUGIN_TYPE BankType::TRANSFORM
#define PLUGIN_VERSION "1.0"
