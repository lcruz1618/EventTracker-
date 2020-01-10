#include "EventTracker.h"
#include "Plugin.h"
#include <iostream>

FlexBank::Pointer
EventTracker::EventTrackerCreate (const FlexBankInfo& info)
{
    return FlexBank::Pointer ((info.type & PLUGIN_TYPE) ? new EventTracker (info) : NULL);
}

void
EventTracker::EventTrackerDestroy (void)
{

}

EventTracker::EventTracker (const FlexBankInfo& info)
{
    mBankInfo = info;

    std::string lang_domain ("tn_i18n");
    std::string lang_path (mBankInfo.path + "//lang");
    i18n::Instance ()->SetParam (lang_domain, lang_path);

    mpBankUI = new EventTrackerUI;
    mpBankMemory = new EventTrackerMemory;
    mpEventTrackerCalc = new EventTrackerCalc;
    mpBankRunUI = new FlexBankRunPage (_("EventTracker Run Page"),
                                       _("Variable"), _("Value"), _("Time"),
                                       _("Legend"),
                                       _("good"), _("bad"), _("uncertain"));
}

EventTracker::EventTracker (const EventTracker& other) :
mpBankUI (other.mpBankUI),
mpBankMemory (other.mpBankMemory)
{
    mBankInfo = other.mBankInfo;
}

EventTracker::EventTracker()
{

}

EventTracker::~EventTracker()
{

}

EventTracker&
EventTracker::operator= (const EventTracker& other)
{
    mpBankUI = other.mpBankUI;
    mpBankMemory = other.mpBankMemory;

    mBankInfo = other.mBankInfo;

    return *this;
}

const Glib::ustring
EventTracker::GetDescription () const
{
    return mpBankMemory->GetDescription ();
}

const Glib::RefPtr<Gdk::Pixbuf>
EventTracker::GetIcon () const
{
    return mpBankMemory->GetIcon ();
}

const Glib::ustring
EventTracker::GetNewName () const
{
    return mpBankUI->GetNameEntry ();
}

bool
EventTracker::CompareGUIConfWithMemory (const FlexGUI::TreeWay& way_to)
{
    std::cout << "CompareGUI" << std::endl;

    return false;
}

//! Function called whenever the user quits clicking Accept button.
//! Saves information from the EventTracker bank.
//! @param way_to
//! @return TRUE if data was successfully recorded on bank memory.
//!
bool
EventTracker::SaveChanges (const FlexGUI::TreeWay& way_to)
{
    auto l_processing_configuration = mpBankUI->GetEventTrackerAlgorithmConfiguration ();
    mpBankMemory->SetSysConfig (l_processing_configuration);

    auto l_variables_map = mpBankUI->GetVariablesConfiguration ();
    mpBankMemory->SetVariablesList (l_variables_map);

    return true;
}

bool
EventTracker::CancelChanges (const FlexGUI::TreeWay& way_to)
{
    std::cout << "Cancel Changes" << std::endl;

    if (way_to.empty ()) return false;

    FlexGUI::TreeWay m_way_to (way_to);

    Glib::ustring m_name (m_way_to.begin ()->second);

    if (m_name.empty ()) return false;

    if (m_name != GetName ()) return false;

    m_way_to.erase (m_way_to.begin ());

    if (m_way_to.empty ())
    {
        return true;
    }
    // no bank child
    return false;

    //mpBankUI->Clear ();
}

void
EventTracker::WriteInputData (const FlexBankData& data)
{
    mpBankMemory->SetInputData (data);
}
#include <iostream>
void
EventTracker::ProcessData ()
{
    std::cout << "\n Process Data" << std::endl;

    // Variable list containing their information, such as threshold, Is analog, etc.
    std::map<Glib::ustring, EventTrackerVariable> l_eventtracker_varlist;
    l_eventtracker_varlist = mpBankMemory->GetVariablesList();

    FlexBankData data = mpBankMemory->GetInputData ();

    // EventTracker Calculation maps
    std::map <Glib::ustring, OCItemVariable> l_oc_varlist;

    std::cout << "Var list size: " << l_eventtracker_varlist.size() << std::endl;

    for (auto it = l_eventtracker_varlist.begin (); it != l_eventtracker_varlist.end (); ++it)
    {
        // If variable not selected, exit
        if ( !it->second.is_selected ) continue;

        auto iter = data.find (it->second.variable_name);

        // if variable not found, exit
        if ( iter == data.end () )
        {
            continue;
        }

        // Set item identifier, whether it's name or alias
        OCItemVariable item ( iter->second );

        if ( it->second.variable_alias.empty () )
        {
            item.SetID (it->second.variable_name);
        }
        else
        {
            item.SetID (it->second.variable_alias);
        }

//        mpBankMemory->StoreData (item);

        // Fill map with variables
        l_oc_varlist.emplace (item.GetID (), item);
    }

    // Set system config into EventTrackerCalc Class
    mpEventTrackerCalc->SetSystemConfig (mpBankMemory->GetSysConfig ());

    // Discretization of variables
    mpEventTrackerCalc->DiscretizeVar (l_oc_varlist, l_eventtracker_varlist);

    // Copying discretized values in order to transform them to toggle method
    std::map<Glib::ustring, OCItemVariable> l_oc_varlist_toggle;

    l_oc_varlist_toggle = l_oc_varlist;

    mpEventTrackerCalc->DiscretizeVarToggle (l_oc_varlist_toggle, l_eventtracker_varlist);

    // Normal or toggled score?
    bool toggle = false;

    // Map with variables scores
    std::map<Glib::ustring, double> scores;
    scores = mpEventTrackerCalc->GetNormScore (toggle);

    // Iterate through the map of variables
    for (auto it : scores)
    {
        OCTime Now;
        Now.SetCurrentTime();
        OCItemVariable item (it.first);
        item.SetValue (it.second);

        item.SetTimeStamp (Now);
        // Store each variable on FlexBankData
        mpBankMemory->StoreData (item);
    }
}

const FlexBankData
EventTracker::ReadOutputData () const
{
    return mpBankMemory->GetStoredData ();
}

void
EventTracker::SetParentGroupName (const Glib::ustring& group)
{
    mpBankMemory->SetParentGroupName(group);
}

const Glib::ustring
EventTracker::GetParentGroupName () const
{
    return mpBankMemory->GetParentGroupName();
}

const ConfiguredVariables
EventTracker::GetConfiguredVariables () const
{
    return mpBankMemory->GetConfiguredVariables ();
}

//! @sa FlexBank::SetPrevConfiguredVariables
//!
void
EventTracker::SetPrevConfiguredVariables (const ConfiguredVariables& var_list)
{
    mpBankMemory->SetPrevConfiguredVariables (var_list);
}


const FlexGUI::PagePair
EventTracker::GetEditPage (const FlexGUI::TreeWay& way_to)
{
    FlexGUI::PagePair edit_pair (FlexGUI::PAGE_NONE, (Gtk::Widget*)NULL);

    if (way_to.empty ())
    {
        return edit_pair;
    }

    FlexGUI::TreeWay m_way_to (way_to);
    Glib::ustring m_name (m_way_to.begin ()->second);

    if (m_name.empty () == true || m_name != GetName ())
    {
        return edit_pair;
    }
    else
    {
        m_way_to.erase (m_way_to.begin ());
    }

    if (m_way_to.empty () == true)
    {
        mpBankUI->SetNameEntry (GetName ());
        mpBankUI->SetDescriptionEntry (mpBankMemory->GetDescription ());
        mpBankUI->FillEditPage (mpBankMemory->GetPrevConfiguredVariables(),
                                mpBankMemory->GetVariablesList (),
                                mpBankMemory->GetSysConfig());

        return FlexGUI::PagePair (FlexGUI::PAGE_ACCEPT_CANCEL,
                                  static_cast<Gtk::Widget*>(mpBankUI));
    }

    // no bank child FlexGUI::PagePair
    return edit_pair;
}

//! @param bankDoc XML element for containing bank data.
//! @param True if save is successful
//!
bool
EventTracker::SaveBank (xmlpp::Document* bankDoc)
{
    try
    {
        if (bankDoc == nullptr)
        {
            throw OCError (__LINE__);
        }

        // create element for current bank
        xmlpp::Element* bankElement = bankDoc->create_root_node (XML::FLEX_BANK);
        if (bankElement == nullptr)
        {
            throw OCError (__LINE__);
        }

        // xmlpp needed resources
        xmlpp::Attribute* att;
        xmlpp::Element* elem;

        // set user name
        Glib::ustring user_name = GetName ();
        if (user_name.empty () == true)
        {
            throw OCError (__LINE__);
        }
        att = bankElement->set_attribute (XML::FLEX_BANK_ATT_NAME, user_name);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set plugin unique name
        Glib::ustring plugin_name = GetPluginName ();
        if (plugin_name.empty () == true)
        {
            throw OCError (__LINE__);
        }
        att = bankElement->set_attribute (XML::FLEX_BANK_ATT_PLUGIN, plugin_name);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set plugin version
        Glib::ustring plugin_version = GetPluginVersion ();
        if (plugin_version.empty () == true)
        {
            throw OCError (__LINE__);
        }
        att = bankElement->set_attribute (XML::FLEX_BANK_ATT_PLUGIN_VERSION, plugin_version);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set bank type
        BankType::BankTypes btype = GetType ();
        if ( ! (btype & BankType::ALL))
        {
            throw OCError (__LINE__);
        }
        att = bankElement->set_attribute(XML::FLEX_BANK_ATT_TYPE, OCDTConvert::toUString<BTYPE_DT> (btype));
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set parent group name
        Glib::ustring parent = GetParentGroupName ();
        if (parent.empty () == true)
        {
            throw OCError (__LINE__);
        }
        att = bankElement->set_attribute (XML::FLEX_BANK_ATT_PARENT_GROUP,parent);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set bank description
        elem = bankElement->add_child_element (XML::FLEX_BANK_ATT_DESCRIPTION);
        if (elem == nullptr)
        {
            throw OCError (__LINE__);
        }
        Glib::ustring description_text = GetDescription ();
        elem->add_child_text (description_text);

        // set EventTracker variables list
        xmlpp::Element* EventTrackerVarElem = bankElement->add_child_element (ETXML::EVTRACKER_VAR_LIST);
        if (elem == nullptr)
        {
            throw OCError (__LINE__);
        }

        // set EventTracker system configuration
        xmlpp::Element* EventTrackerSysConfigElem = bankElement->add_child_element (ETXML::EVTRACKER_SYS_CONFIG);
        if (elem == nullptr)
        {
            throw OCError (__LINE__);
        }


        // Get system configuration
        EventTrackerConfig l_eventtracker_systemconfig = mpBankMemory->GetSysConfig();

        // search slot attribute
        Glib::ustring l_searchslot_text;

        l_searchslot_text = OCDTConvert::toUString (l_eventtracker_systemconfig.mSearchSlot);
        att = EventTrackerSysConfigElem->set_attribute (ETXML::EVTRACKER_SYS_CONFIG_ATT_SEARCHSLOT, l_searchslot_text);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        // update rate attribute
        Glib::ustring l_sysupdaterate_text;

        l_sysupdaterate_text = OCDTConvert::toUString (l_eventtracker_systemconfig.mSysUpdateRate);
        att = EventTrackerSysConfigElem->set_attribute (ETXML::EVTRACKER_SYS_CONFIG_ATT_UPDATERATE, l_sysupdaterate_text);
        if (att == nullptr)
        {
            throw OCError (__LINE__);
        }

        auto variable_list = mpBankMemory->GetVariablesList();
        // Iterate through the Memory and add all the attributes for each variable
        for (auto it = variable_list.begin(); it != variable_list.end(); ++it)
        {
            elem = EventTrackerVarElem->add_child_element (ETXML::EVTRACKER_VARIABLE);
            if (elem == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mIsSelected Attribute
            Glib::ustring selected;
            if (it->second.is_selected)
            {
                selected = "true";
            }
            else
            {
                selected = "false";
            }
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_SELECTED, selected);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mIsAnalog Attribute
            Glib::ustring analog;
            if (it->second.is_analog)
            {
                analog = "true";
            }
            else
            {
                analog = "false";
            }
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_ANALOG, analog);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mIsOutput Attribute
            Glib::ustring output;
            if (it->second.is_output)
            {
                output = "true";
            }
            else
            {
                output = "false";
            }
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_OUTPUT, output);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mName Attribute
            if (it->second.variable_name.empty () == true)
            {
                throw OCError (__LINE__);
            }
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_NAME, it->second.variable_name);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mAlias Attribute
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_ALIAS, it->second.variable_alias);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }

            // mThreshold Attribute
            Glib::ustring l_threshold_text;

            l_threshold_text = OCDTConvert::toUString (it->second.state_analog_threshold);
            att = elem->set_attribute (ETXML::EVTRACKER_VARIABLE_ATT_THRESHOLD, l_threshold_text);
            if (att == nullptr)
            {
                throw OCError (__LINE__);
            }
        }
    }
    catch (xmlpp::exception& err)
    {
        throw OCError (__LINE__, err.what ());
    }
    catch (OCError& err)
    {
        throw err;
    }

    return true;
}

bool
EventTracker::LoadBank (xmlpp::Document* bankDoc)
{

    try
    {
        if (bankDoc == nullptr)
        {
            throw OCError (__LINE__);
        }
        // Get element from bank to load
        xmlpp::Element* bankElement = bankDoc->get_root_node ();

        if (bankElement == nullptr)
        {
            throw OCError (__LINE__);
        }

        if (bankElement->get_name () != XML::FLEX_BANK)
        {
            throw OCError (__LINE__);
        }

        // Check for description node
        xmlpp::Node::NodeList l_node_description;
        l_node_description = bankElement->get_children (XML::FLEX_BANK_ATT_DESCRIPTION);

        if (l_node_description.size () == 1)
        {
            xmlpp::Node::NodeList::iterator iter = l_node_description.begin ();
            xmlpp::Element *elem = dynamic_cast<xmlpp::Element*>(*iter);

            if (elem != nullptr)
            {

                if (elem->has_child_text () != false)
                {
                    // get text in node
                    xmlpp::TextNode* l_description = elem->get_first_child_text ();

                    if (l_description != nullptr)
                    {
                        // record on bank memory
                        mpBankMemory->SetDescription (l_description->get_content());
                    }
                }
            }
        }

        // Check for EventTracker System Configuration node
        xmlpp::Node::NodeList l_node_sysconfig;
        l_node_sysconfig = bankElement->get_children (ETXML::EVTRACKER_SYS_CONFIG);

        // If there is any childs
        if (l_node_sysconfig.size () == 1)
        {
            xmlpp::Element *elem = dynamic_cast<xmlpp::Element*>(*l_node_sysconfig.begin ());

            if (elem != nullptr)
            {
                EventTrackerConfig l_sysconfig;
                Glib::ustring l_att_value;

                // "Search Slot attribute" (Protecting the string->double conversion)
                l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_SYS_CONFIG_ATT_SEARCHSLOT);
                if ( !l_att_value.empty() )
                {
                    l_sysconfig.mSearchSlot = OCDTConvert::fromUString<double> (l_att_value);
                }

                // "Update Rate attribute" (Protecting the string->double conversion)
                l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_SYS_CONFIG_ATT_UPDATERATE);
                if ( !l_att_value.empty() )
                {
                    l_sysconfig.mSysUpdateRate = OCDTConvert::fromUString<double> (l_att_value);
                }


                mpBankMemory->SetSysConfig(l_sysconfig);
            }
        }

        // Check for EventTracker Variables List node
        xmlpp::Node::NodeList l_node_var_list = bankElement->get_children (ETXML::EVTRACKER_VAR_LIST);

        if (l_node_var_list.size () == 1)
        {
            std::map<Glib::ustring, EventTrackerVariable> l_eventtracker_varlist;

            // Iterate through the variables list
            xmlpp::Element* l_node_varlist_elem = dynamic_cast<xmlpp::Element*> (*l_node_var_list.begin());
            xmlpp::Node::NodeList l_node_var = l_node_varlist_elem->get_children (ETXML::EVTRACKER_VARIABLE);

            for (xmlpp::Node::NodeList::iterator iter = l_node_var.begin (); iter != l_node_var.end (); ++iter)
            {
                xmlpp::Element* elem = dynamic_cast<xmlpp::Element*> (*iter);

                if (elem != nullptr)
                {
                    Glib::ustring l_att_value;
                    EventTrackerVariable l_eventtracker_var;

                    // "isSelected" attribute
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_SELECTED);
                    if (l_att_value == "true")
                    {
                        l_eventtracker_var.is_selected = true;
                    }
                    else if (l_att_value == "false")
                    {
                        l_eventtracker_var.is_selected = false;
                    }

                    // "isAnalog" attribute
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_ANALOG);
                    if (l_att_value == "true")
                    {
                        l_eventtracker_var.is_analog = true;
                    }
                    else if (l_att_value == "false")
                    {
                        l_eventtracker_var.is_analog = false;
                    }

                    // "isOutput" attribute
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_OUTPUT);
                    if (l_att_value == "true")
                    {
                        l_eventtracker_var.is_output = true;
                    }
                    else if (l_att_value == "false")
                    {
                        l_eventtracker_var.is_output = false;
                    }

                    // "Name" attribute
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_NAME);
                    l_eventtracker_var.variable_name = l_att_value;

                    // "Alias" attribute
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_ALIAS);
                    l_eventtracker_var.variable_alias = l_att_value;

                    // "Threshold" attribute (Protecting the string->double conversion)
                    l_att_value = elem->get_attribute_value (ETXML::EVTRACKER_VARIABLE_ATT_THRESHOLD);
                    if ( !l_att_value.empty() )
                    {
                        l_eventtracker_var.state_analog_threshold = OCDTConvert::fromUString<double> (l_att_value);
                    }

                    // insert variable into variable list
                    l_eventtracker_varlist.emplace (l_eventtracker_var.variable_name, l_eventtracker_var);
                }
            }

            mpBankMemory->SetVariablesList (l_eventtracker_varlist);
        }
    }
    catch (xmlpp::exception& err)
    {
        throw OCError (__LINE__, err.what ());
    }
    catch (OCError& err)
    {
        throw err;
    }
    return true;
}
