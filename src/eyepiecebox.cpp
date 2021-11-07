#include <glibmm/keyfile.h>
#include "eyepiecebox.hpp"
#include "fileio.hpp"
#include "appglobals.hpp"

EpBox::Eyepiecebox::Eyepiecebox(const bool with_entry, const bool userdataonly)
: m_with_entry(with_entry), 
  m_userdataonly(userdataonly)
{
	if (!m_with_entry)
	{
		m_emodel = Gtk::make_managed<Gtk::ComboBox>(); // only if using the combo
		m_emodel->set_tooltip_text(_("Select an eyepiece from the list of preset models, "\
								     " or use the values below to customise settings."));
	}
	else
		m_emodelentry.set_tooltip_text(_("Search for an eyepiece (from a list of more than a 1000 presets) "\
								        "or use the values below to customise settings."));

	m_frame.set_hexpand(false);
	m_frame.set_vexpand(false);
    m_framelabel.set_markup(_("<b>Eyepiece properties</b>"));
    m_frame.set_label_widget(m_framelabel);
    m_frame.set_label_align(Gtk::Align::CENTER);
    
    Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);
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

	if (m_with_entry)
		m_grid.attach(m_emodelentry, 0, 1, 2, 1);
	else
		m_grid.attach(*m_emodel, 0, 1, 2, 1);

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
	db.load_ep_data(m_ecombomodel, m_userdataonly);		
	set_default_values();
	
	if (m_with_entry)
		create_ep_entry_model();
	else
		create_ep_combo_model();
	
	create_epmodel_connection();
	m_etype.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::ep_type_changed));

	if(m_with_entry)
	{
		con = std::vector<sigc::connection>(6);
		con[0] = m_etype.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
		con[1] = m_efov.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
		con[2] = m_eflen.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
		con[3] = m_efstop.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
		con[4] = m_etrans.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
		con[5] = m_erelief.signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::set_custom_ep));
	}

	m_frame.set_child(m_grid);
	
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
	m_etype.set_active(0);
}

void EpBox::Eyepiecebox::create_ep_entry_model()
{
	m_emodelentry.set_max_width_chars(30);
	m_emodelentry.set_icon_from_icon_name("edit-find-symbolic");	
	epentrycompletion = Gtk::EntryCompletion::create();

	auto eplistcompletion_model = m_ecombomodel.get_ep_list_model();
    epentrycompletion->set_model(eplistcompletion_model);
    epentrycompletion->set_text_column(m_ecombomodel.m_epcompletioncols.m_epmodel);
    epentrycompletion->set_minimum_key_length(3);
    epentrycompletion->set_popup_completion(true);
    m_emodelentry.set_completion(epentrycompletion);
    epentrycompletion->signal_match_selected().connect(sigc::mem_fun(*this, &Eyepiecebox::on_ep_selected), false);
    epentrycompletion->set_match_func(sigc::mem_fun(*this, &Eyepiecebox::on_ep_completion_match));
	reset_emodel_entry();
}

void EpBox::Eyepiecebox::reset_emodel_entry()
{
	const auto& eptreemodel = m_ecombomodel.get_epmodel(); // set the first eyepiece we find
	
	for (auto& connection : con) connection.block();

	if (eptreemodel->children().size() > 0)
    {
        Gtk::TreeIter it = eptreemodel->children().begin();
        if(it->children().size() > 0)
        { 
            auto it2 = it->children().begin();
            m_emodelentry.set_text((*it2)[m_ecombomodel.m_epcols.m_epmodel]);
			current_row = *it2;
			current_iter = it2;
			set_values_from_model(current_row);
        }
    }	

	for (auto& connection : con) connection.unblock();
}

void EpBox::Eyepiecebox::create_ep_combo_model()
{
	m_emodel->signal_changed().connect(sigc::mem_fun(*this, &Eyepiecebox::ep_changed));

	const auto& eptreemodel = m_ecombomodel.get_epmodel();

    m_emodel->set_model(eptreemodel);
    m_emodel->set_id_column(1);
    m_cell.property_ellipsize() =  Pango::EllipsizeMode::END;
    m_cell.property_max_width_chars() = 38;
    m_emodel->set_button_sensitivity(Gtk::SensitivityType::ON);
    m_emodel->pack_start(m_cell);
    m_emodel->set_popup_fixed_width(false);
    m_emodel->set_active(0);
    
    // set the first eyepiece we find;

    m_emodel->set_cell_data_func(m_cell, sigc::mem_fun(*this, &Eyepiecebox::on_cell_data_changed));

    if (eptreemodel->children().size() > 0)
    {

        Gtk::TreeIter it = eptreemodel->children().begin();
        if(it->children().size() > 0)
        { 
            auto it2 = it->children().begin();
			current_row = *it2;
			current_iter = it2;
            m_emodel->set_active(it2);
        }
    }
}
