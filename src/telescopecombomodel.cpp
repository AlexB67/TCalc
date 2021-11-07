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

    bool found = false;
    
    for(parentiter = m_scopetreemodel->children().begin(); parentiter != m_scopetreemodel->children().end(); ++parentiter)
        if ((*parentiter)[m_scopecols.m_sbrand] == _("User")) break;

    for (iter = parentiter->children().begin(); iter != parentiter->children().end(); ++iter)
    {
        if (scopename == iter->get_value(m_scopecols.m_smodel))
        {
            previousiter = iter;
            iter--;
            found = true;
        }

        if(found) break;
    }

    Gtk::TreeRow prow = *previousiter;
    Gtk::TreeRow row = *iter;

    std::tuple<double, double, double, double, int, Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
            Glib::ustring, Glib::ustring, Glib::ustring> scopedata =  
    {
        prow[m_scopecols.m_saperture], prow[m_scopecols.m_sflen],  prow[m_scopecols.m_sobstruct], 
        prow[m_scopecols.m_sreflect],  prow[m_scopecols.m_stype],  prow[m_scopecols.m_smirrorcoating],
        prow[m_scopecols.m_smirrormaterial], prow[m_scopecols.m_slenscoating], prow[m_scopecols.m_slensmaterial],
        prow[m_scopecols.m_sstrehl], prow[m_scopecols.m_sweight], prow[m_scopecols.m_stube_weight] , prow[m_scopecols.m_smount_type],
        prow[m_scopecols.m_sfocuser_type], prow[m_scopecols.m_sfinder_type]
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
        prow[m_scopecols.m_stube_weight] = static_cast<double>(row[m_scopecols.m_sweight]);
        prow[m_scopecols.m_smount_type] = static_cast<Glib::ustring>(row[m_scopecols.m_smount_type]);
        prow[m_scopecols.m_sfocuser_type] = static_cast<Glib::ustring>(row[m_scopecols.m_sfocuser_type]);
        prow[m_scopecols.m_sfinder_type] = static_cast<Glib::ustring>(row[m_scopecols.m_sfinder_type]);

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
        row[m_scopecols.m_stube_weight] = std::get<11>(scopedata);
        row[m_scopecols.m_smount_type] = std::get<12>(scopedata);
        row[m_scopecols.m_sfocuser_type] = std::get<13>(scopedata);
        row[m_scopecols.m_sfinder_type] = std::get<14>(scopedata); 
    }   
}


void ScopeCombomodel::append_scope_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata, bool ischild)
{
    if (true == ischild)
    {
        auto childrow = *(m_scopetreemodel->append(parent_row.children()));
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
        childrow[m_scopecols.m_stube_weight] = std::get<13>(scopedata);
        childrow[m_scopecols.m_smount_type] = std::get<14>(scopedata);
        childrow[m_scopecols.m_sfocuser_type] = std::get<15>(scopedata);
        childrow[m_scopecols.m_sfinder_type] = std::get<16>(scopedata); 

        // used by search completion
        auto listrow = *(m_scopelistmodel->append());
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
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata) const // Used by equipment editor
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
            }

            if(found) break;
        }
    }
    
    if (false == found) // No user eyeieces were aded yet so add the "User" category
    {
        Gtk::TreeRow row = *(m_scopetreemodel->prepend());
        row[m_scopecols.m_sbrand] = _("User");
        row[m_scopecols.m_smodel] = ""; // stops text != NULL warnings
    }

    if (false == found) iter = m_scopetreemodel->children().begin();
    
    if (iter)
    {
        Gtk::TreeRow row = *(m_scopetreemodel->append(iter->children()));
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
        row[m_scopecols.m_stube_weight] = std::get<13>(scopedata);
        row[m_scopecols.m_smount_type] = std::get<14>(scopedata);
        row[m_scopecols.m_sfocuser_type] = std::get<15>(scopedata);
        row[m_scopecols.m_sfinder_type] = std::get<16>(scopedata); 
    }

     // Update completion list
    auto listrow = *(m_scopelistmodel->append());
    listrow[m_scopecompletioncols.m_sbrand] = _("User");
    listrow[m_scopecompletioncols.m_smodel] = std::get<1>(scopedata);
    
}

void ScopeCombomodel::remove_scope_from_model(const Glib::ustring &scopename) const
{
     Gtk::TreeModel::iterator iter;

    if (0 == m_scopetreemodel->children().size()) return;

    for (iter = m_scopetreemodel->children().begin(); iter != m_scopetreemodel->children().end(); ++iter)
        if (iter->get_value(m_scopecols.m_sbrand) == _("User")) break;

    bool deleted = false;
    for (auto iter2 : iter->children())
    {
        if (scopename == iter2.get_value(m_scopecols.m_smodel))
        {
            if(iter2) m_scopetreemodel->erase(iter2.get_iter());
            deleted = true;
        }

        if(deleted) break;
    }

    // If there are no more user eyepieces left delete the User category (parent)
    if (0 == iter->children().size()) m_scopetreemodel->erase(iter);

    // remove from completion list
    deleted = false;
    for (auto &i : m_scopelistmodel->children())
    {
        if (scopename == i.get_value(m_scopecompletioncols.m_smodel))
        {
            m_scopelistmodel->erase(i.get_iter());
            deleted = true;
        }

        if(deleted) break;
    }
}

void ScopeCombomodel::update_scope_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata, const Glib::ustring& oldname) const
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
        auto row = *iter2;
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
            row[m_scopecols.m_stube_weight] = std::get<13>(scopedata);
            row[m_scopecols.m_smount_type] = std::get<14>(scopedata);
            row[m_scopecols.m_sfocuser_type] = std::get<15>(scopedata);
            row[m_scopecols.m_sfinder_type] = std::get<16>(scopedata); 
        }
    }

    // Update completion model
    bool updated = false;

    for (auto &i : m_scopelistmodel->children())
    {
        if (oldname == i.get_value(m_scopecompletioncols.m_smodel))
        {
            auto row = i;
            row[m_scopecompletioncols.m_smodel] =  std::get<1>(scopedata);
            updated = true;
        }

        if(updated) break;
    }
}


