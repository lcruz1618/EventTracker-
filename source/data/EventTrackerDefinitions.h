#ifndef EVENTTRACKER_DEFINITIONS_H__
#define EVENTTRACKER_DEFINITIONS_H__

//Glib::ustring
//int2string (const int number)
//{
//    std::stringstream str;
//    str << number;
//    return Glib::ustring (str.str ());
//}



namespace ETXML
{
    static const char EVTRACKER_VAR_LIST[] = "EventTrackerVarList";
    static const char EVTRACKER_VAR_LIST_ATT_SIZE[] = "size";
    static const char EVTRACKER_VARIABLE[] = "EventTrackerVariables";
    static const char EVTRACKER_VARIABLE_ATT_SELECTED[] = "selected";
    static const char EVTRACKER_VARIABLE_ATT_ANALOG[] = "analog";
    static const char EVTRACKER_VARIABLE_ATT_OUTPUT[] = "output";
    static const char EVTRACKER_VARIABLE_ATT_NAME[] = "name";
    static const char EVTRACKER_VARIABLE_ATT_ALIAS[] = "alias";
    static const char EVTRACKER_VARIABLE_ATT_THRESHOLD[] = "threshold";
    static const char EVTRACKER_SYS_CONFIG[] = "EventTrackerSysConfig";
    static const char EVTRACKER_SYS_CONFIG_ATT_SEARCHSLOT[] = "searchslot";
    static const char EVTRACKER_SYS_CONFIG_ATT_UPDATERATE[] = "updaterate";
    static const char EVTRACKER_PARENT_NAME[] = "parentname";

}


#endif // EVENTTRACKER_DEFINITIONS_H__
