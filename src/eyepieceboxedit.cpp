#include "eyepieceboxedit.hpp"
#include "astrocalc.hpp"

EpBox::EditEyepieces::EditEyepieces(const Glib::RefPtr<Gtk::Application> &app) : Eyepiecebox(true), m_app(app)
{
    m_frame.set_label("");
    m_frame.set_shadow_type(Gtk::SHADOW_NONE);

    m_button_del.set_image_from_icon_name("edit-delete-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_del.set_always_show_image(true);
    m_button_save.set_image_from_icon_name("document-save-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_save.set_always_show_image(true);
    m_button_movedown.set_image_from_icon_name("go-down-symbolic.symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_movedown.set_always_show_image(true);
    m_button_moveup.set_image_from_icon_name("go-up-symbolic.symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_moveup.set_always_show_image(true);
    m_button_new.set_image_from_icon_name("document-new-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_new.set_always_show_image(true);
    m_button_cancel.set_image_from_icon_name("document-revert-symbolic-rtl.symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_cancel.set_always_show_image(true);
    m_button_edit.set_image_from_icon_name("document-edit-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_button_edit.set_always_show_image(true);

    m_button_movedown.set_tooltip_text(_("Move the selected model down the list by one row"));
    m_button_moveup.set_tooltip_text(_("Move the selected model up the list by one row"));
    m_button_save.set_tooltip_text(_("save eyepiece model information."));
    m_button_edit.set_tooltip_text(_("Edit eyepiece model information for the current selection."));
    m_button_del.set_tooltip_text(_("Remove eyepiece model information for the current selection."));
    m_button_new.set_tooltip_text(_("Add a new eyepiece."));
    m_button_cancel.set_tooltip_text(_("Cancel editing or adding a new eyepiece."));
    m_emodelentry.set_tooltip_text(_("Enter the model description."));
    m_ebarrelsize.set_tooltip_text(_("The barrel size (diameter) of the eyepiece in inches, usually 1.25, or 2 \","
                                     "Allowed range 0.96\" to 3\"."));

    sizegroup = Gtk::SizeGroup::create(Gtk::SIZE_GROUP_HORIZONTAL);
    sizegroup->add_widget(m_emodel);
    sizegroup->add_widget(m_emodelentry);

    sizegroup2 = Gtk::SizeGroup::create(Gtk::SIZE_GROUP_HORIZONTAL);
    sizegroup2->add_widget(m_eweight);
    sizegroup2->add_widget(m_elements);
    sizegroup2->add_widget(m_egroups);
    sizegroup2->add_widget(m_ebarrelsize);
    sizegroup2->add_widget(m_button_cancel);
    sizegroup2->add_widget(m_button_del);
    sizegroup2->add_widget(m_button_new);
    sizegroup2->add_widget(m_button_edit);
    sizegroup2->add_widget(m_button_moveup);
    sizegroup2->add_widget(m_button_movedown);

    m_sep.set_margin_top(Uidefs::BORDER_WIDTH_SMALL);
    m_sep.set_margin_bottom(Uidefs::BORDER_WIDTH_SMALL);

    m_grid.set_column_spacing(Uidefs::COLUMN_PADDING * 2);
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
    m_grid.attach(m_sep, 0, 9, 4, 1);
    m_grid.attach(m_button_new, 0, 10, 1, 1);
    m_grid.attach(m_button_edit, 1, 10, 1, 1);
    m_grid.attach(m_button_del, 0, 11, 1, 1);
    m_grid.attach(m_button_cancel, 1, 11, 1, 1);
    m_grid.attach(m_button_movedown, 0, 12, 1, 1);
    m_grid.attach(m_button_moveup, 1, 12, 1, 1);
    m_grid.attach(m_button_save, 0, 13, 2, 1);

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

void EpBox::EditEyepieces::set_default_values()
{
    m_eflen.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_efov.set_spin_entry(0.0, 0.0, 120.0, 0.1, 2, true);
    m_efstop.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_erelief.set_spin_entry(0.0, 0.0, 100.0, 0.1, 2, true);
    m_etrans.set_spin_entry(0.0, 0.0, 100.0, 0.5, 2, true);
    m_ebarrelsize.set_spin_entry(0.0, 0.0, 3.0, 0.01, 2, true);
    m_egroups.set_spin_entry(0.0, 0.0, 10.0, 1.0, 0, true);
    m_elements.set_spin_entry(0.0, 0.0, 30.0, 1.0, 0, true);
    m_eweight.set_spin_entry(0.0, 0.0, 10000.0, 1.0, 2, true);

    m_etype.set_active(2);
    m_ecoatings.set_active(4);
    m_ematerial.set_active(3);
}

void EpBox::EditEyepieces::init()
{
    // call this after widgets are shown

    m_emodelentry.set_visible(false);
    m_emodel.set_visible(true);
    m_button_cancel.set_sensitive(false);
    m_button_moveup.set_sensitive(false);
    m_button_movedown.set_sensitive(false);
    m_button_save.set_sensitive(false);
    m_button_new.set_sensitive(true);

    if (m_emodel.get_model()->children().size() > 0)
    {
        m_emodel.set_active(m_emodel.get_model()->children().size() - 1); // section 1
        m_button_edit.set_sensitive(true);
    }

    else
    {
        m_button_edit.set_sensitive(false); // there are no items yet so there is nothing to edit
        m_button_del.set_sensitive(false);
        set_default_values();
    }

    if (m_emodel.get_model()->children().size() > 1) // there must be at least two items for row swap to be active
    {
        m_button_moveup.set_sensitive(true);
        m_button_movedown.set_sensitive(false); // section1  becuase the last row will be set
    }
}

bool EpBox::EditEyepieces::validate_ep_data() const
{
    bool flag = true;
    Glib::ustring message = "The following errors were encountered.\n";
    message += "Update the following fields:\n\n";

    if (m_emodelentry.get_text_length() > (guint16)40)
    {
        message += "Error: Description too long\n";
        flag = false;
    }

    if (Glib::ustring::npos != m_emodelentry.get_text().find("|"))
    {
        message += "Error: the \"|\" character is not allowed\n";
        flag = false;
    }

    if (m_emodelentry.get_text_length() < (guint16)2)
    {
        message += _("Error: Description empty or too short.\n\n");
        message += _("Hint: Model name, model identifier, focal length, barrel size\n");
        message += _("Example: ");
        message += _("Pentax XF XF12 12mm 1.25\"\n\n");
        flag = false;
    }

    if (m_eflen.get_value() != std::clamp<double>(m_eflen.get_value(), 2.0, 100.0))
    {
        message += "Focal length out of range, allowed range 2mm to a 100mm\n";
        flag = false;
    }

    if (m_efstop.get_value() != std::clamp<double>(m_efstop.get_value(), 0.0, 100.0))
    {
        message += "Field stop out of range, allowed range: 0mm to 100mm. Set 0 if unknown.\n";
        flag = false;
    }

    if (m_erelief.get_value() != std::clamp<double>(m_erelief.get_value(), 0.0, 100.0))
    {
        message += "Eye relief of range, allowed range: 0mm to 100mm. Set 0 if unknown.\n";
        flag = false;
    }

    if (m_etrans.get_value() != std::clamp<double>(m_etrans.get_value(), 50.0, 100.0))
    {
        message += "Transmission out of range: allowed range: 50% to 100%. Set to 0 if unknown.\n";
        flag = false;
    }

    else if (m_ebarrelsize.get_value() != std::clamp<double>(m_ebarrelsize.get_value(), 0.96, 3.0))
    {
        message += "Barrel size out of range: allowed range: 0.96 inches to 3 inches.\n";
        flag = false;
    }

    if (false == flag)
    {
        Gtk::MessageDialog message_dialog(message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
        message_dialog.run();
    }

    return flag;
}
