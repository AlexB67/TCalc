#include "telescopeboxedit.hpp"
#include "astrocalclib/astrocalc.hpp"
#include <gtkmm/messagedialog.h>

ScopeBox::EditTelescopes::EditTelescopes(const Glib::RefPtr<Gtk::Application> &app, Gtk::Window *parent) 
: Telescopebox(false, true), m_app(app), m_parent(parent)
{
    m_frame.set_label("");
    //m_frame.set_shadow_type(Gtk::SHADOW_NONE);
    m_button_del.set_image_from_icon_name("edit-delete-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_save.set_image_from_icon_name("document-save-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_moveup.set_image_from_icon_name("go-up-symbolic.symbolic", Gtk::IconSize::INHERIT, true);
    m_button_new.set_image_from_icon_name("document-new-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_cancel.set_image_from_icon_name("document-revert-symbolic-rtl.symbolic", Gtk::IconSize::INHERIT, true);
    m_button_edit.set_image_from_icon_name("document-edit-symbolic", Gtk::IconSize::INHERIT, true);

    m_button_moveup.set_tooltip_text(_("Move the selected model up the list by one row"));
    m_button_save.set_tooltip_text(_("save telescope model information."));
    m_button_edit.set_tooltip_text(_("Edit telescope model information for the current selection."));
    m_button_del.set_tooltip_text(_("Remove telescope model information for the current selection."));
    m_button_new.set_tooltip_text(_("Add a new telescope."));
    m_button_cancel.set_tooltip_text(_("Cancel editing or adding a new telescope."));
    m_smirrorcoating.set_tooltip_text(_("The coating used on mirrors."));
    m_smirrormaterial.set_tooltip_text(_("The material used for mirror construction."));
    m_slenscoating.set_tooltip_text(_("The type of lens coating."));
    m_slensmaterial.set_tooltip_text(_("The material (or glass) used for construction of lenses."));
    m_sstrehl.set_tooltip_text(_("A measure of the quality of telescope optics, the theoretical maximum value is 1, " 
                              "typically 0.80 - 0.98. Leave at 0 if unknown."));
    m_sweight.set_tooltip_text(_("The weight of the telescope in kilograms (including mount). Leave at 0 if unknown."));
    m_stube_weight.set_tooltip_text(_("The weight of the mount in kilograms. Leave at 0 if unknown."));
    m_smount_type.set_tooltip_text(_("The type of mount, for example, Dobsonian, EQ, alt-azimuth, etc."));
    m_sfocuser_type.set_tooltip_text(_("Focuser model details"));
    m_sfinder_type.set_tooltip_text(_("The type of finder, for example, red dot finder, finder scope, etc."));

    m_button_new.get_style_context()->add_class("suggested-action");

    sizegroup = Gtk::SizeGroup::create(Gtk::SizeGroup::Mode::HORIZONTAL);
    sizegroup->add_widget(*m_smodel);
    sizegroup->add_widget(m_smodelentry);

    m_sep.set_margin_top(Uidefs::BORDER_WIDTH_SMALL);
    m_sep.set_margin_bottom(Uidefs::BORDER_WIDTH_SMALL);
    m_sep.set_vexpand(false);
    m_grid.set_column_spacing(Uidefs::COLUMN_PADDING * 3);
    m_grid.set_column_homogeneous(true);
    
    m_grid.attach(m_smodelentry, 0, 1, 2, 1);
    m_grid.attach(m_smirrorcoatinglabel, 2, 3);
    m_grid.attach(m_smirrorcoating, 3, 3);
    m_grid.attach(m_smirrormateriallabel, 2, 4);
    m_grid.attach(m_smirrormaterial, 3, 4);
    m_grid.attach(m_slenscoatinglabel, 2, 5);
    m_grid.attach(m_slenscoating, 3, 5);
    m_grid.attach(m_slensmateriallabel, 2, 6);
    m_grid.attach(m_slensmaterial, 3, 6);
    m_grid.attach(m_ssthrellabel, 2, 7);
    m_grid.attach(m_sstrehl, 3, 7);
    m_grid.attach(m_sweightlabel, 2, 8);
    m_grid.attach(m_sweight, 3, 8);
    m_grid.attach(m_sfocuser_type_label, 0, 9);
    m_grid.attach(m_sfocuser_type, 1, 9);
    m_grid.attach(m_sfinder_type_label, 0, 10);
    m_grid.attach(m_sfinder_type, 1, 10);
    m_grid.attach(m_stube_weight_label, 2, 9);
    m_grid.attach(m_stube_weight, 3, 9);
    m_grid.attach(m_smount_type_label, 2, 10);
    m_grid.attach(m_smount_type, 3, 10);
    m_grid.attach(m_sep, 0, 11, 4, 1);
    m_grid.attach(m_button_new, 0, 12, 1, 1);
    m_grid.attach(m_button_edit, 1, 12, 1, 1);
    m_grid.attach(m_button_del, 0, 13, 1, 1);
    m_grid.attach(m_button_cancel, 1, 13, 1, 1);
    m_grid.attach(m_button_moveup, 1, 14, 1, 1);
    m_grid.attach(m_button_save, 0, 14, 1, 1);

    m_smirrorcoating.insert(0, _("Hilux"));
    m_smirrorcoating.insert(1, _("Silicon monoxide"));
    m_smirrorcoating.insert(2, _("Silicon dioxide"));
    m_smirrorcoating.insert(3, _("other"));
    m_smirrorcoating.insert(4, _("unknown"));

    m_smirrormaterial.insert(0, _("Pyrex"));
    m_smirrormaterial.insert(1, _("other"));
    m_smirrormaterial.insert(2, _("unknown"));

    m_slensmaterial.insert(0, ("ED glass"));
    m_slensmaterial.insert(1, ("Lanthanum"));
    m_slensmaterial.insert(2, _("other"));
    m_slensmaterial.insert(3, _("unknown"));

    m_slenscoating.insert(0, _("FMC-BE"));
    m_slenscoating.insert(1, _("FMC"));
    m_slenscoating.insert(2, _("SC"));
    m_slenscoating.insert(3, _("Starbright XLT"));
    m_slenscoating.insert(4, _("other"));
    m_slenscoating.insert(5, _("unknown"));

    m_sfinder_type.insert(0, _("Red dot"));
    m_sfinder_type.insert(1, _("Finder scope"));
    m_sfinder_type.insert(2, _("Rigel"));
    m_sfinder_type.insert(3, _("Telrad"));
    m_sfinder_type.insert(4, _("unknown"));

    m_sfocuser_type.insert(0, _("Crayford 1.25\"/2\" single speed."));
    m_sfocuser_type.insert(1, _("Rack and pinion"));
    m_sfocuser_type.insert(2, _("Dual speed 1.25/2\""));
    m_sfocuser_type.insert(3, _("unknown"));

    m_smount_type.insert(0, _("Dobsonian"));
    m_smount_type.insert(1, _("EQ1"));
    m_smount_type.insert(2, _("EQ2"));
    m_smount_type.insert(3, _("EQ5"));
    m_smount_type.insert(4, _("Tripod"));
    m_smount_type.insert(5, _("Alt-azimuth"));
    m_smount_type.insert(6, _("unknown"));


    enable_widgets(false);
    set_signal_handlers();
}

void ScopeBox::EditTelescopes::enable_widgets(const bool enable)
{
    m_saperture.set_sensitive(enable);
    m_sflen.set_sensitive(enable);
    m_sreflect.set_sensitive(enable);
    m_sobstruct.set_sensitive(enable);
    m_sfratio.set_sensitive(enable);
    m_stype.set_sensitive(enable);
    m_smirrormaterial.set_sensitive(enable);
    m_smirrorcoating.set_sensitive(enable);
    m_slensmaterial.set_sensitive(enable);
    m_slenscoating.set_sensitive(enable);
    m_sstrehl.set_sensitive(enable);
    m_sweight.set_sensitive(enable);
    m_stube_weight.set_sensitive(enable);
    m_sfocuser_type.set_sensitive(enable);
    m_smount_type.set_sensitive(enable);
    m_sfinder_type.set_sensitive(enable);
}

void ScopeBox::EditTelescopes::init()
{
    m_smodelentry.set_visible(false);
   // m_smodel->get_entry()->set_can_focus(false);
    m_smodel->set_visible(true);
    m_button_cancel.set_sensitive(false);
    m_button_moveup.set_sensitive(false);
    m_button_save.set_sensitive(false);
    m_button_new.set_sensitive(true);

    Gtk::TreeNodeChildren::size_type size = 0;

    if (m_smodel->get_model()->children().size() > 0)
    {
        m_smodel->set_active(m_smodel->get_model()->children().size() - 1); // section 1

        const Gtk::TreeIter it = m_smodel->get_model()->children().begin(); // First parent is "User" category

        for (auto it2 = it->children().begin(); it2 != it2->children().end(); ++it2)
        {
            if (static_cast<Glib::ustring>((*it2)[m_scombomodel.m_scopecols.m_smodel]).length() > 0)
            {
                m_smodel->set_active(it2);
                size = (*it).children().size();
                break;
            }
        }

        if ( size > 0 ) 
        {
            m_button_edit.set_sensitive(true);
            m_button_del.set_sensitive(true);
        }
    }
    else
    {
        m_button_edit.set_sensitive(false); // there are no items yet so there is nothing to edit
        m_button_del.set_sensitive(false);
        m_smodel->set_visible(true);
        m_smodelentry.set_visible(false);
        set_default_values();
    }
}

void ScopeBox::EditTelescopes::set_default_values()
{
    m_sflen.set_spin_entry(00.0, 0.0, 4000.0, 1.0, 2, true);
    m_saperture.set_spin_entry(0.0, 0.0, 1000.0, 0.5, 2, true);
    m_sfratio.set_spin_entry(0, 0.0, 100.0, 0.01, 2, true);
    m_sreflect.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_sobstruct.set_spin_entry(0, 0.0, 50.0, 0.1, 2, true);
    m_sweight.set_spin_entry(0.0, 0.0, 1000.0, 0.1, 2, true);
    m_stube_weight.set_spin_entry(0.0, 0.0, 1000.0, 0.1, 2, true);
    m_sstrehl.set_spin_entry(0.0, 0.0, 1.0, 0.01, 2, true);
    m_smirrormaterial.set_active(2);
    m_smirrorcoating.set_active(4);
    m_slensmaterial.set_active(3);
    m_slenscoating.set_active(5);
    m_smount_type.set_active(6);
    m_sfinder_type.set_active(4);
    m_sfocuser_type.set_active(3);
    m_stype.set_active(0);
}

bool ScopeBox::EditTelescopes::validate_scope_data()
{
    bool flag = true;
    Glib::ustring title = _("The following errors were encountered. Please correct the following: \r\n");
    Glib::ustring message;

    if (m_smodelentry.get_text_length() > (guint16)40)
    {
        message += _("Error: Description too long.\n");
        flag = false;
    }

    if (Glib::ustring::npos != m_smodelentry.get_text().find("|"))
    {
        message += _("Error: the \"|\" character is not allowed" + '\n');
        flag = false;
    }

    if (m_smodelentry.get_text_length() < (guint16)2)
    {
        message += _("Error: Description empty or too short.\n");
        message += _("Hint: Model name, model identifier, focal length, barrel size.\n");
        message += _("Example: Orion US SkyQuest 10\" Dob\n\n");
        flag = false;
    }

    if (m_sflen.get_value() != std::clamp<double>(m_sflen.get_value(), 10.0, 4000.0))
    {
        message += _("Focal length out of range, allowed range 10 mm to a 4000mm.\n\n");
        flag = false;
    }

    if (m_saperture.get_value() != std::clamp<double>(m_saperture.get_value(), 10.0, 1000.0))
    {
        message += _("Aperture out of range, allowed range: 10mm to 1000mm. Set 0 if unknown.\n\n");
        flag = false;
    }

    if (m_sreflect.get_value() != std::clamp<double>(m_sreflect.get_value(), 0.0, 100.0))
    {
        message += _("Reflectivity and/or transmssion out of range.\n");
        message += _("Allowed range: 0% to 100%. Set 0% if unknown.\n\n");
        flag = false;
    }

    if (m_sobstruct.get_value() != std::clamp<double>(m_sobstruct.get_value(), 0.0, 100.0))
    {
        message += _("Transmission out of range: allowed range: 0% to 100%. Set to 0 if unknown.\n");
        flag = false;
    }

    if (m_sfocuser_type.get_active_text().find('&') != Glib::ustring::npos)
    {
        message += _("Focuser type: & is not allowed.\n");
        flag = false;
    }

    if (m_smount_type.get_active_text().find('&') != Glib::ustring::npos)
    {
        message += _("Mount type: & is not allowed.\n");
        flag = false;
    }

    if (false == flag)
    {
        validate_dialog.reset(new Gtk::MessageDialog(message, false, Gtk::MessageType::ERROR, Gtk::ButtonsType::OK, true));
        validate_dialog->set_hide_on_close(true);
        validate_dialog->set_transient_for(*m_parent);
        validate_dialog->show();

        validate_dialog->signal_response().connect((sigc::track_obj([this](int retcode) 
        {
            validate_dialog->hide();
            switch (retcode)
            {
                case Gtk::ResponseType::OK:
                {
                   // validate_dialog->hide();
                    break;
                }
            }
        }, *this)));
    }

    return flag;
}
