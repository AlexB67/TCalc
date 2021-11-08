#include "eyepiecebox.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"

using AppGlobals::log_msg;


void EpBox::Eyepiecebox::create_epmodel_connection()
{
    AppGlobals::update_ep_data.connect([this](const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                                double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                Glib::ustring>& epdata, const Glib::ustring& oldname)
    {
        m_ecombomodel.update_ep_model(epdata, oldname);
        
        if (!m_with_entry) ep_changed();
        else reset_emodel_entry();
    });

    AppGlobals::new_ep_data.connect([this](const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                                double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                Glib::ustring>& epdata)
    {
       m_ecombomodel.add_ep_to_model(epdata);
    });

    AppGlobals::del_ep_data.connect([this](const Glib::ustring& model_name)
    {
        bool updatecombo = false;

        if (m_with_entry)
        {
            if (m_emodelentry.get_text() == model_name) 
                updatecombo = true;
        }
        else
        {
            if (m_emodel->get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel) == model_name) 
                updatecombo = true;
        }
        
        
        m_ecombomodel.remove_ep_from_model(model_name);
        
        // set the first eyepiece we find after deletion if it was the active one;
        if (m_ecombomodel.get_epmodel()->children().size() > 0 && true == updatecombo)
        {

            Gtk::TreeIter it = m_ecombomodel.get_epmodel()->children().begin();
            if(it->children().size() > 0)
            { 
                auto it2 = it->children().begin();
                if (m_with_entry)
                    m_emodelentry.set_text((*it2)[m_ecombomodel.m_epcols.m_epmodel]);
                else
                    m_emodel->set_active(it2);
                
                current_row = *it2;
                current_iter = it2;
            }
        }
    });

    AppGlobals::move_ep_row_up.connect([this](const Glib::ustring& model_name)
    { m_ecombomodel.swap_ep_rows(model_name);});
}

void EpBox::Eyepiecebox::ep_changed()
{
    const Gtk::TreeModel::iterator iter = m_emodel->get_active();

    if (iter)
    {   
        const Gtk::TreeModel::Row row = *iter;
        if (row) set_values_from_model(row);
    }
}

int EpBox::Eyepiecebox::get_eyepiece_type(const Gtk::TreeModel::Row& row) const
{
    if ( _("Plossl") == row.get_value(m_ecombomodel.m_epcols.m_eptype))
        return 0;
    else if ( _("Ortho") == row.get_value(m_ecombomodel.m_epcols.m_eptype))
        return 1;
    else if ( _("Multi Other") == row.get_value(m_ecombomodel.m_epcols.m_eptype))
        return 2;
    else if (true == row.get_value(m_ecombomodel.m_epcols.m_eptype).empty()) 
        return 2;
    
    return 2;
}

bool EpBox::Eyepiecebox::on_ep_selected(const Gtk::TreeModel::iterator &iter)
{
    const auto& m_eptreemodel = m_ecombomodel.get_epmodel();

    if (iter)
    {
        const auto row1 = *iter;
        Gtk::TreeModel::iterator it;

        for(it = m_eptreemodel->children().begin(); it != m_eptreemodel->children().end(); ++it)
        {
             if ((*it).get_value(m_ecombomodel.m_epcols.m_epbrand) == 
                  row1.get_value(m_ecombomodel.m_epcompletioncols.m_epbrand)) break;
        }
        
        bool found = false;
        for(Gtk::TreeModel::iterator it2 = it->children().begin(); it2 != it->children().end(); ++it2)
        {
            if (row1.get_value(m_ecombomodel.m_epcompletioncols.m_epmodel) == 
                it2->get_value(m_ecombomodel.m_epcols.m_epmodel)) 
            {
                for (auto& connection : con) connection.block();
                m_emodelentry.set_text(row1[m_ecombomodel.m_epcompletioncols.m_epmodel]);
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


bool EpBox::Eyepiecebox::on_ep_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter)
{
    bool m_case_sensitive = false; // okay we we are not using this anymore but might make it an option somewhere

    if (iter)
    {
        size_t found;
        const auto row = *iter;

        if (true == m_case_sensitive)
            found = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcompletioncols.m_epmodel]).find(m_emodelentry.get_text());
        else
            found = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcompletioncols.m_epmodel]).lowercase().find(key);

        if (found != Glib::ustring::npos)
            return true;
    }

    return false;
}

void EpBox::Eyepiecebox::on_cell_data_changed(const Gtk::TreeModel::const_iterator& iter)
{
    if(!iter) 
    {
        m_cell.property_text() = "Select";
        return;
    }
    
    auto row = *iter;
    const Glib::ustring txt = row[m_ecombomodel.m_epcols.m_epmodel];
    m_cell.property_text() = txt;
}

void EpBox::Eyepiecebox::set_values_from_model(const Gtk::TreeRow& row)
{
      using GlibUtils::dtostr;
      bool flag = AppGlobals::LOGFLAG;
      using namespace Astrocalc;
      astrocalc m_calc;

      log_msg.emit(flag, LogView::tINFO, row[m_ecombomodel.m_epcols.m_epmodel] + _(" selected."));
      m_efov.set_value(row[m_ecombomodel.m_epcols.m_epfov]);
      m_eflen.set_value(row[m_ecombomodel.m_epcols.m_epflen]);
      m_etype.set_active(get_eyepiece_type(row));

      if (row[m_ecombomodel.m_epcols.m_eprelief] < astrocalc::tSMALL)
      {
          log_msg.emit(flag, LogView::tINFO, _("Eyerelief unknown."));

          if (astrocalc::PLOSSL == m_etype.get_active_row_number())
          {
              double erelief = m_calc.calc_plossl_eyerelief(m_eflen.get_value());
              log_msg.emit(flag, LogView::tINFO, _("Applying Plossl formula to estimate value: ") 
                         + dtostr<double>(erelief, 2) + _("mm"));
              m_erelief.set_value(erelief);
          }
          else if (astrocalc::ORTHO == m_etype.get_active_row_number())
          {
              double erelief = m_calc.calc_ortho_eyerelief(m_eflen.get_value());
              log_msg.emit(flag, LogView::tINFO, _("Applying Ortho formula to estimate value: ") 
                         + dtostr<double>(erelief, 2) + _("mm"));
              m_erelief.set_value(erelief);
          }
          else
          {
              log_msg.emit(flag, LogView::tINFO, _("No known formula to estimate eye relief."));
              m_erelief.set_value(row[m_ecombomodel.m_epcols.m_eprelief]);
          }
      }
      else
      {
          m_erelief.set_value(row[m_ecombomodel.m_epcols.m_eprelief]);
      }

      if (row[m_ecombomodel.m_epcols.m_eptrans] < astrocalc::tSMALL)
      {
          if (astrocalc::PLOSSL == m_etype.get_active_row_number())
          {
              log_msg.emit(flag, LogView::tINFO, _("Plossl transmission data unknown, using default value: ") 
                         + dtostr<double>(astrocalc::ETRANSPLOSSL, 1) + _("%"));
              m_etrans.set_value(astrocalc::ETRANSPLOSSL);
          }
          else if (astrocalc::ORTHO == m_etype.get_active_row_number())
          {
              log_msg.emit(flag, LogView::tINFO, _("Ortho transmission data unknown, using default value: ") 
                         + dtostr<double>(astrocalc::ETRANSORTHO, 1) + _("%"));
              m_etrans.set_value(astrocalc::ETRANSORTHO);
          }
          else
          {
              log_msg.emit(flag, LogView::tINFO, _("Transmission data unknown, using default value: ") 
                         + dtostr<double>(astrocalc::ETRANS, 1) + _("%"));
              m_etrans.set_value(astrocalc::ETRANS);
          }
      }
      else
      {
          m_etrans.set_value(row[m_ecombomodel.m_epcols.m_eptrans]);
      }

      if (row[m_ecombomodel.m_epcols.m_epfstop] < Astrocalc::astrocalc::tSMALL)
      {
          log_msg.emit(flag, LogView::tINFO, _("Field stop unknown."));
      }

      m_efstop.set_value(row[m_ecombomodel.m_epcols.m_epfstop]);
}

void EpBox::Eyepiecebox::ep_type_changed()
{
    if (Astrocalc::astrocalc::PLOSSL == m_etype.get_active_row_number())
        m_etrans.set_value(Astrocalc::astrocalc::ETRANSPLOSSL);
    else if (Astrocalc::astrocalc::ORTHO == m_etype.get_active_row_number())
        m_etrans.set_value(Astrocalc::astrocalc::ETRANSORTHO);
    else if (Astrocalc::astrocalc::MULTIOTHER == m_etype.get_active_row_number())
        m_etrans.set_value(Astrocalc::astrocalc::ETRANS);
}

void EpBox::Eyepiecebox::set_custom_ep()
{
    m_emodelentry.set_text(_("Custom"));
}

void EpBox::Eyepiecebox::clear_emodel_entry(Gtk::Entry::IconPosition pos)
{
    if (pos == Gtk::Entry::IconPosition::SECONDARY) m_emodelentry.set_text("");
}