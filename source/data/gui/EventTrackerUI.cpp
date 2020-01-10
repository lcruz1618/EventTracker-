#include "EventTrackerUI.h"
#include "Plugin.h"
#include <iostream>

EventTrackerUI::EventTrackerUI () :
FlexBankUI(),
mMainBox (Gtk::ORIENTATION_VERTICAL)
{
    // Create base container for module/plugin
    add(mMainBox);

    // Auxiliary HBox & VBox & Labels declaration
    Gtk::Box* lp_Aux_Hbox;
    Gtk::Box* lp_Aux_Vbox;
    Gtk::Label* lp_Aux_Label;
    Gtk::Frame* lp_Aux_Frame;

    // Add plugin's title label
    lp_Aux_Label = Gtk::manage (new Gtk::Label);
    lp_Aux_Label->set_markup ("<span size=\"large\"><b>"+ _("Event Tracker") + "</b></span>");
    mMainBox.pack_start (*lp_Aux_Label, false, false, 3);

    // Combo Box (Output Var selection)

    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Label  = Gtk::manage (new Gtk::Label (_("Output Variable")));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, false, false, 2);
    lp_Aux_Hbox->pack_start (mComboBoxOutputVariable, true, true, 2);
    mComboBoxOutputVariable.signal_changed().connect
                (sigc::mem_fun (*this, &EventTrackerUI::HandlerOutputSelection));
    mMainBox.pack_start (*lp_Aux_Hbox, false, false, 5);

    // Analog Button & Threshold
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    mCheckButtonIsOutputAnalog.set_halign(Gtk::Align::ALIGN_CENTER);
    lp_Aux_Hbox->pack_start (mCheckButtonIsOutputAnalog, false, false, 2);
    lp_Aux_Label = Gtk::manage (new Gtk::Label ("Analog"));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, false, false, 2);

    mCheckButtonIsOutputAnalog.signal_toggled().connect (sigc::mem_fun (*this, &EventTrackerUI::EventIsAnalogClick));
    mEntryOutputThreshold.set_placeholder_text ("Output Threshold");
    mEntryOutputThreshold.set_sensitive (false);
    lp_Aux_Hbox->pack_start (mEntryOutputThreshold, false, false, 5);
    mMainBox.pack_start (*lp_Aux_Hbox, false, false, 5);

    // Analysis Span
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Vbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    mEntrySearchSlot.set_placeholder_text ("Search Slot");
    lp_Aux_Vbox->pack_start (mEntrySearchSlot, false, false, 2);
    lp_Aux_Hbox->pack_start (*lp_Aux_Vbox, true, true, 2);

    // System Update Rate
    lp_Aux_Vbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    mEntrySystemUpdateRate.set_placeholder_text ("System Update Rate");
    lp_Aux_Vbox->pack_start (mEntrySystemUpdateRate, false, false, 2);
    lp_Aux_Hbox->pack_start (*lp_Aux_Vbox, true, true, 2);

    mMainBox.pack_start (*lp_Aux_Hbox, false, false, 2);

    /**
      *  Input Var Configuration **
                                  */
    lp_Aux_Frame = Gtk::manage (new Gtk::Frame);
    lp_Aux_Frame->set_label ("Input Variables Configuration");
    lp_Aux_Vbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    Gtk::Box* lp_InputVar_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_InputVar_Hbox->set_spacing(5);

    // TreeView Construction
    mRefTreeModel = Gtk::ListStore::create (mColumns);
    mTreeVariables.set_model (mRefTreeModel);

    Gtk::TreeView::Column* lp_Column;
    int cols_count = mTreeVariables.append_column_editable ("Sel.", mColumns.mColumnSelect);
    lp_Column = mTreeVariables.get_column (cols_count-1);
    Gtk::CellRendererToggle* lp_Renderer = static_cast<Gtk::CellRendererToggle*> (lp_Column->get_first_cell());
    lp_Column->add_attribute(*lp_Renderer, "visible", mColumns.mColumnSelectable);

    mTreeVariables.append_column ("TAG ID", mColumns.mColumnName);
    mTreeVariables.set_headers_visible (true);

    // LEFT SIDE WIDGET
    Gtk::ScrolledWindow* lp_VarWindow = Gtk::manage (new Gtk::ScrolledWindow);
    lp_VarWindow->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    lp_VarWindow->set_shadow_type (Gtk::SHADOW_ETCHED_IN);
    lp_VarWindow->set_size_request (240, 240);
    lp_VarWindow->add (mTreeVariables);

    mTreeVariables.signal_button_release_event().connect
            (sigc::mem_fun (*this, &EventTrackerUI::HandlerVariableTreeviewSelection));
    lp_InputVar_Hbox->pack_start(*lp_VarWindow, true, false, 2);

    // RIGHT SIDE WIDGET
    lp_Aux_Vbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    lp_Aux_Vbox->set_spacing(3);

    // Input Variable Name
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Label = Gtk::manage (new Gtk::Label ("Name"));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, Gtk::PackOptions::PACK_SHRINK, 0);
    mEntryVariableName.set_editable (false);
    lp_Aux_Hbox->pack_start (mEntryVariableName, Gtk::PackOptions::PACK_EXPAND_WIDGET, 0);

    lp_Aux_Vbox->pack_start (*lp_Aux_Hbox, Gtk::PackOptions::PACK_SHRINK, 0);

    // Input Variable Is analog
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Label = Gtk::manage (new Gtk::Label ("Is analog?"));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, Gtk::PackOptions::PACK_SHRINK, 0);
    lp_Aux_Hbox->pack_start (mCheckButtonISAnalog, Gtk::PackOptions::PACK_EXPAND_WIDGET, 0);

    lp_Aux_Vbox->pack_start (*lp_Aux_Hbox, Gtk::PackOptions::PACK_SHRINK, 0);

    // Input Variable Threshold
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Label = Gtk::manage (new Gtk::Label ("Threshold (Default: 60)"));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, Gtk::PackOptions::PACK_SHRINK, 0);
    lp_Aux_Hbox->pack_start (mEntryVariableThreshold, Gtk::PackOptions::PACK_EXPAND_WIDGET, 0);

    lp_Aux_Vbox->pack_start (*lp_Aux_Hbox, Gtk::PackOptions::PACK_SHRINK, 0);

    // Input Variable Alias
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    lp_Aux_Label = Gtk::manage (new Gtk::Label ("Alias"));
    lp_Aux_Hbox->pack_start (*lp_Aux_Label, Gtk::PackOptions::PACK_SHRINK, 0);
    lp_Aux_Hbox->pack_start (mEntryVariableAlias, Gtk::PackOptions::PACK_EXPAND_WIDGET, 0);

    lp_Aux_Vbox->pack_start (*lp_Aux_Hbox, Gtk::PackOptions::PACK_SHRINK, 0);

    //Save Input Var Info Button
    lp_Aux_Hbox = Gtk::manage (new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    mButtonSaveInputVariableInfo.set_label ("Save");
    lp_Aux_Hbox->pack_start (mButtonSaveInputVariableInfo, false, false, 0);
    mButtonSaveInputVariableInfo.signal_clicked().connect
            (sigc::mem_fun (*this, &EventTrackerUI::HandlerSaveVariableInformationClick));

    lp_Aux_Vbox->pack_start (*lp_Aux_Hbox, Gtk::PackOptions::PACK_SHRINK, 0);

    lp_InputVar_Hbox->pack_start (*lp_Aux_Vbox, true, false, 0);


    lp_Aux_Frame->add (*lp_InputVar_Hbox);
    mMainBox.pack_start (*lp_Aux_Frame, false, false, 2);



}

EventTrackerUI::~EventTrackerUI ()
{

}

//! The widgets contained in the Edit page are filled with variables names from previous bank and
//! data recorded on bank memory. The widgets-contained data can be further edited.
//!
//! @param previous_variables List with previous configured variables
//! @param memory_variables List with variables loaded on memory
//!
void
EventTrackerUI::FillEditPage (const ConfiguredVariables& previous_variables,
                              const std::map<Glib::ustring, EventTrackerVariable>& memory_variables,
                              const EventTrackerConfig& EventTrackerSystemConfig)
{
    // clear edit page container widgets
    // selected output combobox
    mComboBoxOutputVariable.remove_all();

    // variables treeview
    mRefTreeModel->clear ();

    // Set ComboBox Default Value
    mComboBoxOutputVariable.append ("<Select Variable>");
    mComboBoxOutputVariable.set_active_text( "<Select Variable>");

    // Set System Configuration parameters
    mEntrySearchSlot.set_text (OCDTConvert::toUString (EventTrackerSystemConfig.mSearchSlot));
    mEntrySystemUpdateRate.set_text (OCDTConvert::toUString (EventTrackerSystemConfig.mSysUpdateRate));

    // check if variable already exist in memory
    for (auto variable_name : previous_variables)
    {
        // append new row output combobox
        mComboBoxOutputVariable.append (variable_name);
        // append new row to the treeview
        auto iter = mRefTreeModel->append ();

        auto l_searched_memory_variable = memory_variables.find (variable_name);
        if (l_searched_memory_variable != memory_variables.end())
        {
            // set output selection
            auto l_is_output = l_searched_memory_variable->second.is_output;
            iter->set_value<bool> (mColumns.mColumnIsOutput, l_is_output);

            if (l_is_output == true)
            {
                mComboBoxOutputVariable.set_active_text(l_searched_memory_variable->second.variable_name);

                // if output is analog, set the configure threshold
                if (l_searched_memory_variable->second.is_analog == true)
                {
                    mCheckButtonIsOutputAnalog.set_active (true);
                    mEntryOutputThreshold.set_text
                        (OCDTConvert::toUString
                        (l_searched_memory_variable->second.state_analog_threshold));
                }
                mRefTreeModel->erase(iter);
                continue;
            }

            iter->set_value<bool> (mColumns.mColumnSelectable, true);
            iter->set_value<bool> (mColumns.mColumnSelect, l_searched_memory_variable->second.is_selected);
            iter->set_value<Glib::ustring> (mColumns.mColumnName, l_searched_memory_variable->second.variable_name);
            iter->set_value<Glib::ustring> (mColumns.mColumnAlias, l_searched_memory_variable->second.variable_alias);
            iter->set_value<double> (mColumns.mColumnThreshold, l_searched_memory_variable->second.state_analog_threshold);
            iter->set_value<bool> (mColumns.mColumnIsAnalog, l_searched_memory_variable->second.is_analog);
        }
        else
        {
            iter->set_value<bool> (mColumns.mColumnSelectable, true);
            iter->set_value<bool> (mColumns.mColumnSelect, false);
            iter->set_value<bool> (mColumns.mColumnIsOutput, false);
            iter->set_value<Glib::ustring> (mColumns.mColumnName, variable_name);
            iter->set_value<Glib::ustring> (mColumns.mColumnAlias, Glib::ustring());
            iter->set_value<double> (mColumns.mColumnThreshold, 0.0);
            iter->set_value<bool> (mColumns.mColumnIsAnalog, false);
        }
    }
}

void
EventTrackerUI::EventIsAnalogClick ()
{
    // Enabling Output Threshold entry
    if (mCheckButtonIsOutputAnalog.get_active() == true)
    {
        mEntryOutputThreshold.set_sensitive (true);
    }
    else
    {
        mEntryOutputThreshold.set_sensitive (false);
    }
    mEntryOutputThreshold.set_text("");
}

//! The function is called whenever a variable is selected on the TreeView
//! It gets the treeview selected variable and set its information from entries
//! @return True if there's a selection, false otherwise
//!
bool
EventTrackerUI::HandlerVariableTreeviewSelection (GdkEventButton* event)
{
    // Get iterator from selected item
    Glib::RefPtr<Gtk::TreeSelection> item_selection = mTreeVariables.get_selection ();
    auto lp_selection = item_selection->get_selected ();

    if (lp_selection != nullptr)
    {
        Gtk::TreeModel::Row row = *lp_selection;

        // Set data from the treeview memory
        mEntryVariableName.set_text (row[mColumns.mColumnName]);
        mCheckButtonISAnalog.set_active (row[mColumns.mColumnIsAnalog]);
        Glib::ustring l_threshold = OCDTConvert::toUString (row[mColumns.mColumnThreshold]);
        mEntryVariableThreshold.set_text (l_threshold);
        mEntryVariableAlias.set_text (row[mColumns.mColumnAlias]);

        return true;
    }
    else
    {
        return false;
    }
}

//! The function is called whenever a output variable is selected on the
//! combobox.
//!
//! It iterates the treeview variables and set the output variables as being
//! the only one selected on the combobox. The other are set to false.
//!
void
EventTrackerUI::HandlerOutputSelection ()
{
    // get the output selection
    m_active_output_name = mComboBoxOutputVariable.get_active_text();

    // Iterate through the treeview and select the output
    Gtk::TreeModel::Children l_tree_rows = mRefTreeModel->children();

    // Checking which Variables are selected from TreeView
    for(auto iter = l_tree_rows.begin(); iter != l_tree_rows.end(); ++iter)
    {
        // get specific row
        Gtk::TreeModel::Row row = *iter;

        // check if row is the same as the selected output name
        if (row[mColumns.mColumnName] == m_active_output_name)
        {
            row[mColumns.mColumnIsOutput] = true;
            row[mColumns.mColumnSelect] = true;
            //row[mColumns.mColumnSelectable] = false;
        }
        else
        {
            row[mColumns.mColumnIsOutput] = false;
        }
    }
}

//! Handler for the Save Button, each time it is clicked, saves the entered information
//! onto the treeview memory.
//!
void
EventTrackerUI::HandlerSaveVariableInformationClick ()
{
    // Get iterator from selected item
    Glib::RefPtr<Gtk::TreeSelection> item_selection = mTreeVariables.get_selection ();
    auto lp_selection = item_selection->get_selected ();

    // Check if not NULL
    if (lp_selection != nullptr)
    {
        // Get row from the iterator
        Gtk::TreeModel::Row row = *lp_selection;

        // Set data onto memory
        row[mColumns.mColumnName] = mEntryVariableName.get_text();
        row[mColumns.mColumnIsAnalog] = mCheckButtonISAnalog.get_active();
        row[mColumns.mColumnThreshold] = OCDTConvert::fromUString<double> (mEntryVariableThreshold.get_text());
        row[mColumns.mColumnAlias] = mEntryVariableAlias.get_text();
    }
}

//!
//! @return Map width variables recorded into
//!
std::map <Glib::ustring, EventTrackerVariable>
EventTrackerUI::GetVariablesConfiguration () const
{
    std::map <Glib::ustring, EventTrackerVariable> l_variables_map;

    // Get row from the treeview
    Gtk::TreeModel::Children l_tree_rows = mRefTreeModel->children();

    // Checking which Variables are selected from TreeView
    for(auto iter = l_tree_rows.begin(); iter != l_tree_rows.end(); ++iter)
    {
        EventTrackerVariable l_configured_variable;

        // get specific row
        Gtk::TreeModel::Row row = *iter;

        // check if row is selected to EventTracker_algorithm
        // memory will contain only selected variables
        if (row[mColumns.mColumnSelect] == true || row[mColumns.mColumnIsOutput] == true)
        {
            // if variable is output get from respective output entries
            if (row[mColumns.mColumnIsOutput] == true)
            {
                // copy data to variable
                l_configured_variable.is_selected = row[mColumns.mColumnSelect];
                l_configured_variable.is_output = row[mColumns.mColumnIsOutput];
                l_configured_variable.variable_name = row[mColumns.mColumnName];
                l_configured_variable.variable_alias = row[mColumns.mColumnAlias];

                // if analog also saves threshold
                if (mCheckButtonIsOutputAnalog.get_active() == true)
                {
                    l_configured_variable.is_analog = true;
                    l_configured_variable.state_analog_threshold = OCDTConvert::fromUString<double> ((mEntryOutputThreshold.get_text()));
                }
                else
                {
                    l_configured_variable.is_analog = false;
                }
            }
            else
            {
                // copy data to variable
                l_configured_variable.is_selected = row[mColumns.mColumnSelect];
                l_configured_variable.is_output = row[mColumns.mColumnIsOutput];
                l_configured_variable.variable_name = row[mColumns.mColumnName];
                l_configured_variable.variable_alias = row[mColumns.mColumnAlias];
                l_configured_variable.state_analog_threshold = row[mColumns.mColumnThreshold];
                l_configured_variable.is_analog = row[mColumns.mColumnIsAnalog];
            }
            // insert data into map
            l_variables_map.emplace (l_configured_variable.variable_name, l_configured_variable);
        }
    }

    return l_variables_map;
}

//! @return Configuration structure
//!
EventTrackerConfig
EventTrackerUI::GetEventTrackerAlgorithmConfiguration () const
{
    // variable declaration
    EventTrackerConfig l_runtime_configs;
    Glib::ustring l_entry_text;

    // search slot
    l_entry_text = mEntrySearchSlot.get_text();
    l_runtime_configs.mSearchSlot = OCDTConvert::fromUString<double> (l_entry_text);

    // system update rate
    l_entry_text = mEntrySystemUpdateRate.get_text();
    l_runtime_configs.mSysUpdateRate = OCDTConvert::fromUString<double> (l_entry_text);

    return l_runtime_configs;
}
