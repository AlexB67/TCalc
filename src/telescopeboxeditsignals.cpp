#include "telescopeboxedit.hpp"
#include "appglobals.hpp"
#include "fileio.hpp"
#include <fstream>

void ScopeBox::EditTelescopes::set_signal_handlers()
{
    m_button_moveup.signal_clicked().connect(sigc::mem_fun(*this, &EditTelescopes::swap_rows));

    m_smodel->signal_changed().connect([this]() {

        Gtk::TreeModel::iterator iter = m_smodel->get_active();
        if (iter)
        {
            const auto row = *iter;
            if (row)
            {
                Glib::ustring tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_smirrorcoating]);
                (false == tmp.empty()) ? m_smirrorcoating.set_active_text(tmp) : 
                                         m_smirrorcoating.set_active_text(_("unknown"));

                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_smirrormaterial]);
                (false == tmp.empty()) ? m_smirrormaterial.set_active_text(tmp) : 
                                         m_smirrormaterial.set_active_text(_("unknown"));
                
                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_slenscoating]);
                (false == tmp.empty()) ? m_slenscoating.set_active_text(tmp) : 
                                         m_slenscoating.set_active_text(_("unknown"));

                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_slensmaterial]);
                (false == tmp.empty()) ? m_slensmaterial.set_active_text(tmp) : 
                                         m_slensmaterial.set_active_text(_("unknown"));

                (row[m_scombomodel.m_scopecols.m_sstrehl] < Astrocalc::astrocalc::tSMALL) ? 
                m_sstrehl.set_value(0.0) : m_sstrehl.set_value(row[m_scombomodel.m_scopecols.m_sstrehl]);

                (row[m_scombomodel.m_scopecols.m_sweight] < Astrocalc::astrocalc::tSMALL) ? 
                m_sweight.set_value(0.0) : m_sweight.set_value(row[m_scombomodel.m_scopecols.m_sweight]);

                (row[m_scombomodel.m_scopecols.m_stube_weight] < Astrocalc::astrocalc::tSMALL) ? 
                m_sstrehl.set_value(0.0) : m_stube_weight.set_value(row[m_scombomodel.m_scopecols.m_stube_weight]);

                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_smount_type]);
                (false == tmp.empty()) ? m_smount_type.set_active_text(tmp) : 
                                         m_smount_type.set_active_text(_("unknown"));
                
                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_sfocuser_type]);
                (false == tmp.empty()) ? m_sfocuser_type.set_active_text(tmp) : 
                                         m_sfocuser_type.set_active_text(_("unknown"));
                
                tmp = static_cast<Glib::ustring>(row[m_scombomodel.m_scopecols.m_sfinder_type]);
                (false == tmp.empty()) ? m_sfinder_type.set_active_text(tmp) : 
                                         m_sfinder_type.set_active_text(_("unknown"));
            }
        }

        if(m_smodel->get_model()->children().size() == 0) return;

        if(m_smodel->get_active() == m_smodel->get_model()->children().begin()->children().begin())
            m_button_moveup.set_sensitive(false);
        else if (0 == m_smodel->get_model()->children().begin()->children().size())
            m_button_moveup.set_sensitive(false);
        else
            m_button_moveup.set_sensitive(true);
    });
    
    m_button_new.signal_clicked().connect([this]() 
    {
        enable_widgets(true);
        set_default_values();
        m_button_save.set_sensitive(true);
        m_button_cancel.set_sensitive(true);
        m_button_edit.set_sensitive(false);
        m_button_del.set_sensitive(false);
        m_button_moveup.set_sensitive(false);
        m_button_new.set_sensitive(false);
        m_smodellabel.set_label(_("Enter a telescope description"));
        m_smodel->set_visible(false);
        m_smodelentry.set_visible(true);
        m_smodelentry.set_text("");
        updatemode = false;
    });

    m_button_edit.signal_clicked().connect([this]() {
        enable_widgets(true);
        m_button_save.set_sensitive(true);
        m_button_cancel.set_sensitive(true);
        m_button_edit.set_sensitive(false);
        m_button_new.set_sensitive(false);
        m_button_del.set_sensitive(true);
        m_button_moveup.set_sensitive(false);
        m_smodellabel.set_label(_("Enter a Telescope description"));
        m_smodel->hide();
        m_smodelentry.show();
        Gtk::TreeModel::iterator iter = m_smodel->get_active();
        auto row = *iter;
        m_smodelentry.set_text(row[m_scombomodel.m_scopecols.m_smodel]);
        old_model_name = row[m_scombomodel.m_scopecols.m_smodel];
        updatemode = true;
    });

    m_button_cancel.signal_clicked().connect([this]() {
        enable_widgets(false);
        init();
        m_smodellabel.set_label(_("Select telescope"));
        m_smodel->set_visible(true);
        m_smodelentry.set_visible(false);
        scope_changed();
        m_sobstruct.set_sensitive(false); // since scope_changed actrivates it for reflectors SCT/Mak
    });

    m_button_save.signal_clicked().connect([this]() {

        m_smodellabel.set_label(_("Select telescope"));

        if (false == validate_scope_data())
        {
           // init();
            return;
        }

        std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
        Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double, 
        Glib::ustring, Glib::ustring, Glib::ustring> scopedata;

        std::get<0>(scopedata) = _("User");
        std::get<1>(scopedata) = m_smodelentry.get_text();
        std::get<2>(scopedata) = m_saperture.get_value();
        std::get<3>(scopedata) = m_sflen.get_value();
        std::get<4>(scopedata) = m_sobstruct.get_value();
        std::get<5>(scopedata) = m_sreflect.get_value();
        std::get<6>(scopedata) = m_stype.get_active_row_number();

        (_("unknown") == m_smirrorcoating.get_active_text()) ?
        std::get<7>(scopedata) = "" :
        std::get<7>(scopedata) = m_smirrorcoating.get_active_text();

        (_("unknown") == m_smirrormaterial.get_active_text()) ?
        std::get<8>(scopedata) = "" :
        std::get<8>(scopedata) = m_smirrormaterial.get_active_text();

        (_("unknown") == m_slenscoating.get_active_text()) ?
        std::get<9>(scopedata) = "" :
        std::get<9>(scopedata) = m_slenscoating.get_active_text();

        (_("unknown") == m_slensmaterial.get_active_text()) ?
        std::get<10>(scopedata) = "" :
        std::get<10>(scopedata) = m_slensmaterial.get_active_text();

        (m_sstrehl.get_value() < Astrocalc::astrocalc::tSMALL) ? 
        std::get<11>(scopedata) = 0 : 
        std::get<11>(scopedata) = m_sstrehl.get_value();

        (m_sweight.get_value() < Astrocalc::astrocalc::tSMALL) ? 
        std::get<12>(scopedata) = 0 : 
        std::get<12>(scopedata) = m_sweight.get_value();
        
        (m_stube_weight.get_value() < Astrocalc::astrocalc::tSMALL) ? 
        std::get<13>(scopedata) = 0 : 
        std::get<13>(scopedata) = m_stube_weight.get_value();

        (_("unknown") == m_smount_type.get_active_text()) ?
        std::get<14>(scopedata) = "" :
        std::get<14>(scopedata) = m_smount_type.get_active_text();

        (_("unknown") == m_sfocuser_type.get_active_text()) ?
        std::get<15>(scopedata) = "" :
        std::get<15>(scopedata) = m_sfocuser_type.get_active_text();

        (_("unknown") == m_sfinder_type.get_active_text()) ?
        std::get<16>(scopedata) = "" :
        std::get<16>(scopedata) = m_sfinder_type.get_active_text();

        if (false == updatemode) // it's a new telescope
        {
            m_scombomodel.add_scope_to_model(scopedata);
            AppGlobals::new_scope_data.emit(scopedata);
        }
        else // it's an existing telescope update
        {
            m_scombomodel.update_scope_model(scopedata, old_model_name);
            AppGlobals::update_scope_data.emit(scopedata, old_model_name);
        }

        fileIO::dbfileIO db;
        db.write_scope_user_data(*m_smodel, m_scombomodel);

        init();
        enable_widgets(false);
        AppGlobals::app_notify("User telescopes updated.", m_app, "tcalc-edit");
    });

    m_button_del.signal_clicked().connect([this]() {

        Glib::ustring message ( _("You are about to delete:\n") + 
                                m_smodel->get_active()->get_value(m_scombomodel.m_scopecols.m_smodel) +
                                _("\nCick Yes to proceed or No to cancel."));

        Gtk::MessageDialog  message_dialog(message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_YES_NO, true);
        int retcode = message_dialog.run();
        m_smodellabel.set_label(_("Select Telescope"));

        if (retcode != Gtk::RESPONSE_YES)
        {
            init();
            return;
        }
        Glib::ustring scopemodelname = 
        static_cast<Glib::ustring>(m_smodel->get_active()->get_value(m_scombomodel.m_scopecols.m_smodel));
        
        auto model = m_smodel->get_model();
        m_smodel->unset_model(); // otherwise delete iterator fails in remove_ep_from_model;
        m_scombomodel.remove_scope_from_model(scopemodelname);
        m_smodel->set_model(model);

        AppGlobals::del_scope_data.emit(scopemodelname);

        fileIO::dbfileIO db;
        db.write_scope_user_data(*m_smodel, m_scombomodel);

        init();
        enable_widgets(false);
        AppGlobals::app_notify(_("User telescopes updated."), m_app, "tcalc_edit");
    });
}

void ScopeBox::EditTelescopes::swap_rows()
{

    Glib::ustring scopemodelname = m_smodel->get_active()->get_value(m_scombomodel.m_scopecols.m_smodel);

    m_scombomodel.swap_scope_rows(scopemodelname);
    AppGlobals::move_scope_row_up.emit(scopemodelname);
    
    fileIO::dbfileIO db;
    db.write_scope_user_data(*m_smodel, m_scombomodel);
    AppGlobals::app_notify(_("User telescopes updated."), m_app, "tcalc_edit");
};
