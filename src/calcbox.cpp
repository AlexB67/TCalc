#include "calcbox.hpp"
#include "gtkmmcustomutils.hpp"
#include "appglobals.hpp"

using AppGlobals::log_msg;

CalcBox::Calcbox::Calcbox()
{
    m_calcframe.set_vexpand(false);
    m_calcframelabel.set_markup(_("<b>Calculate</b>"));
    m_calcframe.set_label_widget(m_calcframelabel);
    m_calcframe.set_label_align( Gtk::ALIGN_CENTER,  Gtk::ALIGN_CENTER);

    m_interactive = Gtk::make_managed<Gtk::Switch>();
    m_logswitch = Gtk::make_managed<Gtk::Switch>();
  
    m_calcbutton.set_tooltip_text(_("Calculate results based on current values."));
    m_clearbutton.set_tooltip_text(_("Clear all the output fields."));
    m_interactive->set_tooltip_text(_("Compute results immediately when any input value changes."));
    m_logswitch->set_active(true);
    m_logswitch->set_tooltip_text(_("Enable or disable logging."));
    m_interactive->set_active(false);
    
    m_loglabel.set_halign(Gtk::ALIGN_CENTER);
    m_loglabel.set_valign(Gtk::ALIGN_CENTER);
    m_logswitch->set_halign(Gtk::ALIGN_CENTER);
    m_logswitch->set_valign(Gtk::ALIGN_CENTER);
    m_interactive->set_halign(Gtk::ALIGN_CENTER);
    m_interactive->set_valign(Gtk::ALIGN_CENTER);
    
    m_calcbutton.set_hexpand(true);
    m_clearbutton.set_hexpand(true);
    m_calcbutton.set_vexpand(false);
    m_clearbutton.set_vexpand(false);
    
    Uidefs::set_ui_spacing(m_calcgrid);

    m_calcgrid.set_hexpand(false);
    m_calcgrid.set_vexpand(false);
    m_separator.set_hexpand(false);
    m_separator2.set_hexpand(false);
    m_separator3.set_hexpand(false);
}

Gtk::Frame &CalcBox::Calcbox::create_calc_grid()
{
    
    m_sizegroup =  Gtk::SizeGroup::create(Gtk::SIZE_GROUP_HORIZONTAL);
    m_sizegroup->add_widget(m_calcbutton);
    m_sizegroup->add_widget(m_clearbutton);

    m_calcgrid.attach(m_calcbutton, 0, 0);
    m_calcgrid.attach(m_separator, 1, 0);
    m_calcgrid.attach(m_clearbutton, 2, 0);
    m_calcgrid.attach(m_separator2, 3, 0);
    m_calcgrid.attach(m_loglabel, 4, 0);
    m_calcgrid.attach(*m_logswitch, 5, 0);
    m_calcgrid.attach(m_separator3, 6, 0);
    m_calcgrid.attach(m_interactivelabel, 7, 0);
    m_calcgrid.attach(*m_interactive, 8, 0);
    
    m_calcframe.add(m_calcgrid);
    
    m_interactive->property_active().signal_changed().connect([this]()
    {
        m_calcbutton.set_sensitive(!m_interactive->get_active());
        AppGlobals::IMODE = m_interactive->get_active();
         (true == m_interactive->get_active()) ? log_msg.emit(true, LogView::tINFO, _("Interactive mode enabled")) :
         log_msg.emit(true, LogView::tINFO, _("Interactive mode disabled"));
    });
    
    m_logswitch->property_active().signal_changed().connect([this]()
    {
        if (m_logswitch->get_active()) log_msg.emit(true, LogView::tINFO, _("Logging enabled"));
        else log_msg.emit(true, LogView::tINFO, _("Logging disabled"));
        AppGlobals::LOGFLAG = m_logswitch->get_active();
    });
    
    AppGlobals::get_keyfile_config(m_calcframe);
    AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(m_calcframe);});
    return m_calcframe;
}
