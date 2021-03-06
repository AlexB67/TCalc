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
       ep_changed();
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

        if (m_emodel->get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel) == model_name) 
            updatecombo = true;
        
        m_ecombomodel.remove_ep_from_model(model_name);
        
        // set the first eyepiece we find after deletion if it was the active one;
        if (m_ecombomodel.get_epmodel()->children().size() > 0 && true == updatecombo)
        {

            Gtk::TreeIter it = m_ecombomodel.get_epmodel()->children().begin();
            if(it->children().size() > 0)
            { 
                auto it2 = it->children().begin();
                m_emodel->set_active(it2);
            }
        }
    });

     AppGlobals::move_ep_row_up.connect([this](const Glib::ustring& model_name){
        m_ecombomodel.swap_ep_rows(model_name);
    });
}

void EpBox::Eyepiecebox::ep_changed()
{
    using GlibUtils::dtostr;
    bool flag = AppGlobals::LOGFLAG;
    using namespace Astrocalc;
    astrocalc m_calc;

    const Gtk::TreeModel::iterator iter = m_emodel->get_active();

    if (iter)
    {   
        const Gtk::TreeModel::Row row = *iter;
        if (row)
        {
            log_msg.emit(flag, LogView::tINFO, row[m_ecombomodel.m_epcols.m_epmodel] + _(" selected."));
            auto *entry = m_emodel->get_entry();
            entry->set_text(row[m_ecombomodel.m_epcols.m_epmodel]);
            m_efov.set_value(row[m_ecombomodel.m_epcols.m_epfov]);
            m_eflen.set_value(row[m_ecombomodel.m_epcols.m_epflen]);
            m_etype.set_active(get_eyepiece_type(row));

            if (row[m_ecombomodel.m_epcols.m_eprelief] < astrocalc::tSMALL)
            {
                log_msg.emit(flag, LogView::tINFO, _("Eyerelief unknown."));

                if (astrocalc::PLOSSL == m_etype.get_active_row_number())
                {
                    double erelief =  m_calc.calc_plossl_eyerelief(m_eflen.get_value());
                    log_msg.emit(flag, LogView::tINFO, _("Applying Plossl formula to estimate value: ")
                    + dtostr<double>(erelief, 2) + _("mm"));
                    m_erelief.set_value(erelief);
                }
                else if (astrocalc::ORTHO == m_etype.get_active_row_number())
                {
                    double erelief =  m_calc.calc_ortho_eyerelief(m_eflen.get_value());
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
                    log_msg.emit(flag, LogView::tINFO, _("Plossl transmission data unknown, using default value: ") +
                    dtostr<double>(astrocalc::ETRANSPLOSSL, 1) + _("%"));
                    m_etrans.set_value(astrocalc::ETRANSPLOSSL);
                }
                else if (astrocalc::ORTHO == m_etype.get_active_row_number())
                {
                    log_msg.emit(flag, LogView::tINFO, _("Ortho transmission data unknown, using default value: ") +
                    dtostr<double>(astrocalc::ETRANSORTHO, 1) + _("%"));
                    m_etrans.set_value(astrocalc::ETRANSORTHO);
                }
                else
                {
                    log_msg.emit(flag, LogView::tINFO, _("Transmission data unknown, using default value: ") +
                    dtostr<double>(astrocalc::ETRANS, 1) + _("%"));
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
    }
}

int EpBox::Eyepiecebox::get_eyepiece_type(const Gtk::TreeModel::Row& row) const
{
    if ( _("Plossl") == row[m_ecombomodel.m_epcols.m_eptype])
        return 0;
    else if ( _("Ortho") == row[m_ecombomodel.m_epcols.m_eptype])
        return 1;
    else if ( _("Multi Other") == row[m_ecombomodel.m_epcols.m_eptype])
        return 2;
    else if (true == static_cast<Glib::ustring>(row[m_ecombomodel.m_epcols.m_eptype]).empty()) 
        return 2;
    
    return 2;
}
