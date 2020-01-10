#ifndef MEMORY_DEFINITIONS_H__
#define MEMORY_DEFINITIONS_H__

//! @brief EventTracker base memory structure.
//!
//! It defines the minimum data related with event tracker variable.
//!
struct EventTrackerVariable
{
    bool is_selected;

    bool is_analog;

    bool is_output;

    Glib::ustring variable_name;

    Glib::ustring variable_alias;

    double state_analog_threshold;
};

struct EventTrackerConfig
{
    double mSearchSlot;

    double mSysUpdateRate;
};

    typedef std::map <Glib::ustring, OCItemVariable> VarListMap;

#endif // MEMORY_DEFINITIONS_H__
