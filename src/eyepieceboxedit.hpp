#pragma once
#include "eyepiecebox.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/sizegroup.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/separator.h>
#include <gtkmm/comboboxtext.h>

namespace EpBox
{
class EditEyepieces : public Eyepiecebox
{
public:
    explicit EditEyepieces(const Glib::RefPtr<Gtk::Application> &app);
    EditEyepieces(const Eyepiecebox &) = delete;
    EditEyepieces &operator=(const Eyepiecebox &other) = delete;
    ~EditEyepieces() {}
    void init() override;
    void set_default_values() override;

private:
    Glib::ustring old_model_name;
    Gtk::Entry m_emodelentry;
    Glib::RefPtr<Gtk::Application> m_app;
    Gtk::Label m_ebarrelsizelabel{_("Barrel size/inches"), Gtk::ALIGN_START};
    Gtk::Label m_egroupslabel{_("Group #"), Gtk::ALIGN_START};
    Gtk::Label m_elementslabel{_("Element #"), Gtk::ALIGN_START};
    Gtk::Label m_eweightlabel{_("Weight/grams"), Gtk::ALIGN_START};
    Gtk::Label m_ecoatingslabel{_("Optical coating"), Gtk::ALIGN_START};
    Gtk::Label m_emateriallabel{_("Optical material"), Gtk::ALIGN_START};
    Gtk::Label dummy1, dummy2, dummy3; // to act as spacers
    Ui::SpinEntry m_ebarrelsize;
    Ui::SpinEntry m_egroups;
    Ui::SpinEntry m_elements; 
    Ui::SpinEntry m_eweight;
    Gtk::ComboBoxText   m_ecoatings{true};
    Gtk::ComboBoxText   m_ematerial{true};
    Gtk::Entry          m_ecoatingsentry;
    Gtk::Entry          m_ematerialentry;
    Gtk::Button m_button_new{_("New")};
    Gtk::Button m_button_edit{_("Edit")};
    Gtk::Button m_button_del{_("Delete")};
    Gtk::Button m_button_save{_("Save")};
    Gtk::Button m_button_cancel{_("Cancel")};
    Gtk::Button m_button_moveup{_("Move up")};
    Gtk::Separator m_sep;
    Glib::RefPtr<Gtk::SizeGroup> sizegroup;
    bool updatemode = false;
    void set_signal_handlers();
    void enable_widgets(const bool enable = true);
    bool validate_ep_data() const;
    void swap_rows();
    void create_epmodel_connection() override {}
};
} // namespace EpBox
