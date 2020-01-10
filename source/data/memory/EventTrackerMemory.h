#ifndef BLANKEXAMPLE_MEMORY_H__
#define BLANKEXAMPLE_MEMORY_H__

#include "memory\FlexBankMemory.h"
#include "MemoryDefinitions.h"


class EventTrackerMemory : public FlexBankMemory
{
public:

    EventTrackerMemory ();

    std::map<Glib::ustring, EventTrackerVariable> GetVariablesList () const;

    void SetVariablesList (const std::map<Glib::ustring, EventTrackerVariable>& variables_list);

    void SetSysConfig (EventTrackerConfig& sysconfig);

    EventTrackerConfig GetSysConfig () const;

    void SetParentGroupName (const Glib::ustring& group);

    const Glib::ustring GetParentGroupName () const;

protected:

private:
    //! @brief List contained the variables selected to perform the EventTracker algorithm.
    //!
    std::map<Glib::ustring, EventTrackerVariable> mEventTrackerVarList;

    //! @brief EventTracker System Configuration (Update Rate and Search Slot)
    //!
    EventTrackerConfig mEventTrackerSysConf;

    Glib::ustring mParentName;

};

#endif // EventTracker_MEMORY_H__
