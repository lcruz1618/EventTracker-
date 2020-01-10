#include "EventTrackerCalc.h"
#include "Plugin.h"

#include <iostream>
#include <cmath>

EventTrackerCalc::EventTrackerCalc ()
{

}

EventTrackerCalc::~EventTrackerCalc ()
{

}

void
EventTrackerCalc::DiscretizeVar (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
                               const std::map<Glib::ustring, EventTrackerVariable>& l_eventtracker_variablelist)
{
    std::cout << "\n ===================== NORMAL ===================== " << std::endl;
    // VAR INITIALIZATION
    // Initialize last value's map (first execution)
    if ( m_variableslist_lastvalue.empty() == true )
    {
        m_variableslist_lastvalue = variablesvalueslist;
    }

    // Initialize Temporary variables list to save them to last values map at the end of
    // execution when they are discretized to boolean
    std::map <Glib::ustring, OCItemVariable> l_variableslist_temp;
    l_variableslist_temp = variablesvalueslist;

    // DISCRETIZATION
    // Iterates through the variables list in order to discretize them
    for ( auto it = variablesvalueslist.begin(); it != variablesvalueslist.end(); ++it )
    {
        // Search Variable on the EventTracker's varlist
        Glib::ustring var_name = it->first;
        auto var_info = l_eventtracker_variablelist.find (var_name);

        // Exits if not found
        if ( var_info == l_eventtracker_variablelist.end() )
        {
            std::cout << "Error: Var not found on EventTracker's varlist!" << std::endl;
            return;
        }

        // If the variable IS NOT ANALOG skips it
        if ( var_info->second.is_analog == false )
        {
            continue;
        }

        // Discretization
        // Get last value (t0)
        double last_value = 0;
        auto it_lastval = m_variableslist_lastvalue.find (var_name);

        // If not found, skip variable
        if ( it_lastval == m_variableslist_lastvalue.end() )
        {
            std::cout << var_name << ": key not found in last value's map. ERROR!" << std::endl;
            continue;
        }

        // If found, set's it to double var
        it_lastval->second.GetActualValue (last_value);

        // Get actual value into double var(t)
        double value;
        it->second.GetActualValue (value);

        // Debug purpose
        std::cout << "Var: " << it->first << " = " << value << " - " << last_value << " > " << var_info->second.state_analog_threshold << "?";

        // Calculate discrete value ( |u(t) - u(t0)| > input_threshold )
        if ( std::abs (value - last_value) > var_info->second.state_analog_threshold )
        {
            it->second.SetValue (true);
            std::cout << " yes" << std::endl;
        }
        else
        {
            it->second.SetValue (false);
            std::cout << " no" << std::endl;
        }


        // If we're dealing with the output var, move it to its map
        if ( var_info->second.is_output == true )
        {
            // Get the output value
            bool outputval;
            it->second.GetActualValue (outputval);

            // Remove the old entry and insert the new output value to its map
            m_variableslist_outputlastvalue.clear();
            m_variableslist_outputlastvalue.emplace (it->first, outputval);

            // Remove it from the input variables map
//            variablesvalueslist.erase (it);
        }
    }

    // Move the actual input values to the last ones map, for the next execution
    m_variableslist_lastvalue = l_variableslist_temp;

    // Output Value Management
    if ( m_variableslist_outputlastvalue.empty() )
    {
        std::cout << "No output selected!" << std::endl;
    }

    // Calculate Score
    CalcScore (variablesvalueslist, m_variableslist_score, m_variableslist_normscore, m_omega_weight, m_max_value_score, m_variableslist_outputlastvalue);
}

void
EventTrackerCalc::DiscretizeVarToggle (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
            const std::map<Glib::ustring, EventTrackerVariable>& l_eventtracker_variablelist)
{

    // VAR INITIALIZATION
    // If there's not any var state value available (first execution), exits
    if ( m_variableslist_lastvalue_toggled.empty() )
    {
        for ( auto it = variablesvalueslist.begin(); it != variablesvalueslist.end(); ++it )
        {
            m_variableslist_lastvalue_toggled.emplace (it->first, false);
        }
        return;
    }

     std::cout << "\n ===================== TOGGLE ===================== " << std::endl;
    // DISCRETIZATION
    // Iterating through the variables list in order to discretize them
    for ( auto it = variablesvalueslist.begin(); it != variablesvalueslist.end(); ++it )
    {
        // Search Variable on the EventTracker's varlist
        Glib::ustring var_name = it->first;
        auto var_info = l_eventtracker_variablelist.find (var_name);

        // Exits if not found
        if ( var_info == l_eventtracker_variablelist.end() )
        {
            std::cout << "Error: Var not found on EventTracker's varlist!" << std::endl;
            return;
        }

        // And LAST STATE of the variable
        auto it_lastval_bool = m_variableslist_lastvalue_toggled.find (var_name);

        // If not found, skip variable (BOOL)
        if ( it_lastval_bool == m_variableslist_lastvalue_toggled.end() )
        {
            std::cout << var_name << ": key not found in last bool value's map. ERROR!" << std::endl;
            continue;
        }

        // otherwise gets last and actual state
        bool last_stateval = it_lastval_bool->second;
        bool actual_stateval;
        it->second.GetActualValue (actual_stateval);

        // Calculate toggle discrete value
        if ( actual_stateval )
        {
            // Debug purpose
            std::cout << "Var: " << it->first << " era " << last_stateval << " passou a ";
            if ( last_stateval )
            {
                it->second.SetValue (false);
                last_stateval = false;
                std::cout << last_stateval << std::endl;
            }
            else
            {
                it->second.SetValue (true);
                last_stateval = true;
                std::cout << last_stateval << std::endl;
            }
        }
        else
        {
            std::cout << "Var: " << it->first << " sem alteracao de estado" << std::endl;
        }

        // Save actual state for the next execution
        it_lastval_bool->second = last_stateval;

        // If we're dealing with the output var, move it to its map
        if ( var_info->second.is_output == true )
        {
            std::cout << "SAIDA, ACTUAL: " << actual_stateval << " LAST: " << last_stateval << std::endl;
            // Remove the old entry and insert the new output value to its map
            m_variableslist_outputlastvalue_toggled.clear();
            m_variableslist_outputlastvalue_toggled.emplace (it->first, last_stateval);

        // Remove it from the input variables map
//            variablesvalueslist.erase (it);
        }
        // Calculate Score
        //CalcScore (variablesvalueslist);
    }

    // Output Value Management
    if ( m_variableslist_outputlastvalue_toggled.empty() )
    {
        std::cout << "No output FOUND!" << std::endl;
    }

    // Calculate Score
    CalcScore (variablesvalueslist, m_variableslist_score_toggled, m_variableslist_normscore_toggled, m_omega_weight_toggled, m_max_value_score_toggled, m_variableslist_outputlastvalue_toggled);
}

//double doubleval;
////          item.GetActualValue (doubleval);
//OCDTConvert::toUString
//OCDTConvert::fromUString<double>

void
EventTrackerCalc::CalcScore (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist,
                             std::map <Glib::ustring, int>& score_list,
                             std::map <Glib::ustring, double>& normscore_list,
                             double& omega_weight,
                             int& max_score,
                             std::map <Glib::ustring, bool> outputlastvalue)
{
    // If score map is empty (first execution), initialize it
    if ( score_list.empty() == true)
    {
        InitializeMaps (variablesvalueslist);

        // And exits because there's no output value to compare (first execution)
        return;
    }

    std::cout << "\n\n";
    // Iterate through variables score list and compute each one
    for (auto& it_score : score_list)
    {
        // Search it on the variable values map
        auto it_var = variablesvalueslist.find (it_score.first);

        // Get its value
        bool input_value;
        it_var->second.GetActualValue (input_value);

        // Get the output value
        bool output_val;
        output_val = outputlastvalue.begin()->second;

        // Apply xNOR logic
        if ( input_value == output_val )
        {
            it_score.second++;
        }
        else
        {
            it_score.second--;
        }

        std::cout << "var: " << it_score.first << " Score: " << it_score.second << std::endl;
    }
    std::cout << "\n\n";

    // Moving Average Calculation
    double l_forgetting_factor = 0.8;

    // Check for output values (MAXIMUM)
    auto it_outputscore = score_list.find (outputlastvalue.begin()->first);
    auto it_outputnscore = normscore_list.find (outputlastvalue.begin()->first);

    CalcAverageScore (l_forgetting_factor, score_list, normscore_list, omega_weight, max_score, it_outputscore->second, it_outputnscore->second);
}

void
EventTrackerCalc::CalcAverageScore (const double lambda,
                                    std::map <Glib::ustring, int>& score_list,
                                    std::map <Glib::ustring, double>& normscore_list,
                                    double& omega_weight,
                                    int& max_score,
                                    const double outputval,
                                    const double outputvaln)
{


    // Check the size of variables score list, if null returns
    if ( score_list.empty() )
    {
        std::cout << "Variables score list is empty! Error!" << std::endl;
        return;
    }

    // Fills list with the average score entries (N = 1)
    if ( normscore_list.empty() == true )
    {
        omega_weight = 1;
        std::cout <<  "Omega (weight): " << omega_weight << std::endl;
        for (auto it : score_list)
        {
            normscore_list.emplace (it.first, it.second);
            std::cout << "var: " << it.first << " NScore: " << it.second << std::endl;
        }
    }
    // If not the first execution (N > 1)
    else
    {
        // Compute weighting factor
        omega_weight =  m_sysconfig.mSearchSlot; //( lambda*omega_weight + 1 );
        std::cout <<  "Omega (weight): " << omega_weight << std::endl;

        // Iterates through each variable score
        for (auto it : score_list)
        {
            // Get score
            int var_score_value = it.second;

            // Get moving average of the previous sample
            auto it_prev = normscore_list.find (it.first);
            double previous_sample = it_prev->second;

            // Compute max score in order to normalize the variable moving average value
            double max_nscore = ( 1 - 1/omega_weight ) * outputvaln + ( 1/omega_weight ) * outputval;

            // Compute NORMALIZED MOVING AVERAGE for the current sample
            double current_sample = (( 1 - 1/omega_weight ) * previous_sample + ( 1/omega_weight ) * var_score_value) / max_nscore;

            // Store moving average into its map
            normscore_list[it.first] = current_sample;

            std::cout << it.first << " NScore: " << current_sample << std::endl;
            std::cout << "Max_nscore: " << max_nscore << std::endl;
        }
    }

    //std::cout << "\n====================================" << std::endl;
}

Variables&
EventTrackerCalc::GetVariablesSensitivity ()
{

}

void
EventTrackerCalc::SetVariablesSensitivity (const Variables&)
{

}

void
EventTrackerCalc::UpdateVariablesSensitivity (OCItemVariable& item)
{

}

void
EventTrackerCalc::InitializeMaps (std::map<Glib::ustring, OCItemVariable>& variablesvalueslist)
{
    // Fill score's map with all the variables name
    for (auto it : variablesvalueslist)
    {
        m_variableslist_score.emplace(it.first, 0);
        m_variableslist_score_toggled.emplace(it.first, 0);
    }
}

std::map<Glib::ustring, int>
EventTrackerCalc::GetScore (bool normal_toggle)
{
    if (normal_toggle)
    {
        return m_variableslist_score_toggled;
    }

    return m_variableslist_score;
}

std::map<Glib::ustring, double>
EventTrackerCalc::GetNormScore (bool normal_toggle)
{
    if ( normal_toggle )
    {
        return m_variableslist_normscore_toggled;
    }

    return m_variableslist_normscore;
}

void
EventTrackerCalc::SetSystemConfig (EventTrackerConfig sysconfig)
{
    m_sysconfig = sysconfig;
}
//bool EventTrackerCalc::fileExists(std::string& fileName) {
//    return static_cast<bool>( std::ifstream(fileName) );
//}
//
//bool
//EventTrackerCalc::WriteDataCSV (Glib::ustring file_name, const double value,
//                               const double output_val, const double Score, const double Av_Score)
//{
//    std::fstream file;
//    file.open ( file_name, std::ios::out | std::ios::app );
//    if ( file == false )
//    {
//        std::cerr << "There isn't any file to open." << std::endl;
//    }
//
//
//    for (auto it : variablesvalueslist)
//    {
//
//    }
//
//
//    try
//    {
//        std::fstream file;
//        file.open ( file_name, std::ios::out | std::ios::app );
//        if (file)
//        {
//            file << "\"" << column1 << "\",";
//            file << "\"" << column2 << "\",";
//            file << "\"" << column3 << "\"";
//            file <<  std::endl;
//        }
//    catch ()
//    {
//        std::cerr << "Error opening CSV file! exiting..." << std::endl;
//        return false;
//    }
//    return true;
//}
