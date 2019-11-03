#pragma once
#include <gtkmm/window.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/switch.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <glibmm/keyfile.h>
#include <giomm/settings.h>
#include "gtkmmcustomutils.hpp"
#include "loggerbox.hpp"
#include <iostream>

class PrefsWindow : public Gtk::Window
{
public:
	public:
        PrefsWindow();
        virtual ~PrefsWindow(){}
        PrefsWindow(const PrefsWindow&) = delete;
        PrefsWindow& operator=(const PrefsWindow& other) = delete;
        const Gtk::Switch& showtime_ref() const {return *showtime;}
        const Gtk::Switch& showcolour_ref() const {return *showcolour;}
        const Gtk::Switch& usemonospace_ref() const {return *usemonospace;}

private:
    Gtk::StackSidebar   sidebar;
    Gtk::Stack          stack;
    Gtk::Grid           grid;
    Gtk::Grid           appearancegrid;
    Gtk::Grid           scopegrid;
    Gtk::Grid           epgrid;
    Gtk::Switch         *preferdarktheme;
    Gtk::Switch         *showtime;
    Gtk::Switch         *showcolour;
    Gtk::Switch         *usemonospace;
    Gtk::Switch         *drawframes;
    Gtk::Label          preferdarkthemelabel{_("Prefer dark theme variant:"), Gtk::ALIGN_START};
    Gtk::Label          showtimelabel{_("Enable date and time stamp:"), Gtk::ALIGN_START};
    Gtk::Label          showcolourlabel{_("Enable colour for warnings:"), Gtk::ALIGN_START};
    Gtk::Label          usemonospacelabel{_("Use monospace font:"), Gtk::ALIGN_START};
    Gtk::Label          sreflectlabel{_("Reflectivity/%:"), Gtk::ALIGN_START};
    Gtk::Label          stranslabel{_("Transmission/%:"), Gtk::ALIGN_START};
    Gtk::Label          sobstructlabel{_("Reflector obstruction size/%:"), Gtk::ALIGN_START};
    Gtk::Label          sobstructlabelsct{_("Mak/SCT obstruction size/%:"), Gtk::ALIGN_START};
    Gtk::Label          etransortholabel{_("Orthoscopic transmission/%:"), Gtk::ALIGN_START};
    Gtk::Label          etransplossllabel{_("Plossl transmission/%:"), Gtk::ALIGN_START};
    Gtk::Label          etransmultilabel{_("Multi element transmission/%:"), Gtk::ALIGN_START};
    Gtk::Label          graphthemeslabel{_("Custom graph theme:"), Gtk::ALIGN_START};
    Gtk::Label          drawframeslabel{_("Show Frames:"), Gtk::ALIGN_START};
    Ui::SpinEntry       sreflect;
    Ui::SpinEntry       sobstruct;
    Ui::SpinEntry       sobstructsct;
    Ui::SpinEntry       etransortho;
    Ui::SpinEntry       etransplossl;
    Ui::SpinEntry       etransmulti;
    Ui::SpinEntry       strans;
    Gtk::Button         scopedefaults;
    Gtk::Button         epdefaults;
    Gtk::Button         appearancedefaults;
    Gtk::ComboBoxText   graphthemes;
    Glib::KeyFile       keyfile;
    Glib::ustring       path;
    Glib::RefPtr<Gtk::StyleContext> context;

    void create_appearance_page();
    void create_telescope_page();
    void create_eyepiece_page();
    void get_keyfile_settings();
    bool on_key_press_event(GdkEventKey* event) override;

protected:
    void save_key_settings();
};
