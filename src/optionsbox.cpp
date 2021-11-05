#include "optionsbox.hpp"
#include "appglobals.hpp"

OptionsBox::Optionsbox::Optionsbox()
{
    m_frame.set_label_widget(m_framelabel);
    m_framelabel.set_markup(_("<b>Options</b>"));
    m_frame.set_label_align(Gtk::Align::CENTER);
    m_frame.set_vexpand(true);

    m_usefstop = Gtk::make_managed<Gtk::Switch>();
    m_uselinearmethod = Gtk::make_managed<Gtk::Switch>();

    Uidefs::set_ui_spacing(m_grid);

    m_grid.set_column_homogeneous(true);
    m_uselinearmethod->set_halign(Gtk::Align::END);
    m_uselinearmethod->set_valign(Gtk::Align::CENTER);
    m_usefstop->set_halign(Gtk::Align::END);
    m_usefstop->set_valign(Gtk::Align::CENTER);
    m_usefstop->set_tooltip_text(_("Use field stop data to calculate the field of view."));
    m_uselinearmethod->set_tooltip_markup(_("When enabled, the commonly used linear approximation is used to calculate the field of view. "
                                            "The default uses the more accurate formula: "
                                            "2 tan<sup> -1</sup> ( tan ( <i>θ</i><sub> a</sub> / 2 ) <i>f</i><sub> e</sub> / <i>f</i><sub> s</sub> )"));
    m_wavelength.set_tooltip_text(_("The wavelength in nm. used in various calculations (such as the airy disk)."));
}

Gtk::Frame &OptionsBox::Optionsbox::create_options_grid()
{
    m_grid.attach(m_usefstoplabel, 0, 0);
    m_grid.attach(*m_usefstop, 1, 0);
    m_grid.attach(m_uselinearmethodlabel, 0, 1);
    m_grid.attach(*m_uselinearmethod, 1, 1);
    m_grid.attach(m_wavelengthlabel, 0, 2);
    m_grid.attach(m_wavelength, 1, 2);

    set_default_values();
    m_frame.set_child(m_grid);

    AppGlobals::get_keyfile_config(m_frame);
    //AppGlobals::frame_style.connect([this](){ AppGlobals::set_frame_style(m_frame);});

    return m_frame;
}

void OptionsBox::Optionsbox::set_default_values()
{
    m_usefstop->set_active(false);
    m_uselinearmethod->set_active(false);
    m_wavelength.set_spin_entry(550.0, 400.0, 700.0, 1.0, 2, true);
}

void OptionsBox::Optionsbox::show_wavelength(const bool show)
{
    if (true == show)
    {
        m_wavelengthlabel.show();
        m_wavelength.show();
    }
    else
    {
        m_wavelengthlabel.hide();
        m_wavelength.hide();
    }
}
