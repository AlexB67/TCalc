#include "telescopebox.hpp"
#include "fileio.hpp"
#include "appglobals.hpp"

ScopeBox::Telescopebox::Telescopebox(bool with_entry, bool userdataonly)
: m_with_entry(with_entry),
  m_userdataonly(userdataonly)
{
	if (!m_with_entry)
	{
		m_smodel = Gtk::make_managed<Gtk::ComboBox>(); // only if using the combo
		m_smodel->set_tooltip_text(_("Select a telescope from the list of preset models, "\
								     " or use the values below to customise settings."));
	}
	else
		m_smodelentry.set_tooltip_text(_("Search for a telescope "\
								         "or use the values below to customise settings."));
										 
	m_frame.set_hexpand(false);
	m_frame.set_vexpand(false);
    m_frame.set_label_widget(m_framelabel);
	m_framelabel.set_markup(_("<b>Telescope properties</b>"));
    m_frame.set_label_align( Gtk::Align::CENTER);
   
    Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);
    
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

	if (m_with_entry)
		m_grid.attach(m_smodelentry, 0, 1, 2, 1);
	else
		m_grid.attach(*m_smodel, 0, 1, 2, 1);

	//m_grid.attach(*m_smodel, 0, 2, 2, 1);
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
	db.load_scope_data(m_scombomodel, m_userdataonly);
	set_default_values();
	
	if (m_with_entry)
		create_scope_entry_model();
	else
		create_scope_combo_model();

	create_scopemodel_connection();
						
	m_sflen.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::update_sfratio));
	m_saperture.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::update_sfratio));
	m_sfratio.signal_value_changed().connect( sigc::mem_fun(*this, &Telescopebox::set_sflen));
	m_stype.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::scope_type_changed));

	if(m_with_entry)
	{
		con = std::vector<sigc::connection>(6);
		con[0] = m_stype.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
		con[1] = m_saperture.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
		con[2] = m_sflen.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
		con[3] = m_sobstruct.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
		con[4] = m_sreflect.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
		con[5] = m_sfratio.signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::set_custom_scope));
	}

	m_frame.set_child(m_grid);
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

void ScopeBox::Telescopebox::create_scope_entry_model()
{
	m_smodelentry.set_max_width_chars(28);
	m_smodelentry.set_icon_from_icon_name("edit-find-symbolic", Gtk::Entry::IconPosition::PRIMARY);
	m_smodelentry.set_icon_activatable(false, Gtk::Entry::IconPosition::PRIMARY);
	m_smodelentry.set_icon_from_icon_name("edit-clear-symbolic", Gtk::Entry::IconPosition::SECONDARY);
	m_smodelentry.signal_icon_press().connect( sigc::mem_fun(*this, &Telescopebox::clear_smodel_entry));
	scopeentrycompletion = Gtk::EntryCompletion::create();

	auto& scopelistcompletion_model = m_scombomodel.get_scope_list_model();
    scopeentrycompletion->set_model(scopelistcompletion_model);
    scopeentrycompletion->set_text_column(m_scombomodel.m_scopecompletioncols.m_smodel);
    scopeentrycompletion->set_minimum_key_length(3);
    scopeentrycompletion->set_popup_completion(true);
    m_smodelentry.set_completion(scopeentrycompletion);
    scopeentrycompletion->signal_match_selected().connect(sigc::mem_fun(*this, &Telescopebox::on_scope_selected), false);
    scopeentrycompletion->set_match_func(sigc::mem_fun(*this, &Telescopebox::on_scope_completion_match));
	reset_smodel_entry();
}

void ScopeBox::Telescopebox::reset_smodel_entry()
{
	const auto& scopetreemodel = m_scombomodel.get_scopemodel(); // set the first scope we find

	for (auto& connection : con) connection.block();

	if (scopetreemodel->children().size() > 0)
    {
        Gtk::TreeIter it = scopetreemodel->children().begin();
        if(it->children().size() > 0)
        { 
            auto it2 = it->children().begin();
            m_smodelentry.set_text((*it2)[m_scombomodel.m_scopecols.m_smodel]);
			current_row = *it2;
			current_iter = it2;
			set_values_from_model(current_row);
        }
    }

	for (auto& connection : con) connection.unblock();	
}

void ScopeBox::Telescopebox::create_scope_combo_model()
{
	m_smodel->signal_changed().connect(sigc::mem_fun(*this, &Telescopebox::scope_changed));

	const auto& scopetreemodel = m_scombomodel.get_scopemodel();

    m_smodel->set_model(scopetreemodel);
    m_smodel->set_id_column(1);
    m_cell.property_ellipsize() =  Pango::EllipsizeMode::END;
    m_cell.property_max_width_chars() = 38;
    m_smodel->set_button_sensitivity(Gtk::SensitivityType::ON);
    m_smodel->pack_start(m_cell);
    m_smodel->set_popup_fixed_width(false);
    m_smodel->set_active(0);
    
    // set the first eyepiece we find;

    m_smodel->set_cell_data_func(m_cell, sigc::mem_fun(*this, &Telescopebox::on_cell_data_changed));

    if (scopetreemodel->children().size() > 0)
    {

        Gtk::TreeIter it = scopetreemodel->children().begin();
        if(it->children().size() > 0)
        { 
            auto it2 = it->children().begin();
			current_row = *it2;
			current_iter = it2;
            m_smodel->set_active(it2);
        }
    }
}

