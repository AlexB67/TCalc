#pragma once
#include <memory>
#include <giomm/notification.h>
#include <gtkmm/application.h>
#include <gtkmm/frame.h>
#include <glibmm/keyfile.h>
#include <glibmm/miscutils.h>

namespace AppGlobals
{
    inline bool IMODE = false;  // default interactive mode is off
    inline bool LOGFLAG = false; // defaul logging is off
    inline int  resultsset = 1;  // the first results column is the default

    // paths used throughout
    inline const Glib::ustring dsolistfilename = Glib::get_home_dir() + "/Documents/TCalc/skyobjects.TCalc"; //dsos
    inline const Glib::ustring configpath = Glib::get_home_dir() + "/Documents/TCalc/tcalc.conf";
    inline const Glib::ustring telescopesfilename = Glib::get_home_dir() + "/Documents/TCalc/telescopes.TCalc";
    inline const Glib::ustring telescopesfilenameuser = Glib::get_home_dir() + "/Documents/TCalc/telescopesuser.TCalc";
    inline const Glib::ustring eyepiecesfilename = Glib::get_home_dir() + "/Documents/TCalc/eyepieces.TCalc";  
    inline const Glib::ustring eyepiecesfilenameuser = Glib::get_home_dir() + "/Documents/TCalc/eyepiecesuser.TCalc";

    inline void get_keyfile_config(Gtk::Frame &frame)
    {
        // we can add more when needed, just for frame style for now
        Glib::KeyFile keyfile;
        keyfile.load_from_file(AppGlobals::configpath); 
        std::vector<bool> appearance = keyfile.get_boolean_list("Appearance", "settings");

        if (false == appearance[4])
            frame.set_shadow_type(Gtk::SHADOW_NONE);
    }

    inline void app_notify(const Glib::ustring &message, const Glib::RefPtr<Gtk::Application> &app,
                           const Glib::ustring messageid = _("tcalc-notify"))
    {
        if (!app)
            return;

        auto note = Gio::Notification::create(_("gnome-tcalc"));
        note->set_body(message);
        app->send_notification(messageid, note);
    }

    inline void change_frame_style(Gtk::Frame &frame)
    {
        (Gtk::SHADOW_ETCHED_IN == frame.get_shadow_type()) ? 
        frame.set_shadow_type(Gtk::SHADOW_NONE) : frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    }

    inline std::tuple<Glib::ustring, double, double, double, double,
                    double, double, Glib::ustring, int, int, double, Glib::ustring, 
                    Glib::ustring> epdata;

    inline std::tuple<Glib::ustring, double, double, double, double, int, 
                    Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                    double, double> scopedata;

    inline sigc::signal<void(const bool, const int, const Glib::ustring&)> log_msg;

    inline sigc::signal<void()> set_frame_style;

    inline sigc::signal<void()> update_ep_data;
    inline sigc::signal<void()> new_ep_data;
    inline sigc::signal<void()> del_ep_data;
    inline sigc::signal<void()> move_ep_row_up;
    inline sigc::signal<void()> move_ep_row_down;


    inline sigc::signal<void()> update_scope_data;
    inline sigc::signal<void()> new_scope_data;
    inline sigc::signal<void()> del_scope_data;
    inline sigc::signal<void()> move_scope_row_up;
    inline sigc::signal<void()> move_scope_row_down;

    inline sigc::signal<void(const Glib::ustring&)> update_graphthemes;

    
} // namespace AppGlobals
