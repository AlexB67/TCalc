#include <glibmm/keyfile.h>
#include "eyepiecebox.hpp"
#include "fileio.hpp"
#include "appglobals.hpp"

EpBox::Eyepiecebox::Eyepiecebox(const bool userdataonly) : m_userdataonly(userdataonly)
{
	m_frame.set_hexpand(false);
	m_frame.set_vexpand(false);
    m_framelabel.set_markup(_("<b>Eyepiece properties</b>"));
    m_frame.set_label_widget(m_framelabel);
    m_frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    
    Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);
	m_emodel.set_tooltip_text(_("Select an eyepiece from the list of preset models, " 
								"or use the values below to customise settings."));
	m_eflen.set_tooltip_text(_("The focal length of the eyepiece in mm."));
	m_efstop.set_tooltip_text(_("The field stop diameter of the eyepiece in mm. set to 0 if unknown."));
	m_erelief.set_tooltip_text(_("The eye relief of the eyepiece in mm. set to 0 if unknown."));
	m_etrans.set_tooltip_text(_("The transmission of the eyepiece as a percentage."));
	m_efov.set_tooltip_text(_("The field of view of the eyepiece in degrees."));
	m_etype.set_tooltip_text(_("The optical design of the eyepiece. Plossl, ortho, or other types (specified as multi other)."));

	m_etype.set_vexpand(false);
    
	m_grid.set_hexpand(false);
	m_grid.set_vexpand(false);
}

Gtk::Frame &EpBox::Eyepiecebox::create_eyepiece_grid()
{
	m_grid.attach(m_emodellabel, 0, 0, 2, 1);
	m_grid.attach(m_emodel, 0, 1, 2, 1);
	m_grid.attach(m_efovlabel, 0, 2, 1, 1);
	m_grid.attach(m_efov, 1, 2, 1, 1);
	m_grid.attach(m_eflenlabel, 0, 3, 1, 1);
	m_grid.attach(m_eflen, 1, 3, 1, 1);
	m_grid.attach(m_efstoplabel, 0, 4, 1, 1);
	m_grid.attach(m_efstop, 1, 4, 1, 1);
	m_grid.attach(m_erelieflabel, 0, 5, 1, 1);
	m_grid.attach(m_erelief, 1, 5, 1, 1);
	m_grid.attach(m_etranslabel, 0, 6, 1, 1);
	m_grid.attach(m_etrans, 1, 6, 1, 1);
	m_grid.attach(m_etypelabel, 0, 7, 1, 1);
	m_grid.attach(m_etype, 1, 7, 1, 1);

	m_etype.insert(0, _("Plossl"));
	m_etype.insert(1, _("Ortho"));
	m_etype.insert(2, _("Multi other"));
	
	fileIO::dbfileIO db;
	db.load_ep_data(m_emodel, m_ecombomodel, m_userdataonly);		

	set_default_values();

	m_emodel.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::ep_changed));

	create_epmodel_connection();

	AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(m_frame);});
    AppGlobals::get_keyfile_config(m_frame);
	m_frame.add(m_grid);

	return m_frame;
}

void EpBox::Eyepiecebox::set_default_values()
{
    m_efstop.set_sensitive(false);
    m_eflen.set_spin_entry(11.0, 2.0, 100.0, 0.1, 2, true);
    m_efov.set_spin_entry(50.0, 10.0, 120.0, 0.1, 2, true);
    m_efstop.set_spin_entry(9.1, 0.0, 100.0, 0.1, 2, true);
	m_erelief.set_spin_entry(8.0, 0.0, 100.0, 0.1, 2, true);
    m_etrans.set_spin_entry(98.02, 0.0, 100.0, 0.5, 2, true);
}
