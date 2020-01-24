#include "telescopebox.hpp"
#include <iostream>
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"

using AppGlobals::log_msg;


void ScopeBox::Telescopebox::create_scopemodel_connection()
{
    AppGlobals::update_scope_data.connect([this](){
       m_scombomodel.update_scope_model(AppGlobals::scopedata);
       scope_changed();
    });

    AppGlobals::new_scope_data.connect([this](){
       m_scombomodel.add_scope_to_model(AppGlobals::scopedata, false);
    });

    AppGlobals::del_scope_data.connect([this](){
      m_scombomodel.remove_scope_from_model(std::get<1>(AppGlobals::scopedata));
    });

     AppGlobals::move_scope_row_up.connect([this](){
      m_scombomodel.swap_scope_rows(std::get<1>(AppGlobals::scopedata));
    });
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
  using GlibUtils::dtostr;
  using namespace Astrocalc;
  bool flag = AppGlobals::LOGFLAG;
  const Gtk::TreeModel::iterator iter = m_smodel->get_active();
  
  if(iter)
  {
    const Gtk::TreeModel::Row row = *iter;
    if(row)
    {  
      log_msg.emit(flag, LogView::tINFO, row[m_scombomodel.m_scopecols.m_smodel] + _(" selected."));
      
      m_saperture.set_value(row[m_scombomodel.m_scopecols.m_saperture]); 
      m_sflen.set_value(row[m_scombomodel.m_scopecols.m_sflen]);
      m_stype.set_active(row[m_scombomodel.m_scopecols.m_stype]);

      if (astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Reflectivity/%"));
      else if ( astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Trans.(Refl.)/%"));
      else if ( astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
        m_sreflectlabel.set_label(_("Transmission/%"));
      


      if (row[m_scombomodel.m_scopecols.m_sobstruct] < astrocalc::tSMALL && 
          astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
      {
        log_msg.emit(flag, LogView::tINFO, _("Reflector obstruction size unknown, using default value: ") +
        dtostr<double>(astrocalc::OBSTRUCTSIZE, 2) + _("%"));
        m_sobstruct.set_value(astrocalc::OBSTRUCTSIZE);
      }
      else if (row[m_scombomodel.m_scopecols.m_sobstruct] < astrocalc::tSMALL && 
              astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
      {
        log_msg.emit(flag, LogView::tINFO, _("SCT/Mak obstruction size unknown, using default value: ") +
        dtostr<double>(astrocalc::OBSTRUCTSIZESCT, 2) + _("%"));
        m_sobstruct.set_value(astrocalc::OBSTRUCTSIZESCT);
      }
      else if ( astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
      {
        m_sobstruct.set_value(0.0); // we'll ignore any value in the data file since this is always true.
      }
      else
      {
        m_sobstruct.set_value(row[m_scombomodel.m_scopecols.m_sobstruct]);
      }
      

      (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype]) ? 
            m_sobstruct.set_sensitive(false) : m_sobstruct.set_sensitive(true);


      if ( row[m_scombomodel.m_scopecols.m_sreflect] < astrocalc::tSMALL )
      {
        if (astrocalc::REFLECTOR == row[m_scombomodel.m_scopecols.m_stype])
        {
          log_msg.emit(flag, LogView::tINFO, _("Reflector Reflectivity data unknown, using default value: ") +
          dtostr<double>(astrocalc::SCOPEREFLECT, 2) + _("%")); 
          m_sreflect.set_value(astrocalc::SCOPEREFLECT);
        }
        else if (astrocalc::SCTMAK == row[m_scombomodel.m_scopecols.m_stype])
        {
          log_msg.emit(flag, LogView::tINFO, _("SCT/Mak reflectivity data unknown, using default value: ") +
          dtostr<double>(astrocalc::SCOPEREFLECT, 2) + _("%"));
          log_msg.emit(flag, LogView::tINFO, _("SCT/Mak Transmission data unknown, using default value: ") +
          dtostr<double>(astrocalc::SCOPETRANS, 2) + _("%"));
          log_msg.emit(flag, LogView::tINFO, _("SCT/Mak estimated Reflectivity/Transmission value: ") +
          dtostr<double>(astrocalc::SCOPEREFLECT * astrocalc::SCOPETRANS / 100.0, 2) + _("%"));
          m_sreflect.set_value(astrocalc::SCOPEREFLECT * astrocalc::SCOPETRANS / 100.0);
        }
        else if (astrocalc::REFRACTOR == row[m_scombomodel.m_scopecols.m_stype])
        {
          log_msg.emit(flag, LogView::tINFO, _("Refractor transmission data unknown, using default value: ") +
          dtostr<double>(astrocalc::SCOPETRANS, 2) + _("%")); 
          m_sreflect.set_value(astrocalc::SCOPETRANS);
        }
      }
      else
      {
        m_sreflect.set_value(row[m_scombomodel.m_scopecols.m_sreflect]);
      } 
     
    }
  }

  if (true == m_userdataonly) m_sobstruct.set_sensitive(false); // for edit mode
}
