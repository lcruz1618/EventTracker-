#ifndef EVENTTRACKER__CALC_H__
#define EVENTTRACKER_CALC_H__

#ifdef _MSC_VER
#pragma warning (disable : 4099) // Gtk::PaperSizeTraits
#endif // _MSC_VER

#include <BankManager.h>

#include "MemoryDefinitions.h"


class EventTrackerCalc
{
    public:
        //! @brief Constructor
        //!
        EventTrackerCalc ();

        //! @brief Destructor
        //!
        ~EventTrackerCalc ();

        //! @brief Discretize list of variables
        //! @param variablesvalueslist List of variables values to discretize
        //! @param l_eventtracker_variablelist List of variables containing EventTracker
        //! information, like threshold.
        //! @return Discrete output variable
        void DiscretizeVar (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
                const std::map<Glib::ustring, EventTrackerVariable>& l_eventtracker_variablelist);

        void DiscretizeVarToggle (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
                const std::map<Glib::ustring, EventTrackerVariable>& l_eventtracker_variablelist);

        std::map<Glib::ustring, int> GetScore (bool normal_toggle);

        std::map<Glib::ustring, double> GetNormScore (bool normal_toggle);

        void SetSystemConfig (EventTrackerConfig sysconfig);

        Variables& GetVariablesSensitivity ();

        void UpdateVariablesSensitivity (OCItemVariable& item);

        bool WriteDataCSV (Glib::ustring file_name, const double value,
            const double output_val, const double Score, const double NScore);

    protected:


    private:

        void SetVariablesSensitivity (const Variables&);

        //! @brief Compute sensitivity score for each variable and store into score's map
        //! @param m_variablesvalueslist List of variables values
        //!
        void CalcScore (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
                        std::map <Glib::ustring, int>& score_list,
                        std::map <Glib::ustring, double>& normscore_list,
                        double& omega_weight,
                        int& max_value,
                        std::map <Glib::ustring, bool> outputlastvalue);

        //! @brief Normalize variables through the Exponential Weighting Method
        //! @param m_variablesvalueslist List of variables to normalize
        //! @param lambda  Forgetting factor, used to calculate weighting factor
        //!
        void CalcAverageScore (const double lambda,
                               std::map <Glib::ustring, int>& score_list,
                               std::map <Glib::ustring, double>& normscore_list,
                               double& omega_weight,
                               int& max_value,
                               const double outputval,
                               const double outputvaln);

        //! @brief Fill score's maps with all the variables name
        //! @param variablesvalueslist: All the variables to fill
        void InitializeMaps (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist);

        bool fileExists(std::string& fileName);

        // Weighting factor
        double m_omega_weight;
        double m_omega_weight_toggled;
        int m_max_value_score;
        int m_max_value_score_toggled;


        std::map <Glib::ustring, OCItemVariable> m_variableslist_lastvalue;

        std::map <Glib::ustring, bool> m_variableslist_lastvalue_toggled;

        std::map <Glib::ustring, bool> m_variableslist_outputlastvalue;

        std::map <Glib::ustring, bool> m_variableslist_outputlastvalue_toggled;

        std::map <Glib::ustring, int> m_variableslist_score;

        std::map <Glib::ustring, int> m_variableslist_score_toggled;

        std::map <Glib::ustring, double> m_variableslist_normscore;

        std::map <Glib::ustring, double> m_variableslist_normscore_toggled;

        EventTrackerConfig m_sysconfig;
};

#endif // EVENTTRACKER_CALC_H__

