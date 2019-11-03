#pragma once
#include <memory>
#include <giomm/notification.h>
#include <gtkmm/application.h>
#include <gtkmm/frame.h>
#include <glibmm/keyfile.h>
#include <glibmm/miscutils.h>
#include <filesystem>

namespace AppGlobals
{
    inline bool IMODE = false;  // default interactive mode is off
    inline bool LOGFLAG = false; // defaul logging is off
    inline int  resultsset = 1;  // the first results column is the default

    // paths used throughout
    inline Glib::ustring datadir = "/usr/local/share/";  // default, but searched for later
    inline Glib::ustring userconfigdir = Glib::get_user_config_dir() + "/gnome-tcalc/";

    inline Glib::ustring dsolistfilename = datadir + "gnome-tcalc/skyobjects.TCalc"; //dsos
    inline Glib::ustring configpath = userconfigdir + "tcalc.conf";
    inline Glib::ustring telescopesfilename = datadir + "gnome-tcalc/telescopes.TCalc";
    inline Glib::ustring telescopesfilenameuser = userconfigdir + "telescopesuser.TCalc";
    inline Glib::ustring eyepiecesfilename = datadir + "gnome-tcalc/eyepieces.TCalc";  
    inline Glib::ustring eyepiecesfilenameuser = userconfigdir + "eyepiecesuser.TCalc";

    inline void get_keyfile_config(Gtk::Frame &frame)
    {
        // we can add more when needed, just for frame style for now

        if (false == std::filesystem::exists(AppGlobals::configpath.c_str())) return;

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
