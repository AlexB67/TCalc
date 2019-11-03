#include "eyepieceboxedit.hpp"
#include "appglobals.hpp"
#include "fileio.hpp"
#include "glibmmcustomutils.hpp"
#include <fstream>

using GlibUtils::dtostr;

void EpBox::EditEyepieces::set_signal_handlers()
{

    m_button_movedown.signal_clicked().connect(sigc::bind( sigc::mem_fun(*this, &EditEyepieces::swap_rows), true));
    m_button_moveup.signal_clicked().connect(sigc::bind( sigc::mem_fun(*this, &EditEyepieces::swap_rows), false));

    m_emodel.signal_changed().connect([this]() {
        Gtk::TreeModel::iterator iter = m_emodel.get_active();
        if (iter)
        {
            auto row = *iter;
            if (row)
            {
                m_ebarrelsize.set_value(row[m_ecombomodel.m_epcols.m_epbarrel]);
                m_egroups.set_value(row[m_ecombomodel.m_epcols.m_epgroups]);
                m_elements.set_value(row[m_ecombomodel.m_epcols.m_epelements]);

                (row[m_ecombomodel.m_epcols.m_epweight] < Astrocalc::astrocalc::tSMALL) ?
                m_eweight.set_value(0.0) :  m_eweight.set_value(row[m_ecombomodel.m_epcols.m_epweight]);

                Glib::ustring tmp = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcols.m_epcoating]);
                (false == tmp.empty()) ? m_ecoatings.set_active_text(tmp) : 
                                        m_ecoatings.set_active_text(_("unknown"));

                tmp = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcols.m_epmaterial]);
                (false == tmp.empty()) ? m_ematerial.set_active_text(tmp) : 
                                         m_ematerial.set_active_text(_("unknown"));

            }
        }

        if (m_emodel.get_model()->children().size() - 1 == static_cast<size_t>(m_emodel.get_active_row_number()))
            m_button_movedown.set_sensitive(false);
        else
            m_button_movedown.set_sensitive(true);

        if (0 == m_emodel.get_active_row_number())
            m_button_moveup.set_sensitive(false);
        else
            m_button_moveup.set_sensitive(true);

        if(0 == m_emodel.get_model()->children().size())
        {
            m_button_moveup.set_sensitive(false);
            m_button_movedown.set_sensitive(false);
        }
    });

    m_button_new.signal_clicked().connect([this]() {
        enable_widgets(true);
        set_default_values();
        m_button_save.set_sensitive(true);
        m_button_cancel.set_sensitive(true);
        m_button_new.set_sensitive(false);
        m_button_del.set_sensitive(false);
        m_button_moveup.set_sensitive(false);
        m_button_movedown.set_sensitive(false);
        m_emodellabel.set_label(_("Enter an eyepiece description"));
        m_emodel.set_visible(false);
        m_emodelentry.set_visible(true);
        m_emodelentry.set_text("");
        updatemode = false;
    });

    m_button_edit.signal_clicked().connect([this]() {
        enable_widgets(true);
        m_button_save.set_sensitive(true);
        m_button_cancel.set_sensitive(true);
        m_button_edit.set_sensitive(false);
        m_button_del.set_sensitive(false);
        m_button_moveup.set_sensitive(false);
        m_button_movedown.set_sensitive(false);
        m_emodellabel.set_label(_("Enter an eyepiece description"));
        m_emodel.set_visible(false);
        m_emodelentry.set_visible(true);

        Gtk::TreeModel::iterator iter = m_emodel.get_active();
        auto row = *iter;
        m_emodelentry.set_text(row[m_ecombomodel.m_epcols.m_epmodel]);
        updatemode = true;
    });

    m_button_cancel.signal_clicked().connect([this]() {
        enable_widgets(false);
        init();
        m_emodellabel.set_label(_("Select eyepiece"));
        ep_changed();
    });

    m_button_save.signal_clicked().connect([this]() {

        m_emodellabel.set_label(_("Select eyepiece"));

        if (false == validate_ep_data())
        {
            init();
            return;
        }

        unsigned int size = m_emodel.get_model()->children().size();

	    if ( 0 == size && false == updatemode) // is it the first new entry ? then we add
        {                                      //  a separator (but not in the editor window)
            // type column reused as a separator
            AppGlobals::epdata = {"", 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, "separator", 0, 0, 0.0 , "", ""};
            AppGlobals::new_ep_data.emit();   
        };

        std::get<0>(AppGlobals::epdata) = m_emodelentry.get_text();
        std::get<1>(AppGlobals::epdata) = m_efov.get_value();
        std::get<2>(AppGlobals::epdata) = m_eflen.get_value();
        std::get<3>(AppGlobals::epdata) = m_efstop.get_value();
        std::get<4>(AppGlobals::epdata) = m_erelief.get_value();
        std::get<5>(AppGlobals::epdata) = m_etrans.get_value();
        std::get<6>(AppGlobals::epdata) = m_ebarrelsize.get_value();
        std::get<7>(AppGlobals::epdata) = m_etype.get_active_text(); 
        std::get<8>(AppGlobals::epdata) = m_egroups.get_value_as_int();
        std::get<9>(AppGlobals::epdata) = m_elements.get_value_as_int();

        (m_eweight.get_value() < Astrocalc::astrocalc::tSMALL) ? 
        std::get<10>(AppGlobals::epdata) = 0: 
        std::get<10>(AppGlobals::epdata) = m_eweight.get_value();

        (_("unknown") == m_ecoatings.get_active_text()) ?
        std::get<11>(AppGlobals::epdata) = "" :
        std::get<11>(AppGlobals::epdata) = m_ecoatings.get_active_text();

        (_("unknown") == m_ematerial.get_active_text()) ?
        std::get<12>(AppGlobals::epdata) = "" :
        std::get<12>(AppGlobals::epdata) = m_ematerial.get_active_text();

        if (false == updatemode) // it's a new eyepiece
        {
            m_ecombomodel.append_ep_to_model(AppGlobals::epdata);
            AppGlobals::new_ep_data.emit();
        }
        else // it's an existing  eyepiece update
        {
            m_ecombomodel.update_ep_model(AppGlobals::epdata);
            AppGlobals::update_ep_data.emit();
        }

        fileIO::dbfileIO db;
        db.write_ep_user_data(m_emodel, m_ecombomodel);

        init();
        enable_widgets(false);
        AppGlobals::app_notify("User eyepieces updated.", m_app, "tcalc-edit");
    });

    m_button_del.signal_clicked().connect([this]() {

        Glib::ustring message ( _("You are about to delete:\n") + 
                                m_emodel.get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel) +
                                _("\nCick Yes to proceed No to cancel."));


        Gtk::MessageDialog  message_dialog(message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_YES_NO, true);
        int retcode = message_dialog.run();
        m_emodellabel.set_label(_("Select eyepiece"));

        if (retcode != Gtk::RESPONSE_YES)
        {
            init();
            return;
        }

        Glib::ustring epmodelname = 
        static_cast<Glib::ustring>(m_emodel.get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel));
        
        m_ecombomodel.remove_ep_from_model(epmodelname);
        std::get<0>(AppGlobals::epdata) = epmodelname;
        AppGlobals::del_ep_data.emit();

        fileIO::dbfileIO db;
        db.write_ep_user_data(m_emodel, m_ecombomodel);

        init();
        enable_widgets(false);
        AppGlobals::app_notify(_("User eyepieces updated."), m_app, "tcalc_edit");
    });
}

void EpBox::EditEyepieces::swap_rows(const bool movedown)
{

    Glib::ustring epmodelname = 
    static_cast<Glib::ustring>(m_emodel.get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel));

    m_ecombomodel.swap_ep_rows(epmodelname, movedown);
    std::get<0>(AppGlobals::epdata) = epmodelname;
    (true == movedown) ? AppGlobals::move_ep_row_down.emit() : AppGlobals::move_ep_row_up.emit();

    fileIO::dbfileIO db;
    db.write_ep_user_data(m_emodel, m_ecombomodel);
    AppGlobals::app_notify(_("User eyepieces updated."), m_app, "tcalc_edit");
};
