#include "telescopebox.hpp"
#include <iostream>
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"

using AppGlobals::log_msg;


void ScopeBox::Telescopebox::create_scopemodel_connection()
{
    AppGlobals::update_scope_data.connect([this](const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata, const Glib::ustring& oldname)
    {
        m_scombomodel.update_scope_model(scopedata, oldname);

        if (!m_with_entry) scope_changed();
        else reset_smodel_entry();
    });

    AppGlobals::new_scope_data.connect([this](const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata)
    {
       m_scombomodel.add_scope_to_model(scopedata);
    });

    AppGlobals::del_scope_data.connect([this](const Glib::ustring& scopemodelname)
    {

        bool updatecombo = false;

        if (m_with_entry)
        {
            if (m_smodelentry.get_text() == scopemodelname) 
                updatecombo = true;
        }
        else
        {
            if (m_smodel->get_active()->get_value(m_scombomodel.m_scopecols.m_smodel) == scopemodelname) 
                updatecombo = true;
        }

        m_scombomodel.remove_scope_from_model(scopemodelname);

        // set the first telescope we find after deletion.
        if (m_scombomodel.get_scopemodel()->children().size() > 0 && true == updatecombo)
        {

            Gtk::TreeIter it = (m_scombomodel.get_scopemodel()->children().begin());
            if(it->children().size() > 0)
            { 
                auto it2 = it->children().begin();
                if (m_with_entry)
                    m_smodelentry.set_text((*it2)[m_scombomodel.m_scopecols.m_smodel]);
                else
                    m_smodel->set_active(it2);
                
                current_row = *it2;
                current_iter = it2;
            }
        }
    });

      AppGlobals::move_scope_row_up.connect([this](const Glib::ustring& scopemodelname){
      m_scombomodel.swap_scope_rows(scopemodelname);
    });
}

void ScopeBox::Telescopebox::set_values_from_model(const Gtk::TreeRow& row)
{
    using GlibUtils::dtostr;
    using namespace Astrocalc;
    bool flag = AppGlobals::LOGFLAG;

    log_msg.emit(flag, LogView::tINFO, row[m_scombomodel.m_scopecols.m_smodel] + _(" selected."));
    m_saperture.set_value(row[m_scombomodel.m_scopecols.m_saperture]);
    m_sflen.set_value(row[m_scombomodel.m_scopecols.m_sflen]);
    m_stype.set_active(row[m_scombomodel.m_scopecols.m_stype]);

    if (astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Reflectivity/%"));
    else if (astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Trans.(Refl.)/%"));
    else if (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Transmission/%"));

    if (row[m_scombomodel.m_scopecols.m_sobstruct] < astrocalc::tSMALL &&
        astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
    {
        log_msg.emit(flag, LogView::tINFO, _("Reflector obstruction size unknown, using default value: ") 
                   + dtostr<double>(astrocalc::OBSTRUCTSIZE, 2) + _("%"));
        m_sobstruct.set_value(astrocalc::OBSTRUCTSIZE);
    }
    else if (row[m_scombomodel.m_scopecols.m_sobstruct] < astrocalc::tSMALL &&
             astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
    {
        log_msg.emit(flag, LogView::tINFO, _("SCT/Mak obstruction size unknown, using default value: ") 
                   + dtostr<double>(astrocalc::OBSTRUCTSIZESCT, 2) + _("%"));
        m_sobstruct.set_value(astrocalc::OBSTRUCTSIZESCT);
    }
    else if (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
    {
        m_sobstruct.set_value(0.0); // we'll ignore any value in the data file since this is always true.
    }
    else
    {
        m_sobstruct.set_value(row[m_scombomodel.m_scopecols.m_sobstruct]);
    }

    (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype]) 
    ? m_sobstruct.set_sensitive(false) : m_sobstruct.set_sensitive(true);

    if (row[m_scombomodel.m_scopecols.m_sreflect] < astrocalc::tSMALL)
    {
        if (astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
        {
            log_msg.emit(flag, LogView::tINFO, _("Reflector Reflectivity data unknown, using default value: ") 
                       + dtostr<double>(astrocalc::SCOPEREFLECT, 2) + _("%"));
            m_sreflect.set_value(astrocalc::SCOPEREFLECT);
        }
        else if (astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
        {
            log_msg.emit(flag, LogView::tINFO, _("SCT/Mak reflectivity data unknown, using default value: ") 
                       + dtostr<double>(astrocalc::SCOPEREFLECT, 2) + _("%"));
            log_msg.emit(flag, LogView::tINFO, _("SCT/Mak Transmission data unknown, using default value: ") 
                       + dtostr<double>(astrocalc::SCOPETRANS, 2) + _("%"));
            log_msg.emit(flag, LogView::tINFO, _("SCT/Mak estimated Reflectivity/Transmission value: ") 
                       + dtostr<double>(astrocalc::SCOPEREFLECT * astrocalc::SCOPETRANS / 100.0, 2) + _("%"));
            m_sreflect.set_value(astrocalc::SCOPEREFLECT * astrocalc::SCOPETRANS / 100.0);
        }
        else if (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
        {
            log_msg.emit(flag, LogView::tINFO, _("Refractor transmission data unknown, using default value: ") 
                       + dtostr<double>(astrocalc::SCOPETRANS, 2) + _("%"));
            m_sreflect.set_value(astrocalc::SCOPETRANS);
        }
    }
    else
    {
        m_sreflect.set_value(row[m_scombomodel.m_scopecols.m_sreflect]);
    }

    if (true == m_userdataonly)
        m_sobstruct.set_sensitive(false); // for edit mode
}

void ScopeBox::Telescopebox::update_sfratio()
{   
    (m_saperture.get_value() > Astrocalc::astrocalc::tSMALL) ?
    m_sfratio.set_value(m_sflen.get_value() / m_saperture.get_value()) : m_sfratio.set_value(0.0);
}

void ScopeBox::Telescopebox::set_sflen()
{   
    m_sflen.set_value(m_sfratio.get_value() * m_saperture.get_value());
}

void ScopeBox::Telescopebox::scope_type_changed()
{   
    if (Astrocalc::astrocalc::REFRACTOR == m_stype.get_active_row_number())
    {
      m_sobstruct.set_value(0.0);
      m_sreflect.set_value(Astrocalc::astrocalc::SCOPETRANS);
      m_sobstruct.set_sensitive(false);
      m_sreflectlabel.set_text(_("Transmission/%"));
    }
    else if (Astrocalc::astrocalc::REFLECTOR == m_stype.get_active_row_number())
    {
      m_sobstruct.set_value(Astrocalc::astrocalc::OBSTRUCTSIZE);
      m_sreflect.set_value(Astrocalc::astrocalc::SCOPEREFLECT);
      m_sobstruct.set_sensitive(true);
      m_sreflectlabel.set_text(_("Reflectivity/%"));
    }
    else if (Astrocalc::astrocalc::SCTMAK == m_stype.get_active_row_number())
    {
      m_sobstruct.set_value(Astrocalc::astrocalc::OBSTRUCTSIZESCT);
      m_sreflect.set_value(Astrocalc::astrocalc::SCOPEREFLECT * Astrocalc::astrocalc::SCOPETRANS / 100.0);
      m_sobstruct.set_sensitive(true);
      m_sreflectlabel.set_text(_("Trans.(Refl.)/%"));
    }
}

void ScopeBox::Telescopebox::scope_changed()
{
    const Gtk::TreeModel::iterator iter = m_smodel->get_active();

    if (iter)
    {   
        const Gtk::TreeModel::Row row = *iter;
        if (row) 
        {
            set_values_from_model(row);
        }
    }
}

bool ScopeBox::Telescopebox::on_scope_selected(const Gtk::TreeModel::iterator &iter)
{
    const auto& m_scopetreemodel = m_scombomodel.get_scopemodel();

    if (iter)
    {
        const auto row1 = *iter;
        m_smodelentry.set_text(row1[m_scombomodel.m_scopecompletioncols.m_smodel]);

        Gtk::TreeModel::iterator it;

        for(it = m_scopetreemodel->children().begin(); it != m_scopetreemodel->children().end(); ++it)
        {
             if ((*it).get_value(m_scombomodel.m_scopecols.m_sbrand) == 
             row1.get_value(m_scombomodel.m_scopecompletioncols.m_sbrand)) break;
        }
        
        bool found = false;
        for(Gtk::TreeModel::iterator it2 = it->children().begin(); it2 != it->children().end(); ++it2)
        {
            if (row1.get_value(m_scombomodel.m_scopecompletioncols.m_smodel) == 
              (*it2).get_value(m_scombomodel.m_scopecols.m_smodel)) 
            {
                for (auto& connection : con) connection.block();
                m_smodelentry.set_text(row1[m_scombomodel.m_scopecompletioncols.m_smodel]);
                found = true;
                const auto row = *it2;
                current_iter = it2;
                current_row = row;
                set_values_from_model(row);
                for (auto& connection : con) connection.unblock();
            }

            if (found) break;
        }

        return true;
    }

    return false;
}

void ScopeBox::Telescopebox::on_cell_data_changed(const Gtk::TreeModel::const_iterator& iter)
{
    if (!iter)
    {
        m_cell.property_text() = "Select";
        return;
    }

    auto row = *iter;
    const Glib::ustring txt = row[m_scombomodel.m_scopecols.m_smodel];
    m_cell.property_text() = txt;
}

bool ScopeBox::Telescopebox::on_scope_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter)
{
    bool m_case_sensitive = false;

    if (iter)
    {
        size_t found;
        const auto row = *iter;

        if (true == m_case_sensitive)
            found = row.get_value(m_scombomodel.m_scopecompletioncols.m_smodel).find(m_smodelentry.get_text());
        else
            found = row.get_value(m_scombomodel.m_scopecompletioncols.m_smodel).lowercase().find(key);

        if (found != Glib::ustring::npos)
            return true;
    }

    return false;
}

void ScopeBox::Telescopebox::set_custom_scope()
{
    m_smodelentry.set_text(_("Custom"));
}