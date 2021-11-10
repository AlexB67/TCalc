#include "eyepieceboxedit.hpp"
#include "appglobals.hpp"
#include "fileio.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include <fstream>

using GlibUtils::dtostr;

void EpBox::EditEyepieces::set_signal_handlers()
{
    m_button_moveup.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::swap_rows));
    m_emodel->signal_changed().connect(sigc::mem_fun(*this, &EditEyepieces::on_emodel_edit_changed));
    m_button_new.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::on_new_button_clicked));
    m_button_edit.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::on_edit_button_clicked));
    m_button_cancel.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::on_cancel_button_clicked));
    m_button_save.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::on_save_button_clicked));
    m_button_del.signal_clicked().connect(sigc::mem_fun(*this, &EditEyepieces::on_del_button_clicked));
}

void EpBox::EditEyepieces::swap_rows()
{

    Glib::ustring epmodelname = 
    static_cast<Glib::ustring>(m_emodel->get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel));

    m_ecombomodel.swap_ep_rows(epmodelname);
    AppGlobals::move_ep_row_up.emit(epmodelname);

    fileIO::dbfileIO db;
    db.write_ep_user_data(*m_emodel, m_ecombomodel);
    AppGlobals::app_notify(_("User eyepieces updated."), m_app, "tcalc_edit");
}

void EpBox::EditEyepieces::on_emodel_edit_changed()
{
    const Gtk::TreeModel::iterator iter = m_emodel->get_active();
    if (iter)
    {
        auto row = *iter;
        if (row)
        {
            m_ebarrelsize.set_value(row[m_ecombomodel.m_epcols.m_epbarrel]);
            m_egroups.set_value(row[m_ecombomodel.m_epcols.m_epgroups]);
            m_elements.set_value(row[m_ecombomodel.m_epcols.m_epelements]);

            (row[m_ecombomodel.m_epcols.m_epweight] < Astrocalc::astrocalc::tSMALL) 
            ? m_eweight.set_value(0.0) : m_eweight.set_value(row[m_ecombomodel.m_epcols.m_epweight]);

            Glib::ustring tmp = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcols.m_epcoating]);
            (false == tmp.empty()) ? m_ecoatings.set_active_text(tmp) : m_ecoatings.set_active_text(_("unknown"));

            tmp = static_cast<Glib::ustring>(row[m_ecombomodel.m_epcols.m_epmaterial]);
            (false == tmp.empty()) ? m_ematerial.set_active_text(tmp) : m_ematerial.set_active_text(_("unknown"));
        }
    }

    if (m_emodel->get_model()->children().size() == 0)
        return;

    if (m_emodel->get_active() == m_emodel->get_model()->children().begin()->children().begin())
        m_button_moveup.set_sensitive(false);
    else if (0 == m_emodel->get_model()->children().begin()->children().size())
        m_button_moveup.set_sensitive(false);
    else
        m_button_moveup.set_sensitive(true);
}

void EpBox::EditEyepieces::on_new_button_clicked()
{
    enable_widgets(true);
    set_default_values();
    m_button_save.set_sensitive(true);
    m_button_cancel.set_sensitive(true);
    m_button_new.set_sensitive(false);
    m_button_edit.set_sensitive(false);
    m_button_del.set_sensitive(false);
    m_button_moveup.set_sensitive(false);
    m_emodellabel.set_label(_("Enter an eyepiece description"));
    m_emodel->set_visible(false);
    m_emodelentry.set_visible(true);
    m_emodelentry.set_text("");
    updatemode = false;
}

void EpBox::EditEyepieces::on_edit_button_clicked()
{
    enable_widgets(true);
    m_button_save.set_sensitive(true);
    m_button_cancel.set_sensitive(true);
    m_button_new.set_sensitive(false);
    m_button_edit.set_sensitive(false);
    m_button_del.set_sensitive(false);
    m_button_moveup.set_sensitive(false);
    m_emodellabel.set_label(_("Enter an eyepiece description"));
    m_emodel->set_visible(false);
    m_emodelentry.set_visible(true);

    Gtk::TreeModel::iterator iter = m_emodel->get_active();
    const auto row = *iter;
    m_emodelentry.set_text(row[m_ecombomodel.m_epcols.m_epmodel]);
    old_model_name = row[m_ecombomodel.m_epcols.m_epmodel];
    updatemode = true;
}

void EpBox::EditEyepieces::on_cancel_button_clicked()
{
    enable_widgets(false);
    init();
    m_emodellabel.set_label(_("Select eyepiece"));
    m_emodel->set_visible(true);
    m_emodelentry.set_visible(false);
    ep_changed();
}

void EpBox::EditEyepieces::on_save_button_clicked()
{
    m_emodellabel.set_label(_("Select eyepiece"));

    if (false == validate_ep_data())
        return;

    std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
               double, double, Glib::ustring, int, int, double, Glib::ustring,
               Glib::ustring> epdata;

    std::get<0>(epdata) = _("User");
    std::get<1>(epdata) = m_emodelentry.get_text();
    std::get<2>(epdata) = m_efov.get_value();
    std::get<3>(epdata) = m_eflen.get_value();
    std::get<4>(epdata) = m_efstop.get_value();
    std::get<5>(epdata) = m_erelief.get_value();
    std::get<6>(epdata) = m_etrans.get_value();
    std::get<7>(epdata) = m_ebarrelsize.get_value();
    std::get<8>(epdata) = m_etype.get_active_text();
    std::get<9>(epdata) = m_egroups.get_value_as_int();
    std::get<10>(epdata) = m_elements.get_value_as_int();

    (m_eweight.get_value() < Astrocalc::astrocalc::tSMALL) 
    ? std::get<11>(epdata) = 0 : std::get<11>(epdata) = m_eweight.get_value();

    (_("unknown") == m_ecoatings.get_active_text()) 
    ? std::get<12>(epdata) = "" : std::get<12>(epdata) = m_ecoatings.get_active_text();
    (_("unknown") == m_ematerial.get_active_text()) 
    ? std::get<13>(epdata) = "" : std::get<13>(epdata) = m_ematerial.get_active_text();

    if (false == updatemode) // it's a new eyepiece
    {
        m_ecombomodel.add_ep_to_model(epdata);
        AppGlobals::new_ep_data.emit(epdata);
    }
    else // it's an existing  eyepiece update
    {
        m_ecombomodel.update_ep_model(epdata, old_model_name);
        AppGlobals::update_ep_data.emit(epdata, old_model_name);
    }

    fileIO::dbfileIO db;
    db.write_ep_user_data(*m_emodel, m_ecombomodel);

    init();
    enable_widgets(false);
    AppGlobals::app_notify("User eyepieces updated.", m_app, "tcalc-edit");
}

void EpBox::EditEyepieces::on_del_button_clicked()
{
    Glib::ustring message ( _("You are about to delete:\n") + 
                            m_emodel->get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel) +
                          _("\nCick Yes to proceed No to cancel."));

    delete_dialog.reset(new Gtk::MessageDialog(message, false, Gtk::MessageType::ERROR, Gtk::ButtonsType::YES_NO, true));
    delete_dialog->set_hide_on_close(true);
    delete_dialog->set_transient_for(*m_parent);
    delete_dialog->show();

    m_emodellabel.set_label(_("Select eyepiece"));

    delete_dialog->signal_response().connect((sigc::track_obj([this](int retcode)
    {
        delete_dialog->hide();

        switch (retcode)
        {
            case Gtk::ResponseType::NO:
            {
                init();
                break;
            }
            case Gtk::ResponseType::YES:
            {
                Glib::ustring epmodelname = 
                m_emodel->get_active()->get_value(m_ecombomodel.m_epcols.m_epmodel);
                    
                auto model = m_emodel->get_model();
                m_emodel->unset_model(); // otherwise delete iterator fails in remove_ep_from_model;
                m_ecombomodel.remove_ep_from_model(epmodelname);
                m_emodel->set_model(model);

                AppGlobals::del_ep_data.emit(epmodelname);

                fileIO::dbfileIO db;
                db.write_ep_user_data(*m_emodel, m_ecombomodel);

                init();
                enable_widgets(false);
                AppGlobals::app_notify(_("User eyepieces updated."), m_app);
            }
        } 
    },*this)));
}