#pragma once
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/aspectframe.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/cssprovider.h>
#include "eyepiecebox.hpp"
#include "telescopebox.hpp"
#include "loggerbox.hpp"
#include "optionsbox.hpp"
#include "starmagbox.hpp"
#include "oculardrawingarea.hpp"
#include "dsocombomodel.hpp"
#include "searchwindow.hpp"

class OcularWindow : public Gtk::Window
{
public:
    explicit OcularWindow();
    virtual ~OcularWindow();
    OcularWindow(const OcularWindow &) = delete;
    OcularWindow(OcularWindow &&) = delete;
    OcularWindow &operator=(const OcularWindow &other) = delete;
    OcularWindow &operator=(OcularWindow &&other) = delete;

private:
    Gtk::HeaderBar headerbar;
    Gtk::Button searchbutton;
    Gtk::Grid windowgrid;
    Gtk::Grid oculargrid;
    Gtk::Grid controlsgrid;
    Gtk::Grid controlsgrid2;
    Gtk::Grid dsoobjgrid;
    Gtk::Grid ocularinfogrid;
    Gtk::Grid ocularboxgrid;
    Gtk::Label dsocontrastlabel;
    Gtk::Label ocularfovlabel;
    Gtk::Label ocularexitpupillabel;
    Gtk::Label ocularmaglabel;
    Gtk::Label dsoframelabel;
    Gtk::Label ocularboxframelabel;
    Gtk::Label obscontrastlabel;
    Gtk::Entry ocularfov;
    Gtk::Entry ocularexitpupil;
    Gtk::Entry ocularmag;
    Gtk::Entry obscontrast;
    Ui::SpinEntry dsocontrast;
    Gtk::Switch *nightmode;
    Gtk::Frame dsoframe;
    Gtk::AspectFrame ocularframe;
    Gtk::Frame ocularinfoframe;
    Gtk::Frame ocularboxframe;
    std::shared_ptr<EpBox::Eyepiecebox> epbox;
    std::shared_ptr<ScopeBox::Telescopebox> scopebox;
    std::unique_ptr<OptionsBox::Optionsbox> optionsbox;
    std::unique_ptr<MagBox::Magbox> magbox;
    std::unique_ptr<SearchWindow> searchwindow;
    Glib::RefPtr<Gtk::CssProvider> provider;
    Glib::RefPtr<Gtk::StyleContext> context;
    bool preferdarktheme;
    Ocular ocularbox;
    void create_sky_objects_box();
    void create_ocular_info_box();
    bool on_key_press_event(GdkEventKey *event) override;
    void set_signal_handlers();
    void set_ocular_info();
    void dso_changed();
    void set_contrast_info();
    void ocular_changed();
    void search();
};
