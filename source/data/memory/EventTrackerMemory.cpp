#include "EventTrackerMemory.h"


EventTrackerMemory::EventTrackerMemory ()
{

}

std::map<Glib::ustring, EventTrackerVariable>
EventTrackerMemory::GetVariablesList () const
{
    return mEventTrackerVarList;
}

//#include <iostream> //DEBUG

void
EventTrackerMemory::SetVariablesList (const std::map<Glib::ustring, EventTrackerVariable>& variables_list)
{
    mEventTrackerVarList = variables_list;
}

void
EventTrackerMemory::SetSysConfig (EventTrackerConfig& sysconfig)
{
    mEventTrackerSysConf = sysconfig;
}

EventTrackerConfig
EventTrackerMemory::GetSysConfig () const
{
    return mEventTrackerSysConf;
}

void
EventTrackerMemory::SetParentGroupName (const Glib::ustring& group)
{
    mParentName = group;
}

const Glib::ustring
EventTrackerMemory::GetParentGroupName () const
{
    return mParentName;
}

