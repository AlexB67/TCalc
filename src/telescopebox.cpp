#include "telescopebox.hpp"
#include "fileio.hpp"
#include "appglobals.hpp"

ScopeBox::Telescopebox::Telescopebox(const bool userdataonly) : m_userdataonly(userdataonly)
{
	m_frame.set_hexpand(false);
	m_frame.set_vexpand(false);
    m_frame.set_label_widget(m_framelabel);
	m_framelabel.set_markup(_("<b>Telescope properties</b>"));
    m_frame.set_label_align( Gtk::ALIGN_CENTER,  Gtk::ALIGN_CENTER);
   
    Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);
    
	m_smodel.set_tooltip_text(_("Select a telescope from the list of preset models," 
								" or use the values below to customise settings."));
	m_sflen.set_tooltip_text(_("The focal length of the eyepiece in mm."));
	m_sfratio.set_tooltip_markup(_("The speed of the telescope. Expressed as <i>f</i>/# ." 
								   "The focal length devided by the aperture."));
	m_saperture.set_tooltip_text(_("The aperture of the telescope in mm."));
	m_sreflect.set_tooltip_text(_("The Reflectifity of a reflector, or transmission for a refractor " 
	                              "as a percentage. A combination of both for Maksutov/SCT types."));
	m_stype.set_tooltip_text(_("The type of telescope. Reflector, refractor or SCT/Mak."));
	m_sobstruct.set_tooltip_text(_("The obtruction size as a percentage of the aperture diameter. " 
								   "Reflectors and SCT/Maksutov types only."));
	m_stype.set_vexpand(false);
	m_grid.set_hexpand(false);
	m_grid.set_vexpand(false);
}

Gtk::Frame &ScopeBox::Telescopebox::create_telescope_grid()
{
	m_grid.attach(m_smodellabel, 0, 0, 2, 1);
	m_grid.attach(m_smodel, 0, 2, 2, 1);
	m_grid.attach(m_sflenlabel, 0, 3, 1, 1);
	m_grid.attach(m_sflen, 1, 3, 1, 1);
	m_grid.attach(m_saperturelabel, 0, 4, 1, 1);
	m_grid.attach(m_saperture, 1, 4, 1, 1);
	m_grid.attach(m_sfratiolabel, 0, 5, 1, 1);
	m_grid.attach(m_sfratio, 1, 5, 1, 1);
	m_grid.attach(m_sobstructlabel, 0, 6, 1, 1);
	m_grid.attach(m_sobstruct, 1, 6, 1, 1);
	m_grid.attach(m_sreflectlabel, 0, 7, 1, 1);
	m_grid.attach(m_sreflect, 1, 7, 1, 1);
	m_grid.attach(m_stypelabel, 0, 8, 1, 1);
	m_grid.attach(m_stype, 1, 8, 1, 1);

	m_stype.insert(0, _("Reflector"));
	m_stype.insert(1, _("Refractor"));
	m_stype.insert(2, _("SCT/Mak"));
	
	
	fileIO::dbfileIO db;
	db.load_scope_data(m_smodel, m_scombomodel, m_userdataonly);

	set_default_values();

	create_scopemodel_connection();

	AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(m_frame);});
	AppGlobals::get_keyfile_config(m_frame);
						
	m_sflen.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::update_sfratio));
	m_saperture.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::update_sfratio));
	m_sfratio.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::set_sflen));
	m_smodel.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::scope_changed));
	m_stype.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::scope_type_changed));	

	m_frame.add(m_grid);
	return m_frame;
	
}
void ScopeBox::Telescopebox::set_default_values()
{   
    m_sflen.set_spin_entry(1200.0, 10.0, 4000.0, 1.0, 2, true);
    m_saperture.set_spin_entry(200.0, 10.0, 1000.0, 0.5, 2, true);
    m_sfratio.set_spin_entry(1200.0 / 200.0, 0.0, 100.0, 0.01, 2, true);
	m_sreflect.set_spin_entry(90.0, 0.0, 100.0, 0.1, 2, true);
	m_sobstruct.set_spin_entry(23.15, 0.0, 50.0, 0.1, 2, true);
	m_sobstruct.set_spin_entry(23.15, 0.0, 50.0, 0.1, 2, true);
	m_stype.set_active(0);
}

