#include "appglobals.hpp"
#include "prefswindow.hpp"
#include "gtkmmcustomutils.hpp"
#include "astrocalclib/astrocalc.hpp"
#include <glibmm/keyfile.h>
#include <glibmm/i18n.h>
#include <gtkmm/settings.h>
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm.h>
#include <iostream>
#include <filesystem>

using AppGlobals::log_msg;

PrefsWindow::PrefsWindow()
{
    set_title(_("Preferences"));
    set_hide_on_close(true);
    stack.set_transition_type(Gtk::StackTransitionType::SLIDE_DOWN);
    stack.set_transition_duration(500);

    sidebar.set_hexpand(true);
    stack.set_vexpand(true);
    stack.set_hexpand(true);
    sidebar.set_stack(stack);

    grid.attach(sidebar, 0, 0);
    grid.attach(stack, 1, 0);

    create_appearance_page();
    create_telescope_page();
    create_eyepiece_page();

    stack.add(appearancegrid, _("Appearance"), _("Appearance"));
    stack.add(scopegrid, _("Telescope optical defaults"), _("Telescope optical defaults"));
    stack.add(epgrid, _("Eyepiece optical defaults"), _("Eyepiece optical defaults"));

    set_child(grid);

    get_keyfile_settings();

    if ("cinnamon" == Glib::getenv("XDG_SESSION_DESKTOP"))
    {
        preferdarktheme->set_active(false);
        preferdarktheme->set_sensitive(false);
        preferdarktheme->set_tooltip_text(_("Cinnamon does not use this feature."));
        preferdarkthemelabel.set_tooltip_text(preferdarktheme->get_tooltip_text());
    }

    auto controller = Gtk::EventControllerKey::create();
    controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    controller->signal_key_pressed().connect(
    sigc::bind(sigc::mem_fun(*this, &PrefsWindow::on_key_press_event), "capture"), false);
    add_controller(controller);

    signal_hide().connect(sigc::mem_fun(*this, &PrefsWindow::save_key_settings));
}

void PrefsWindow::create_appearance_page()
{
    showtime = Gtk::make_managed<Gtk::Switch>();
    showtime->set_halign(Gtk::Align::END);
    showtime->set_valign(Gtk::Align::CENTER);
   
    showcolour = Gtk::make_managed<Gtk::Switch>();
    showcolour->set_halign(Gtk::Align::END);
    showcolour->set_valign(Gtk::Align::CENTER);

    preferdarktheme = Gtk::make_managed<Gtk::Switch>();
    preferdarktheme->set_halign(Gtk::Align::END);
    preferdarktheme->set_valign(Gtk::Align::CENTER);
   
    usemonospace = Gtk::make_managed<Gtk::Switch>();
    usemonospace->set_halign(Gtk::Align::END);
    usemonospace->set_valign(Gtk::Align::CENTER);

    drawframes = Gtk::make_managed<Gtk::Switch>();
    drawframes->set_halign(Gtk::Align::END);
    drawframes->set_valign(Gtk::Align::CENTER);
   
    appearancedefaults.set_halign(Gtk::Align::END);
    appearancedefaults.set_label(_("Defaults"));
    appearancedefaults.set_tooltip_text(_("Restore factory settings."));

    preferdarkthemelabel.set_tooltip_text(_("Whether to use a dark variant of the current desktop theme by default (if available)."));
    preferdarktheme->set_tooltip_text(preferdarkthemelabel.get_tooltip_text());
    showtimelabel.set_tooltip_text(_("Information log: Displays a time stamp in front of log messages."));
    showtime->set_tooltip_text(showtimelabel.get_tooltip_text());
    showcolourlabel.set_tooltip_text(_("Information log: Highlights warning messages in red."));
    showcolour->set_tooltip_text(showcolourlabel.get_tooltip_text());
    usemonospacelabel.set_tooltip_text(_("Information log: Enables monospace font for log messages."));
    usemonospace->set_tooltip_text(usemonospacelabel.get_tooltip_text());
    graphthemes.set_tooltip_text(_("Graphswindow: Changes the appearance of graphs."));
    graphthemeslabel.set_tooltip_text(graphthemes.get_tooltip_text());
    drawframes->set_tooltip_text(_("General appearance: Determines whether frames are visible."));
    drawframeslabel.set_tooltip_text(drawframes->get_tooltip_text());

    // defaults to be overriden by key settings, first time user without a keyfile will have the default values below
    preferdarktheme->set_active(true);
    showtime->set_active(true);
    showcolour->set_active(false);
    usemonospace->set_active(false);
    drawframes->set_active(true);

    Uidefs::set_ui_spacing<Gtk::Grid>(appearancegrid);

    graphthemes.insert(0, _("Fade to black"));
    graphthemes.insert(1, _("Black"));
    graphthemes.insert(2, _("Herculean blue"));
    graphthemes.insert(3, _("Midnight blue"));
    graphthemes.insert(4, _("Dark"));
    graphthemes.insert(5, _("Desktop theme"));

    graphthemes.set_active(1);
    graphthemes.set_tooltip_text(_("Customise the graph colours. This will reset if the" 
    "desktop theme changes, or if prefer darkmode is swithed on or off."));

    appearancegrid.attach(preferdarkthemelabel, 0, 0);
    appearancegrid.attach(*preferdarktheme, 1, 0);
    appearancegrid.attach(showtimelabel, 0, 1);
    appearancegrid.attach(*showtime, 1, 1);
    appearancegrid.attach(showcolourlabel, 0, 2);
    appearancegrid.attach(*showcolour, 1, 2);
    appearancegrid.attach(usemonospacelabel, 0, 3);
    appearancegrid.attach(*usemonospace, 1, 3);
    appearancegrid.attach(drawframeslabel, 0, 4);
    appearancegrid.attach(*drawframes, 1, 4);
    appearancegrid.attach(graphthemeslabel, 0, 5);
    appearancegrid.attach(graphthemes, 1, 5);
    appearancegrid.attach(appearancedefaults, 1, 6);

    graphthemes.signal_changed().connect([this](){

        std::vector<Glib::ustring> themes
        {
             "Fade to black", "Black", "Herculean blue", "Midnight blue",
             "Dark", "Default"
        };
    
        AppGlobals::update_graphthemes.emit(themes[graphthemes.get_active_row_number()]);
    });

    preferdarktheme->property_active().signal_changed().connect([this]() { 
        Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().set_value(preferdarktheme->get_active());
    });

    drawframes->property_active().signal_changed().connect([this](){
            (true == drawframes->get_active())  ? AppGlobals::frame_style.emit(1.0)
                                                : AppGlobals::frame_style.emit(0.0);

	});



    appearancedefaults.signal_clicked().connect([this]() {
        showtime->set_active(true);
        usemonospace->set_active(false);
        preferdarktheme->set_active(true);
        showcolour->set_active(false);
        graphthemes.set_active(1); // black graph theme
        drawframes->set_active(true);
    });
}

void PrefsWindow::create_telescope_page()
{
    sobstruct.set_max_width_chars(6); // gtk4 behaving strange .. again! but this solves it, was fine in gtk3
    sobstruct.set_halign(Gtk::Align::FILL);
    sobstructsct.set_halign(Gtk::Align::FILL);
    strans.set_halign(Gtk::Align::FILL);
    sreflect.set_halign(Gtk::Align::FILL);
    scopedefaults.set_halign(Gtk::Align::FILL);
    scopedefaults.set_label(_("Defaults"));
    scopedefaults.set_tooltip_text(_("Restore factory settings."));

    sobstruct.set_spin_entry(20.0, 0.0, 100.0, 0.1, 2, true);
    sobstructsct.set_spin_entry(35.0, 0.0, 100.0, 0.1, 2, true);
    sreflect.set_spin_entry(88.0, 0.0, 100.0, 0.1, 2, true);
    strans.set_spin_entry(99.0, 0.0, 100.0, 0.1, 2, true);

    Uidefs::set_ui_spacing(scopegrid);
    scopegrid.attach(sreflectlabel, 0, 0);
    scopegrid.attach(sreflect, 1, 0);
    scopegrid.attach(stranslabel, 0, 1);
    scopegrid.attach(strans, 1, 1);
    scopegrid.attach(sobstructlabel, 0, 2);
    scopegrid.attach(sobstruct, 1, 2);
    scopegrid.attach(sobstructlabelsct, 0, 3);
    scopegrid.attach(sobstructsct, 1, 3);
    scopegrid.attach(scopedefaults, 1, 4);

    scopedefaults.signal_clicked().connect([this]() {
        sreflect.set_value(Astrocalc::astrocalc::SCOPEREFLECTDEFAULT);
        strans.set_value(Astrocalc::astrocalc::SCOPETRANSDEFAULT);
        sobstruct.set_value(Astrocalc::astrocalc::OBSTRUCTSIZEDEFAULT);
        sobstructsct.set_value(Astrocalc::astrocalc::OBSTRUCTSIZESCTDEFAULT);
    });
}

void PrefsWindow::create_eyepiece_page()
{
    Uidefs::set_ui_spacing(epgrid);

    etransmulti.set_max_width_chars(6); // gtk4 behaving strange .. again! but this solves it, was fine in gtk3
    etransmulti.set_halign(Gtk::Align::FILL);
    etransplossl.set_halign(Gtk::Align::FILL);
    etransortho.set_halign(Gtk::Align::FILL);
    epdefaults.set_halign(Gtk::Align::FILL);
    epdefaults.set_label(_("Defaults"));
    epdefaults.set_tooltip_text(_("Restore factory settings."));

    etransmulti.set_spin_entry(96.0, 0.0, 100.0, 0.1, 2, true);
    etransortho.set_spin_entry(98.0, 0.0, 100.0, 0.1, 2, true);
    etransplossl.set_spin_entry(98.0, 0.0, 100.0, 0.1, 2, true);

    epgrid.attach(etransmultilabel, 0, 0);
    epgrid.attach(etransmulti, 1, 0);
    epgrid.attach(etransplossllabel, 0, 1);
    epgrid.attach(etransplossl, 1, 1);
    epgrid.attach(etransortholabel, 0, 2);
    epgrid.attach(etransortho, 1, 2);
    epgrid.attach(epdefaults, 1, 4);

    epdefaults.signal_clicked().connect([this]() {
        etransmulti.set_value(Astrocalc::astrocalc::ETRANSDEFAULT);
        etransplossl.set_value(Astrocalc::astrocalc::ETRANSPLOSSLDEFAULT);
        etransortho.set_value(Astrocalc::astrocalc::ETRANSORTHODEFAULT);
    });
}

void PrefsWindow::get_keyfile_settings()
{
    if (false == std::filesystem::exists(AppGlobals::configpath.c_str()))
        save_key_settings();

    Glib::RefPtr<Glib::KeyFile> keyfile = Glib::KeyFile::create();
    keyfile->load_from_file(AppGlobals::configpath);

    std::vector<bool> appearance = keyfile->get_boolean_list("Appearance", "settings");
    preferdarktheme->set_active(appearance[0]);
    showtime->set_active(appearance[1]);
    showcolour->set_active(appearance[2]);
    usemonospace->set_active(appearance[3]);
    drawframes->set_active(appearance[4]);
    graphthemes.set_active(keyfile->get_integer("Appearance", "graphtheme"));

    std::vector<double> sdefaults = keyfile->get_double_list("Telescope optical defaults", "telescopes");
    sreflect.set_value(sdefaults[0]);
    strans.set_value(sdefaults[1]);
    sobstruct.set_value(sdefaults[2]);
    sobstructsct.set_value(sdefaults[3]);

    std::vector<double> edefaults = keyfile->get_double_list("Eyepiece optical defaults", "eyepieces");
    etransmulti.set_value(edefaults[0]);
    etransplossl.set_value(edefaults[1]);
    etransortho.set_value(edefaults[2]);

    edefaults.clear();
}

void PrefsWindow::save_key_settings()
{
    std::vector<bool> appearance = {preferdarktheme->get_active(), showtime->get_active(),
                                    showcolour->get_active(), usemonospace->get_active(), drawframes->get_active()};
    std::vector<double> sdefaults = {sreflect.get_value(), strans.get_value(), sobstruct.get_value(), sobstructsct.get_value()};
    std::vector<double> edefaults = {etransmulti.get_value(), etransplossl.get_value(), etransortho.get_value()};

    Glib::RefPtr<Glib::KeyFile> keyfile = Glib::KeyFile::create();
    keyfile->set_boolean_list("Appearance", "settings", appearance);
    keyfile->set_integer("Appearance", "graphtheme", graphthemes.get_active_row_number());
    keyfile->set_double_list("Telescope optical defaults", "telescopes", sdefaults);
    keyfile->set_double_list("Eyepiece optical defaults", "eyepieces", edefaults);

    Astrocalc::astrocalc::ETRANSPLOSSL = etransplossl.get_value();
    Astrocalc::astrocalc::ETRANSORTHO = etransortho.get_value();
    Astrocalc::astrocalc::ETRANS = etransmulti.get_value();
    Astrocalc::astrocalc::OBSTRUCTSIZE = sobstruct.get_value();
    Astrocalc::astrocalc::OBSTRUCTSIZESCT = sobstructsct.get_value();
    Astrocalc::astrocalc::SCOPEREFLECT = sreflect.get_value();
    Astrocalc::astrocalc::SCOPETRANS = strans.get_value();

    try
    {
        keyfile->save_to_file(AppGlobals::configpath);
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "Error saving key file: " << ex.what() << '\n';
        return;
    }
}

bool PrefsWindow::on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring&)
{
    if (keyval == GDK_KEY_Escape) hide();
    
    return false;
}

