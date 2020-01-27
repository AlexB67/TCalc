#include "telescopecombomodel.hpp"
#include <iostream>
#include <glibmm/i18n.h>

using namespace ScopeCombo;

void ScopeCombomodel::create_scope_model()
{
    m_scopetreemodel = Gtk::TreeStore::create(m_scopecols);
    m_scopelistmodel = Gtk::ListStore::create(m_scopecompletioncols);
}

void ScopeCombomodel::swap_scope_rows(const Glib::ustring& scopename) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator previousiter;
    Gtk::TreeModel::iterator parentiter;

    
    for(parentiter = m_scopetreemodel->children().begin(); parentiter != m_scopetreemodel->children().end(); ++parentiter)
        if ((*parentiter)[m_scopecols.m_sbrand] == _("User")) break;

    for (iter = parentiter->children().begin(); iter != parentiter->children().end(); ++iter)
    {
        if (scopename == iter->get_value(m_scopecols.m_smodel))
        {
            previousiter = iter;
            iter--;
            break;
        }
    }

    const Gtk::TreeRow prow = *previousiter;
    const Gtk::TreeRow row = *iter;

    const std::tuple<double, double, double, double, int, Glib::ustring, Glib::ustring, Glib::ustring, 
                    Glib::ustring, double, double> scopedata = 
    {
        prow[m_scopecols.m_saperture], prow[m_scopecols.m_sflen],  prow[m_scopecols.m_sobstruct], 
        prow[m_scopecols.m_sreflect],  prow[m_scopecols.m_stype],  prow[m_scopecols.m_smirrorcoating],
        prow[m_scopecols.m_smirrormaterial], prow[m_scopecols.m_slenscoating], prow[m_scopecols.m_slensmaterial],
        prow[m_scopecols.m_sstrehl], prow[m_scopecols.m_sweight] 
    }; // note, we omit the model name and brand

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
    }
}


void ScopeCombomodel::append_scope_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                            Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                            double, double>& scopedata, bool ischild)
{
    if (true == ischild)
    {
        const auto childrow = *(m_scopetreemodel->append(parent_row.children()));
       // brand is omitted for children row, 
        childrow[m_scopecols.m_smodel] = std::get<1>(scopedata);;
        childrow[m_scopecols.m_saperture] = std::get<2>(scopedata);
        childrow[m_scopecols.m_sflen] = std::get<3>(scopedata);
        childrow[m_scopecols.m_sobstruct] = std::get<4>(scopedata);
        childrow[m_scopecols.m_sreflect] = std::get<5>(scopedata);
        childrow[m_scopecols.m_stype] = std::get<6>(scopedata);
        childrow[m_scopecols.m_smirrorcoating] = std::get<7>(scopedata);
        childrow[m_scopecols.m_smirrormaterial] = std::get<8>(scopedata);
        childrow[m_scopecols.m_slenscoating] = std::get<9>(scopedata);
        childrow[m_scopecols.m_slensmaterial] = std::get<10>(scopedata);
        childrow[m_scopecols.m_sstrehl] = std::get<11>(scopedata);
        childrow[m_scopecols.m_sweight] = std::get<12>(scopedata);

        // used by search completion
        const auto listrow = *(m_scopelistmodel->append());
        listrow[m_scopecompletioncols.m_sbrand] = std::get<0>(scopedata);
        listrow[m_scopecompletioncols.m_smodel] = std::get<1>(scopedata);
    }
    else
    {
        parent_row = *(m_scopetreemodel->append());
        // Parent holds the brand name and will be appended by child rows above.
        parent_row[m_scopecols.m_sbrand] = std::get<0>(scopedata);
        parent_row[m_scopecols.m_smodel] = ""; // stops text != NULL warnings
    }
}


void ScopeCombomodel::add_scope_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                        Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                        double, double>& scopedata) const // Used by equipment editor
{
    Gtk::TreeModel::iterator iter;
    bool found = false;
    
    if(m_scopetreemodel->children().size() > 0)
    {
        for (iter = m_scopetreemodel->children().begin(); iter !=m_scopetreemodel->children().end(); ++iter)
        {
            if (iter->get_value(m_scopecols.m_sbrand) == _("User")) 
            {
                found = true;
                break;
            }
        }
    }
    
    if (false == found) // No user eyeieces were aded yet so add the "User" category
    {
        const Gtk::TreeRow row = *(m_scopetreemodel->prepend());
        row[m_scopecols.m_sbrand] = _("User");
        row[m_scopecols.m_smodel] = ""; // stops text != NULL warnings
    }

    if (false == found) iter = m_scopetreemodel->children().begin();
    
    if (iter)
    {
        const Gtk::TreeRow row = *(m_scopetreemodel->append(iter->children()));
        row[m_scopecols.m_smodel] = std::get<1>(scopedata);
        row[m_scopecols.m_saperture] = std::get<2>(scopedata);
        row[m_scopecols.m_sflen] = std::get<3>(scopedata);
        row[m_scopecols.m_sobstruct] = std::get<4>(scopedata);
        row[m_scopecols.m_sreflect] = std::get<5>(scopedata);
        row[m_scopecols.m_stype] = std::get<6>(scopedata);
        row[m_scopecols.m_smirrorcoating] = std::get<7>(scopedata);
        row[m_scopecols.m_smirrormaterial] = std::get<8>(scopedata);
        row[m_scopecols.m_slenscoating] = std::get<9>(scopedata);
        row[m_scopecols.m_slensmaterial] = std::get<10>(scopedata);
        row[m_scopecols.m_sstrehl] = std::get<11>(scopedata);
        row[m_scopecols.m_sweight] = std::get<12>(scopedata);
    }

     // Update completion list
    const auto listrow = *(m_scopelistmodel->append());
    listrow[m_scopecompletioncols.m_sbrand] = _("User");
    listrow[m_scopecompletioncols.m_smodel] = std::get<1>(scopedata);
    
}

void ScopeCombomodel::remove_scope_from_model(const Glib::ustring &scopename) const
{
     Gtk::TreeModel::iterator iter;

    if (0 == m_scopetreemodel->children().size()) return;

    for (iter = m_scopetreemodel->children().begin(); iter != m_scopetreemodel->children().end(); ++iter)
        if (iter->get_value(m_scopecols.m_sbrand) == _("User")) break;

    for (auto iter2 : iter->children())
    {
        if (scopename == (*iter2)->get_value(m_scopecols.m_smodel))
        {
            if(iter2) m_scopetreemodel->erase(iter2);
            break;
        }
    }

    // If there are no more user eyepieces left delete the User category (parent)
    if (0 == iter->children().size()) m_scopetreemodel->erase(iter);

    // remove from completion list
    for (auto &i : m_scopelistmodel->children())
    {
        if (scopename == i->get_value(m_scopecompletioncols.m_smodel))
        {
            m_scopelistmodel->erase(i);
            break;
        }
    }
}

void ScopeCombomodel::update_scope_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                        Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                        double, double>& scopedata, const Glib::ustring& oldname) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator iter2;

    for (iter = m_scopetreemodel->children().begin(); iter !=m_scopetreemodel->children().end(); ++iter)
        if (iter->get_value(m_scopecols.m_sbrand) == _("User")) break;

    for (iter2 = iter->children().begin(); iter2 != iter->children().end(); ++iter2)
        if (oldname == iter2->get_value(m_scopecols.m_smodel))
            break;

    if (iter2)
    {
        const auto row = *iter2;
        if (row)
        {
            row[m_scopecols.m_smodel] = std::get<1>(scopedata);
            row[m_scopecols.m_saperture] = std::get<2>(scopedata);
            row[m_scopecols.m_sflen] = std::get<3>(scopedata);
            row[m_scopecols.m_sobstruct] = std::get<4>(scopedata);
            row[m_scopecols.m_sreflect] = std::get<5>(scopedata);
            row[m_scopecols.m_stype] = std::get<6>(scopedata);
            row[m_scopecols.m_smirrorcoating] = std::get<7>(scopedata);
            row[m_scopecols.m_smirrormaterial] = std::get<8>(scopedata);
            row[m_scopecols.m_slenscoating] = std::get<9>(scopedata);
            row[m_scopecols.m_slensmaterial] = std::get<10>(scopedata);
            row[m_scopecols.m_sstrehl] = std::get<11>(scopedata);
            row[m_scopecols.m_sweight] = std::get<12>(scopedata);
        }
    }

    // Update completion model
    for (auto &i : m_scopelistmodel->children())
    {
        if (oldname == (*i)->get_value(m_scopecompletioncols.m_smodel))
        {
            const auto row = *i;
            row[m_scopecompletioncols.m_smodel] =  std::get<1>(scopedata);
            break;
        }
    }
}

void ScopeCombomodel::setup_scope_combo_model(Gtk::ComboBox& scopecombo)
{
    m_scopecombo = &scopecombo;
    m_scopecombo->set_model(m_scopetreemodel);
    m_scopecombo->pack_start(m_scopecols.m_sbrand);
    m_scopecombo->set_entry_text_column(1);
    auto *entry = static_cast<Gtk::Entry *>(m_scopecombo->get_entry());
    entry->property_width_chars() = 32;
    entry->property_editable() = false;
    entry->set_placeholder_text("Select model");
    m_scopecombo->set_active(0);

     // set the first telescope we find;
    if (m_scopetreemodel->children().size() > 0)
    {

        Gtk::TreeIter it = m_scopetreemodel->children().begin();
        if(it->children().size() > 0)
        { 
            auto it2 = it->children().begin();
            m_scopecombo->set_active(it2);
            entry->set_text((*it2)[m_scopecols.m_smodel]);
        }
    }
}

void ScopeCombomodel::set_scope_completion_model(Gtk::SearchEntry& scopesearch)
{
    m_scopesearch = &scopesearch;
    scopeentrycompletion = Gtk::EntryCompletion::create();
    scopeentrycompletion->set_model(m_scopelistmodel);
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
        m_scopesearch->set_text(row[m_scopecompletioncols.m_smodel]);

        Gtk::TreeModel::iterator it;

        for(it = m_scopetreemodel->children().begin(); it != m_scopetreemodel->children().end(); ++it)
        {
             if ((*it)[m_scopecols.m_sbrand] == row[m_scopecompletioncols.m_sbrand]) break;
        }
           
        for(Gtk::TreeModel::iterator it2 = it->children().begin(); it2 != it->children().end(); ++it2)
        {
            if (row[m_scopecompletioncols.m_smodel] == it2->get_value(m_scopecols.m_smodel)) 
            {
                m_scopecombo->set_active(*it2);
                break;
            }
        }

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
