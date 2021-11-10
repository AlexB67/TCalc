#include "eyepieceboxedit.hpp"
#include "astrocalclib/astrocalc.hpp"

EpBox::EditEyepieces::EditEyepieces(const Glib::RefPtr<Gtk::Application> &app, Gtk::Window *parent) 
: Eyepiecebox(false, true), m_app(app), m_parent(parent)
{
    m_frame.set_label("");

    m_button_del.set_image_from_icon_name("edit-delete-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_save.set_image_from_icon_name("document-save-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_moveup.set_image_from_icon_name("go-up-symbolic.symbolic", Gtk::IconSize::INHERIT, true);
    m_button_new.set_image_from_icon_name("document-new-symbolic", Gtk::IconSize::INHERIT, true);
    m_button_cancel.set_image_from_icon_name("document-revert-symbolic-rtl.symbolic", Gtk::IconSize::INHERIT, true);
    m_button_edit.set_image_from_icon_name("document-edit-symbolic", Gtk::IconSize::INHERIT, true);

    m_button_moveup.set_tooltip_text(_("Move the selected model up the list by one row. Useful or reordering."));
    m_button_save.set_tooltip_text(_("save eyepiece model information."));
    m_button_edit.set_tooltip_text(_("Edit eyepiece model information for the current selection."));
    m_button_del.set_tooltip_text(_("Remove eyepiece model information for the current selection."));
    m_button_new.set_tooltip_text(_("Add a new eyepiece."));
    m_button_cancel.set_tooltip_text(_("Cancel editing or adding a new eyepiece."));
    m_emodelentry.set_tooltip_text(_("Enter the model description."));
    m_ebarrelsize.set_tooltip_text(_("The barrel size (diameter) of the eyepiece in inches, usually 1.25, or 2 \","
                                     "Allowed range 0.96\" to 6\"."));
    m_ecoatings.set_tooltip_text(_("The type of optical coating used on lenses."));
    m_ematerial.set_tooltip_text(_("The type of glass used for lens construction."));
    m_eweight.set_tooltip_text(_("The weight of the eyepiece in grams."));
    m_egroups.set_tooltip_text(_("The number of optical groups. Leave at 0 if unknown."));
    m_elements.set_tooltip_text(_("The total number of optical elements. Leave at zero if unknown."));

    m_button_new.get_style_context()->add_class("suggested-action");

    sizegroup = Gtk::SizeGroup::create(Gtk::SizeGroup::Mode::HORIZONTAL);
    sizegroup->add_widget(*m_emodel);
    sizegroup->add_widget(m_emodelentry);

    m_sep.set_margin_top(Uidefs::BORDER_WIDTH_SMALL);
    m_sep.set_margin_bottom(Uidefs::BORDER_WIDTH_SMALL);
    m_grid.set_column_spacing(Uidefs::COLUMN_PADDING * 3);
    m_grid.set_column_homogeneous(true);
    
    m_grid.attach(m_emodelentry, 0, 1, 2, 1);
    m_grid.attach(m_ebarrelsizelabel, 2, 2);
    m_grid.attach(m_egroupslabel, 2, 3);
    m_grid.attach(m_elementslabel, 2, 4);
    m_grid.attach(m_eweightlabel, 2, 5);
    m_grid.attach(m_ecoatingslabel, 2, 6);
    m_grid.attach(m_emateriallabel, 2, 7);
    m_grid.attach(m_ebarrelsize, 3, 2);
    m_grid.attach(m_egroups, 3, 3);
    m_grid.attach(m_elements, 3, 4);
    m_grid.attach(m_eweight, 3, 5);
    m_grid.attach(m_ecoatings, 3, 6);
    m_grid.attach(m_ematerial, 3, 7); 
    m_grid.attach(m_sep, 0, 8, 4, 1);
    m_grid.attach(dummy1, 0, 9);
    m_grid.attach(dummy2, 0, 10);
    m_grid.attach(dummy3, 0, 11);
    m_grid.attach(m_button_new, 0, 12, 1, 1);
    m_grid.attach(m_button_edit, 1, 12, 1, 1);
    m_grid.attach(m_button_del, 0, 13, 1, 1);
    m_grid.attach(m_button_cancel, 1, 13, 1, 1);
    m_grid.attach(m_button_moveup, 1, 14, 1, 1);
    m_grid.attach(m_button_save, 0, 14, 1, 1);

    m_ematerial.insert(0, _("ED Glass"));
    m_ematerial.insert(1, _("Lanthanum"));
    m_ematerial.insert(2, _("other"));
    m_ematerial.insert(3, _("unknown"));


    m_ecoatings.insert(0, _("FMC-BE"));
    m_ecoatings.insert(1, _("FMC"));
    m_ecoatings.insert(2, _("SC"));
    m_ecoatings.insert(3, _("other"));
    m_ecoatings.insert(4, _("unknown"));

    enable_widgets(false);
    set_signal_handlers();
}

void EpBox::EditEyepieces::enable_widgets(const bool enable)
{
    m_efov.set_sensitive(enable);
    m_eflen.set_sensitive(enable);
    m_efstop.set_sensitive(enable);
    m_erelief.set_sensitive(enable);
    m_etrans.set_sensitive(enable);
    m_ebarrelsize.set_sensitive(enable);
    m_etype.set_sensitive(enable);
    m_egroups.set_sensitive(enable);
    m_elements.set_sensitive(enable);
    m_eweight.set_sensitive(enable);
    m_ecoatings.set_sensitive(enable);
    m_ematerial.set_sensitive(enable);
}

void EpBox::EditEyepieces::init()
{
    // call this after widgets are shown

    m_emodelentry.set_visible(false);
    m_emodel->set_visible(true);
    //m_emodel->get_entry()->set_can_focus(false);
    m_button_cancel.set_sensitive(false);
    m_button_moveup.set_sensitive(false);
    m_button_save.set_sensitive(false);
    m_button_new.set_sensitive(true);

    Gtk::TreeNodeChildren::size_type size = 0;

    if (m_emodel->get_model()->children().size() > 0)
    {
        m_emodel->set_active(m_emodel->get_model()->children().size() - 1); // section 1

        const Gtk::TreeIter it = m_emodel->get_model()->children().begin(); // First parent is "User" category

        for (auto it2 = it->children().begin(); it2 != it2->children().end(); ++it2)
        {
            if (static_cast<Glib::ustring>((*it2)[m_ecombomodel.m_epcols.m_epmodel]).length() > 0)
            {
                m_emodel->set_active(it2);
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
        m_emodel->set_visible(true);
        m_emodelentry.set_visible(false);
        set_default_values();
    }

    if (size > 1) // there must be at least two items for row swap to be active
        m_button_moveup.set_sensitive(false);
}

void EpBox::EditEyepieces::set_default_values()
{
    m_eflen.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_efov.set_spin_entry(0.0, 0.0, 120.0, 0.1, 2, true);
    m_efstop.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_erelief.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_etrans.set_spin_entry(0.0, 0.0, 100.0, 0.5, 2, true);
    m_ebarrelsize.set_spin_entry(0.0, 0.0, 6.0, 0.01, 2, true);
    m_egroups.set_spin_entry(0.0, 0.0, 10.0, 1.0, 0, true);
    m_elements.set_spin_entry(0.0, 0.0, 30.0, 1.0, 0, true);
    m_eweight.set_spin_entry(0.0, 0.0, 10000.0, 1.0, 2, true);

    m_etype.set_active(2);
    m_ecoatings.set_active(4);
    m_ematerial.set_active(3);
}

bool EpBox::EditEyepieces::validate_ep_data()
{
    bool flag = true;
    Glib::ustring title = _("The following errors were encountered. Please correct the following: \r\n");
    Glib::ustring message;

    if (m_emodelentry.get_text_length() > (guint16)40)
    {
        message += _("Error: Description too long.\n");
        flag = false;
    }

    if (Glib::ustring::npos != m_emodelentry.get_text().find("|"))
    {
        message += _("Error: the \"|\" character is not allowed\r\n");
        flag = false;
    }

    if (m_emodelentry.get_text_length() < (guint16)2)
    {
        message += _("Error: Description empty or too short.\r\n");
        message += _("Hint: Model name, model identifier, focal length, barrel size \n");
        message += _("Example: Pentax XF XF12 12mm 1.25\" \n\n");
        flag = false;
    }

    if (m_eflen.get_value() != std::clamp<double>(m_eflen.get_value(), 2.0, 100.0))
    {
        message += _("Error: Focal length out of range, allowed range 2mm to a 100mm. \n\n");
        flag = false;
    }

    if (m_efstop.get_value() != std::clamp<double>(m_efstop.get_value(), 0.0, 100.0))
    {
        message += _("Error: Field stop out of range, allowed range: 0mm to 100mm. Set 0 if unknown.\n\n");
        flag = false;
    }

    if (m_erelief.get_value() != std::clamp<double>(m_erelief.get_value(), 0.0, 100.0))
    {
        message += _("Error: Eye relief of range, allowed range: 0mm to 100mm. Set 0 if unknown.\n\n");
        flag = false;
    }

    else if (m_ebarrelsize.get_value() != std::clamp<double>(m_ebarrelsize.get_value(), 0.96, 6.0))
    {
        message += _("Error: Barrel size out of range: allowed range: 0.96 inches to 6 inches.\n\n");
        flag = false;
    }

    if (m_etrans.get_value() != std::clamp<double>(m_etrans.get_value(), 0.0, 100.0))
    {
        message += _("Transmssion out of range, ");
        message += _("allowed range: 0% to 100%. Set 0% if unknown.")  + '\n';
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
