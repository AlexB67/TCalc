#include "telescopecombomodel.hpp"
#include <iostream>

using namespace ScopeCombo;

void ScopeCombomodel::create_scope_model_combo(Gtk::ComboBox& scopecombo)
{
    m_scopecombo = &scopecombo;
    m_scopetreemodel = Gtk::ListStore::create(m_scopecols);
    m_scopecombo->set_model(m_scopetreemodel);
    m_scopecombo->set_row_separator_func(sigc::mem_fun(*this, &ScopeCombomodel::on_separator));
}

bool ScopeCombomodel::on_separator(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::iterator& iter)
{
    if (iter)
    {
        const auto row = *iter;
        if (100 == row[m_scopecols.m_stype]) return true; // 100 represents a separator
    }

    return false;
}

void ScopeCombomodel::swap_scope_rows(const Glib::ustring& scopename, bool movedown) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator previousiter;

    for (iter = m_scopetreemodel->children().begin(); iter != m_scopetreemodel->children().end(); ++iter)
    {
        if (scopename == iter->get_value(m_scopecols.m_smodel))
        {
            previousiter = iter;
            break;
        }
    }

    if (true == movedown) 
        iter++; 
    else 
        iter--;

    const Gtk::TreeRow prow = *previousiter;
    const Gtk::TreeRow row = *iter;

    const std::tuple<double, double, double, double, int, Glib::ustring, Glib::ustring, Glib::ustring, 
                    Glib::ustring, double, double> scopedata = 
    {
        prow[m_scopecols.m_saperture], prow[m_scopecols.m_sflen],  prow[m_scopecols.m_sobstruct], 
        prow[m_scopecols.m_sreflect],  prow[m_scopecols.m_stype],  prow[m_scopecols.m_smirrorcoating],
        prow[m_scopecols.m_smirrormaterial], prow[m_scopecols.m_slenscoating], prow[m_scopecols.m_slensmaterial],
        prow[m_scopecols.m_sstrehl], prow[m_scopecols.m_sweight] 
    }; // note, we omit the model name 

    if (iter && previousiter) // swap values of the two rows
    {
        prow[m_scopecols.m_smodel] = static_cast<Glib::ustring>(row[m_scopecols.m_smodel]);
        prow[m_scopecols.m_saperture] =  static_cast<double>(row[m_scopecols.m_saperture]);
        prow[m_scopecols.m_sflen] = static_cast<double>(row[m_scopecols.m_sflen]);
        prow[m_scopecols.m_sobstruct] = static_cast<double>(row[m_scopecols.m_sobstruct]);
        prow[m_scopecols.m_sreflect] = static_cast<double>(row[m_scopecols.m_sreflect]);
        prow[m_scopecols.m_stype] = static_cast<int>(row[m_scopecols.m_stype]);
        prow[m_scopecols.m_smirrorcoating] = static_cast<Glib::ustring>(row[m_scopecols.m_smirrorcoating]);
        prow[m_scopecols.m_smirrormaterial] = static_cast<Glib::ustring>(row[m_scopecols.m_smirrormaterial]);
        prow[m_scopecols.m_slenscoating] = static_cast<Glib::ustring>(row[m_scopecols.m_slenscoating]);
        prow[m_scopecols.m_slensmaterial] = static_cast<Glib::ustring>(row[m_scopecols.m_slensmaterial]);
        prow[m_scopecols.m_sstrehl] = static_cast<double>(row[m_scopecols.m_sstrehl]);
        prow[m_scopecols.m_sweight] = static_cast<double>(row[m_scopecols.m_sweight]);

        row[m_scopecols.m_smodel] = scopename;
        row[m_scopecols.m_saperture] = std::get<0>(scopedata);
        row[m_scopecols.m_sflen] = std::get<1>(scopedata);
        row[m_scopecols.m_sobstruct] = std::get<2>(scopedata);
        row[m_scopecols.m_sreflect] = std::get<3>(scopedata);
        row[m_scopecols.m_stype] = std::get<4>(scopedata);
        row[m_scopecols.m_smirrorcoating] = std::get<5>(scopedata);
        row[m_scopecols.m_smirrormaterial] = std::get<6>(scopedata);
        row[m_scopecols.m_slenscoating] = std::get<7>(scopedata);
        row[m_scopecols.m_slensmaterial] = std::get<8>(scopedata);
        row[m_scopecols.m_sstrehl] = std::get<9>(scopedata);
        row[m_scopecols.m_sweight] = std::get<10>(scopedata);

        m_scopecombo->set_active(previousiter);
    }
}


void ScopeCombomodel::append_scope_to_model(const std::tuple<Glib::ustring, double, double, double, double, int, 
                                            Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                            double, double>& scopedata) const
{
    const auto row = *(m_scopetreemodel->append());
    tuple_to_model(row, scopedata);
}

void ScopeCombomodel::add_scope_to_model(const std::tuple<Glib::ustring, double, double, double, double, int, 
                                        Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                        double, double>& scopedata, bool append) const
{

    Gtk::TreeModel::iterator iter;

    for (iter = m_scopetreemodel->children().begin(); iter != m_scopetreemodel->children().end(); ++iter)
        if (100 == (*iter)[m_scopecols.m_stype]) break;

    if (iter)
    {
        Gtk::TreeRow row;
        (true == append) ? row = *(m_scopetreemodel->append()) : row = *(m_scopetreemodel->insert(*iter));
        tuple_to_model(row, scopedata);
    }
}

void ScopeCombomodel::remove_scope_from_model(const Glib::ustring &epname) const
{
    // TODO create a std::map of index to name in the combo model to avoid searching like this, this is temporary.
    for (auto &i : m_scopetreemodel->children())
    {
        if (epname == i->get_value(m_scopecols.m_smodel))
        {
            m_scopetreemodel->erase(i);
            break;
        }
    }
}

void ScopeCombomodel::update_scope_model(const std::tuple<Glib::ustring, double, double, double, double, int, 
                                        Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                        double, double>& scopedata) const
{

    // TODO create a std::map of index to name in the combo model to avoid searching like this, this is temporary.

    Gtk::TreeModel::iterator iter;

    for (iter = m_scopetreemodel->children().begin(); iter != m_scopetreemodel->children().end(); ++iter)
        if (std::get<0>(scopedata) == iter->get_value(m_scopecols.m_smodel))
            break;

    if (iter)
    {
        auto row = *(m_scopetreemodel->append());
        tuple_to_model(row, scopedata);
    }
}

void ScopeCombomodel::tuple_to_model(const Gtk::TreeRow& row, const std::tuple<Glib::ustring, double, double, 
                          double, double, int, Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                          double, double>& scopedata) const
{
    row[m_scopecols.m_smodel] = std::get<0>(scopedata);
    row[m_scopecols.m_saperture] = std::get<1>(scopedata);
    row[m_scopecols.m_sflen] = std::get<2>(scopedata);
    row[m_scopecols.m_sobstruct] = std::get<3>(scopedata);
    row[m_scopecols.m_sreflect] = std::get<4>(scopedata);
    row[m_scopecols.m_stype] = std::get<5>(scopedata);
    row[m_scopecols.m_smirrorcoating] = std::get<6>(scopedata);
    row[m_scopecols.m_smirrormaterial] = std::get<7>(scopedata);
    row[m_scopecols.m_slenscoating] = std::get<8>(scopedata);
    row[m_scopecols.m_slensmaterial] = std::get<9>(scopedata);
    row[m_scopecols.m_sstrehl] = std::get<10>(scopedata);
    row[m_scopecols.m_sweight] = std::get<11>(scopedata);
}

void ScopeCombomodel::setup_scope_combo()
{
    m_scopecombo->pack_start(m_scopecols.m_smodel);
    m_scopecombo->set_active(0);
}

void ScopeCombomodel::set_scope_completion_model(Gtk::SearchEntry& scopesearch)
{
    m_scopesearch = &scopesearch;
    scopeentrycompletion = Gtk::EntryCompletion::create();
    scopeentrycompletion->set_model(m_scopetreemodel);
    scopeentrycompletion->set_text_column(m_scopecols.m_smodel);
    scopeentrycompletion->set_minimum_key_length(2);
    scopeentrycompletion->set_popup_completion(true);
    m_scopesearch->set_completion(scopeentrycompletion);
    scopeentrycompletion->signal_match_selected().connect(sigc::mem_fun(*this, &ScopeCombomodel::on_scope_selected), false);
    scopeentrycompletion->set_match_func( sigc::mem_fun(*this, &ScopeCombomodel::on_scope_completion_match));
}

bool ScopeCombomodel::on_scope_selected(const Gtk::TreeModel::iterator& iter)
{

    if (iter)
    {
        const auto row = *iter;
        m_scopecombo->set_active(row);
        m_scopesearch->set_text(row[m_scopecols.m_smodel]);
        return true;
    }

    return false;
}

void ScopeCombomodel::set_case_sensitive(const bool case_sensitive)
{
    // to make auto completion search case sensitive
    m_case_sensitive = case_sensitive;
}

bool ScopeCombomodel::on_scope_completion_match(const Glib::ustring& key, const Gtk::TreeModel::iterator& iter)
{
    if (iter)
    {
        size_t found;
        const auto row = *iter;

        if (true == m_case_sensitive)
            found = static_cast<Glib::ustring>(row[m_scopecols.m_smodel]).find(m_scopesearch->get_text());
        else
            found = static_cast<Glib::ustring>(row[m_scopecols.m_smodel]).lowercase().find(key);
     
        if (found != Glib::ustring::npos)
            return true;
    }

    return false;
}
