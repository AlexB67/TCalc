#pragma once
#include "eyepieceboxedit.hpp"
#include "telescopeboxedit.hpp"
#include <gtkmm/window.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/label.h>
#include <gtkmm/sizegroup.h>
#include <gtkmm/headerbar.h>

class EquipWindow : public Gtk::Window
{
public:
    explicit EquipWindow(const Glib::RefPtr<Gtk::Application>& app);
    virtual ~EquipWindow(){}
    EquipWindow(const EquipWindow&) = delete;
    EquipWindow& operator=(const EquipWindow& other) = delete;

private:
    std::unique_ptr<EpBox::EditEyepieces> epedit;
    std::unique_ptr<ScopeBox::EditTelescopes> scopeedit;
    Gtk::HeaderBar headerbar;
    Gtk::Label     headerlabel;
    Gtk::StackSwitcher  switcher;
    Gtk::Stack          stack;
    Gtk::Grid           grid;
    Gtk::Grid           epgrid;
    Gtk::Grid           scopegrid;
    bool m_previouslogstate = false;
    bool on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring& phase);
};
