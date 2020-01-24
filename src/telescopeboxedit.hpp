
#pragma once
#include "telescopebox.hpp"
#include "graphswindow.hpp"
#include "ocularwindow.hpp"
#include <gtkmm/sizegroup.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/separator.h>


namespace ScopeBox
{
    class EditTelescopes : public Telescopebox
    {  
         public:
            explicit EditTelescopes(const Glib::RefPtr<Gtk::Application>& app);

            EditTelescopes(const EditTelescopes&) = delete;
            EditTelescopes& operator=(const EditTelescopes& other) = delete;
             ~EditTelescopes(){}
            void init() override;
            void set_default_values() override;

        private:
            Gtk::Entry m_smodelentry;
            Glib::RefPtr<Gtk::Application> m_app;
            Gtk::Label m_smirrorcoatinglabel{_("Mirror coating"), Gtk::ALIGN_START};
            Gtk::Label m_smirrormateriallabel{_("Mirror material"), Gtk::ALIGN_START};
            Gtk::Label m_slenscoatinglabel{_("Lens coating"), Gtk::ALIGN_START};
            Gtk::Label m_slensmateriallabel{_("Lens material"), Gtk::ALIGN_START};
            Gtk::Label m_ssthrellabel{_("Strehl ratio"), Gtk::ALIGN_START};
            Gtk::Label m_sweightlabel{_("Weight/kg"), Gtk::ALIGN_START};
            Gtk::Button m_button_new{_("New")};
            Gtk::Button m_button_edit{_("Edit")};
            Gtk::Button m_button_del{_("Delete")};
            Gtk::Button m_button_save{_("Save")};
            Gtk::Button m_button_cancel{_("Cancel")};
            Gtk::Button m_button_moveup{_("Move up")};
            Gtk::ComboBoxText m_smirrorcoating;
            Gtk::ComboBoxText m_smirrormaterial;
            Gtk::ComboBoxText m_slenscoating;
            Gtk::ComboBoxText m_slensmaterial;
            Ui::SpinEntry   m_sstrehl;
            Ui::SpinEntry   m_sweight;
            Gtk::Separator m_sep;
            Glib::RefPtr<Gtk::SizeGroup> sizegroup;
            Glib::RefPtr<Gtk::SizeGroup> sizegroup2;
            bool updatemode = false;
            void set_signal_handlers();
            void enable_widgets(const bool enable = true);
            bool validate_scope_data() const;
            void swap_rows();
            void create_scopemodel_connection() override {}
    };
}
