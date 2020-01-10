#ifndef EVENTTRACKER_UI_H__
#define EVENTTRACKER_UI_H__

#ifdef _MSC_VER
#pragma warning (disable : 4099) // Gtk::PaperSizeTraits
#endif // _MSC_VER
#include <gtkmm.h>
#include <gui\FlexBankUI.h>

#include "MemoryDefinitions.h"


class EventTrackerUI : public FlexBankUI
{
    public:
        //! @brief Constructor
        //!
        EventTrackerUI ();

        //! @brief Destructor
        //!
        ~EventTrackerUI ();

        //! @brief Fill edit page widget with information from previous bank and previous saved memory.
        //!
        void FillEditPage (const ConfiguredVariables& previous_variables,
                           const std::map<Glib::ustring, EventTrackerVariable>& memory_variables,
                           const EventTrackerConfig& EventTrackerSystemConfig);


        //! @brief On exit, save selected variables into memory
        //! @return List of Variables
        //!
        std::map<Glib::ustring, EventTrackerVariable> GetVariablesConfiguration () const;

        EventTrackerConfig GetEventTrackerAlgorithmConfiguration () const;


    protected:


    private:
        //Tree model columns
        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
            // columns model constructor
            ModelColumns ()
            {
                add (mColumnSelectable);
                add (mColumnSelect);
                add (mColumnIsOutput);
                add (mColumnName);
                add (mColumnIsAnalog);
                add (mColumnThreshold);
                add (mColumnAlias);
            }

            //! Item to check if selectable.
            //!
            Gtk::TreeModelColumn<bool> mColumnSelectable;

            //! @brief Selected item ticket.
            //!
            Gtk::TreeModelColumn<bool> mColumnSelect;

            //! @brief Is output information
            //!
            Gtk::TreeModelColumn<bool> mColumnIsOutput;

            //! @brief Name of the previous-bank item
            //!
            Gtk::TreeModelColumn<Glib::ustring> mColumnName;

            //! @brief Is analog variable ticket.
            //!
            Gtk::TreeModelColumn<bool> mColumnIsAnalog;

            //! @brief Analog threshold value.
            //!
            Gtk::TreeModelColumn<double> mColumnThreshold;

            //! @brief Item alias name.
            //!
            Gtk::TreeModelColumn<Glib::ustring> mColumnAlias;
        };

        //! @brief Model for the treeview columns.
        //!
        ModelColumns mColumns;

        //! @brief Treeview data container
        //!
        Glib::RefPtr<Gtk::ListStore> mRefTreeModel;

        //! @brief Treeview graphical widget.
        //!
        Gtk::TreeView mTreeVariables;

        /**
        * Widget's declaration: *
                               **/

        Gtk::Box mMainBox;

        Glib::ustring mBankPath;

        Gtk::ComboBoxText mComboBoxOutputVariable;

        Gtk::CheckButton mCheckButtonIsOutputAnalog;

        Gtk::Entry mEntryOutputThreshold;

        Gtk::Entry mEntrySearchSlot;

        Gtk::Entry mEntrySystemUpdateRate;

        Gtk::Entry mEntryVariableName;

        Gtk::CheckButton mCheckButtonISAnalog;

        Gtk::Entry mEntryVariableThreshold;

        Gtk::Entry mEntryVariableAlias;

        Gtk::Button mButtonSaveInputVariableInfo;

        Glib::ustring m_active_output_name;

        void EventIsAnalogClick ();

        //! @brief Variable Selection Handler
        //! @param event X-event argument
        //! @return Execution Status
        //!
        bool HandlerVariableTreeviewSelection (GdkEventButton* event);

        //! @brief On save button click, save the variable info that has been written by the user
        //!
        void HandlerSaveVariableInformationClick ();

        //! @brief After Output Var selected, constructs TreeView without it
        //! @param Selected Var to be excluded from the TreeView
        void HandlerOutputSelection ();


    };

#endif // EVENTTRACKER_UI_H__
