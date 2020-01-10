#ifndef EVENTTRACKER_H__
#define EVENTTRACKER_H__

#ifdef _MSC_VER
#pragma warning (disable : 4099) // Gtk::PaperSizeTraits
#endif // _MSC_VER

#include <gtkmm.h>
#include <BankManager.h>
#include "EventTrackerMemory.h"
#include "EventTrackerUI.h"
#include "EventTrackerDefinitions.h"
#include "EventTrackerCalc.h"

//! @brief Implementation of the Controller (C) Class for the EventTracker module.
//!
//! The module will permit to use the correlation between variables in order to
//! identify the inner relation between process variables. Per example, to identify
//! bottlenecks on process operation.
//!
class EventTracker : public FlexBank
{
    public:
        //! @brief Pointer constructor
        //! @brief Create a flex bank pointer to extract the info of EventTracker
        //!
        static FlexBank::Pointer EventTrackerCreate (const FlexBankInfo& info);

        //! @brief EventTracker Bank destructor for "Plugin.h" call
        //!
        static void EventTrackerDestroy (void);

        EventTracker (const FlexBankInfo& info);

        EventTracker (const EventTracker& other);

        //! @brief Constructor
        //!
        EventTracker ();

        //! @brief Destructor
        //!
        ~EventTracker ();

        //! @brief Copy operator, were data from a EventTracker module will be copied to this one.
        //!
        EventTracker& operator= (const EventTracker& other);

        //! @brief
        //! @return
        //!
        virtual const Glib::ustring GetDescription () const;

        //! @brief
        //! @return
        //!
        virtual const Glib::RefPtr<Gdk::Pixbuf> GetIcon () const;

        //! @brief
        //! @return
        //!
        virtual const Glib::ustring GetNewName () const;

        //! @brief Compare GUI configurations with memory.
        //! @param way_to A FlexGUI::TreeWay to the item to compare.
        //! @return True if GUI configurations and memory are equal.
        //!
        virtual bool CompareGUIConfWithMemory (const FlexGUI::TreeWay&);

        //! @brief Save bank or item changes made in the edit page.
        //! @param way_to A FlexGUI::TreeWay to the item to save changes.
        //!
        virtual bool SaveChanges (const FlexGUI::TreeWay&);

        //! @brief Cancel bank or item changes made in the edit page.
        //! @param way_to A FlexGUI::TreeWay to the item to cancel changes.
        //!
        virtual bool CancelChanges (const FlexGUI::TreeWay&);


        //! @brief Set Input data.
        //!
        virtual void WriteInputData (const FlexBankData&);


        //!@brief Process bank data.
        //!
        virtual void ProcessData ();

        //! @brief Returns the bank stored data.
        //!
        virtual const FlexBankData ReadOutputData () const;

        //! @brief Save the name of the parent group name.
        //! @param group Name of the parent FlexGroup.
        //! @sa FlexBankMemory::SetParentGroupName
        //! @sa EventTracker::GetParentGroupName
        //!
        virtual void SetParentGroupName (const Glib::ustring&);

        //! @brief Get configured variables.
        //! @return Vector of strings with the configured variables.
        //! @sa EventTracker::mBankVariablesConfigured
        //! @sa EventTracker::SetPreviousConfiguredVariables
        //! @sa FlexBankMemory::GetConfiguredVariables
        //!
        virtual const ConfiguredVariables GetConfiguredVariables () const;

        //! @brief Set configured variables of the previous bank.
        //! @param var_list Vector of strings with the configured variables in the previous bank.
        //! @sa EventTracker::mBankVariablesConfigured
        //! @sa EventTracker::GetConfiguredVariables
        //! @sa FlexBankMemory::SetPreviousConfiguredVariables
        //!
        virtual void SetPrevConfiguredVariables (const ConfiguredVariables&);

        //! @brief Set TODO
        bool SetNewEventTrackerVariables (const Variables&);

        //! @brief Get bank or bank item edit page.
        //! @param way_to A FlexGUI::TreeWay to the item to get edit page.
        //! @return FlexGUI::PagePair
        //!
        const FlexGUI::PagePair GetEditPage (const FlexGUI::TreeWay& way_to);

        //! @brief Save FlexBank plugin information.
        //!
        virtual bool SaveBank (xmlpp::Document* bankDoc);

        //! @brief Load FlexBank plugin information.
        //!
        virtual bool LoadBank (xmlpp::Document* bankDoc);

        bool SaveDataCSV (Glib::ustring file_name,
                          const std::map<Glib::ustring, EventTrackerVariable>&,
                          const std::map<Glib::ustring, EventTrackerVariable>&);

    protected:

        //! @brief Return the name of the parent group name.
        //! @return Name of the parent FlexGroup.
        //! @sa FlexBankMemory::GetParentGroupName
        //! @sa BlankExample::SetParentGroupName
        //!
        virtual const Glib::ustring GetParentGroupName () const;

    private:

        EventTrackerUI* mpBankUI;

        EventTrackerMemory* mpBankMemory;

        FlexBankRunPage* mpBankRunUI;

        EventTrackerCalc* mpEventTrackerCalc;

};

#endif // EVENTTRACKER_H__
