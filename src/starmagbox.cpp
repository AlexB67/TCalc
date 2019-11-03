#include "starmagbox.hpp"

MagBox::Magbox::Magbox()
{
    m_frame.set_label_widget(m_framelabel);
    m_framelabel.set_markup(_("<b>Star Limiting magnitude</b>"));
    m_frame.set_label_align( Gtk::ALIGN_CENTER,  Gtk::ALIGN_CENTER);
   // m_frame.set_shadow_type(Gtk::SHADOW_NONE);
	
	Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);

    m_sitetypelabel.set_halign(Gtk::ALIGN_CENTER);
    m_sitetypelabel.set_label(_("Select site"));
    m_explevellabel.set_halign(Gtk::ALIGN_START);
    m_explevellabel.set_label(_("Experience level"));
    m_agelabel.set_halign(Gtk::ALIGN_START);
    m_agelabel.set_label(_("Age/years"));
    m_pupillabel.set_halign(Gtk::ALIGN_START);
    m_pupillabel.set_label(_("pupil size/mm"));
    m_dirtlevellabel.set_halign(Gtk::ALIGN_START);
    m_dirtlevellabel.set_label(_("Optical cleanliness"));
    m_sqmlabel.set_halign(Gtk::ALIGN_START);
    m_sqmlabel.set_use_markup(true);
    m_sqmlabel.set_label(_("SQM/arc sec<sup> -2</sup>"));
    m_nelmlabel.set_halign(Gtk::ALIGN_START);
    m_nelmlabel.set_label(_("NELM"));
    m_zenithlabel.set_halign(Gtk::ALIGN_START);
    m_zenithlabel.set_label(_("Zenith angle/degrees"));
    m_colourlabel.set_halign(Gtk::ALIGN_START);
    m_colourlabel.set_label(_("CI (B-V)"));
    m_extinctionlabel.set_use_markup(true);
    m_extinctionlabel.set_halign(Gtk::ALIGN_START);
    m_extinctionlabel.set_label(_("Extinction/mag atm<sup>-1</sup>"));
    m_colouradjustlabel.set_halign(Gtk::ALIGN_START);
    m_colouradjustlabel.set_label(_("Customise CI"));
    m_seeinglabel.set_halign(Gtk::ALIGN_START);
    m_seeinglabel.set_label(_("Seeing disk/arcsec"));
    m_dirtlevel.set_tooltip_text(_("Dirt level of the optical elements."));
    m_extinction.set_tooltip_text(_("Extinction due to atmosphere."));
    m_nelm.set_tooltip_text(_("The magnitude of the faintest star that can be seen near zenith."));
    m_sqm.set_tooltip_text(_("Sky brightness. Can be obtained from an SQM meter to obtain the nelm value above."));
    m_colourbyname.set_tooltip_text(_("The colour index of the star, or specify a value below."));
    m_colour.set_tooltip_text(_("Custom value for the colour index of the star."));
    m_seeing.set_tooltip_text(_("Seeing conditions. 1 - 3 arcsec is typical for good to average skies."));
    m_explevel.set_tooltip_text(_("Experience level of the observer. Range is beginner to expert."));
    m_sitetype.set_tooltip_text(_("Select a preset or specify custom settings below."));
    m_zenith.set_tooltip_text(_("Zenith angle, 0 degrees is right above. 90 degrees the horizon."));
    m_age.set_tooltip_text(_("Calculate eye pupil size based on age."));
    m_pupilsize.set_tooltip_text(_("Eye pupil size of the observer. Use the slider above to estimate a value if unknown."));

    m_grid.set_hexpand(false);
	m_grid.set_vexpand(false);

}
Gtk::Frame &MagBox::Magbox::create_mag_grid()
{   
    m_grid.attach(m_explevellabel, 0, 0);
    m_grid.attach(m_explevel, 1, 0);
    m_grid.attach(m_agelabel, 0, 1);
    m_grid.attach(m_age, 1, 1);
    m_grid.attach(m_pupillabel, 0, 2);
    m_grid.attach(m_pupilsize, 1, 2);
    m_grid.attach(m_dirtlevellabel, 0, 3);
    m_grid.attach(m_dirtlevel, 1, 3);
    m_grid.attach(m_sitetypelabel, 0, 4, 2, 1);
    m_grid.attach(m_sitetype, 0, 5, 2, 1);
    m_grid.attach(m_nelmlabel, 0, 6);
    m_grid.attach(m_nelm, 1, 6);
    m_grid.attach(m_sqmlabel, 0, 7);
    m_grid.attach(m_sqm, 1, 7);
    m_grid.attach(m_zenithlabel, 0, 8);
    m_grid.attach(m_zenith, 1, 8);
    m_grid.attach(m_extinctionlabel, 0, 9);
    m_grid.attach(m_extinction, 1, 9);
    m_grid.attach(m_colourlabel, 0, 10);
    m_grid.attach(m_colourbyname, 1, 10);
    m_grid.attach(m_colouradjustlabel, 0, 11);
    m_grid.attach(m_colour, 1, 11);
    m_grid.attach(m_seeinglabel, 0, 12);
    m_grid.attach(m_seeing, 1, 12);

    m_dirtlevel.insert(0, _("Clean"));
    m_dirtlevel.insert(1, _("Standard"));
    m_dirtlevel.insert(2, _("Dirty"));

    m_sitetype.insert(0, _("Custom"));  // 6.0
    m_sitetype.insert(1, _("Excellent dark site")); // 7.8
    m_sitetype.insert(2, _("Typical truly dark site")); // 7.3
    m_sitetype.insert(3, _("Rural sky")); // 6.8
    m_sitetype.insert(4, _("Rural suburban transition")); // 6.3
    m_sitetype.insert(5, _("Suburban sky")); // 5.8
    m_sitetype.insert(6, _("Bright suburban sky"));  // 5.3
    m_sitetype.insert(7, _("Suburban urban transition"));  // 4.8
    m_sitetype.insert(8, _("City"));  // 4.3
    m_sitetype.insert(9, _("Inner City")); // 4.0  optimistic for central london :D

    m_colourbyname.insert(0, _("Custom"));
    m_colourbyname.insert(1, _("Red"));
    m_colourbyname.insert(2, _("Orange"));
    m_colourbyname.insert(3, _("Yellow"));
    m_colourbyname.insert(4, _("Light Yellow"));
    m_colourbyname.insert(5, _("White"));
    m_colourbyname.insert(6, _("Blue"));
    
    set_default_values();
    set_signal_handlers();
    
    AppGlobals::get_keyfile_config(m_frame);
    AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(m_frame);});
    m_frame.add(m_grid);

    return m_frame;
}

void MagBox::Magbox::set_default_values()
{
    m_explevel.set_draw_value(false);
    m_explevel.set_round_digits(0);
    auto adjust = Gtk::Adjustment::create(6.0, 3.0, 9.0, 1.0, 1.0);
    m_explevel.set_adjustment(adjust);
    
    m_dirtlevel.set_active(1);
    m_sitetype.set_active(0);
    m_colourbyname.set_active(5);

    m_age.set_spin_entry(24.0, 0.0, 100.0, 1.0, 2, true);
    m_zenith.set_spin_entry(45.0, 0.0, 85.0, 0.1, 2, true);
    m_colour.set_spin_entry(0.0, -6.0, 2.0, 0.05, 2, true);
    m_extinction.set_spin_entry(0.3, 0.0, 3.0, 0.05, 2, true);
    m_seeing.set_spin_entry(1.0, 0.0, 100.0, 0.1, 2, true);
    m_pupilsize.set_spin_entry(m_calc.calc_eyepupil_from_age(m_age.get_value()), 2.5, 8.0, 0.05, 2, true);
    m_nelm.set_spin_entry(6.0, 2.5, 8.0, 0.05, 2, true);
    m_sqm.set_spin_entry(m_calc.convert_nelm_to_sqm(6.0), m_calc.convert_nelm_to_sqm(2.5), 
                            m_calc.convert_nelm_to_sqm(8.0), 0.1, 2.0, true);
}

double MagBox::Magbox::get_optical_dirt_level() const
{   
    std::vector<double> m_dirtlevellist = {0.0, 0.15, 0.3};
    double dirt = m_dirtlevellist[m_dirtlevel.get_active_row_number()];
    m_dirtlevellist.clear();
    return dirt;
}

