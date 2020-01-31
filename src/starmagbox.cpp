#include "starmagbox.hpp"
#include "fileio.hpp"

MagBox::Magbox::Magbox()
{
    m_frame.set_label_widget(m_framelabel);
    m_framelabel.set_markup(_("<b>Object visibility</b>"));
    m_frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

    Uidefs::set_ui_spacing<Gtk::Grid>(m_grid);
    Uidefs::set_ui_spacing<Gtk::Grid>(m_dsogrid);
    Uidefs::set_ui_spacing<Gtk::Grid>(m_stargrid);

    m_grid.set_column_homogeneous(true);
    m_stargrid.set_column_homogeneous(true);
    m_dsogrid.set_column_homogeneous(true);

    m_sep.set_margin_top(Uidefs::ROW_PADDING);

    m_sitetypelabel.set_halign(Gtk::ALIGN_CENTER);
    m_sitetypelabel.set_label(_("Select site"));
    m_sitetypelabel1.set_label(m_sitetypelabel.get_label());
    m_sitetypelabel1.set_halign(Gtk::ALIGN_CENTER);
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
    m_sqmlabel.set_label(_("SQM/mag arcsec<sup>-2</sup>"));
    m_nelmlabel.set_halign(Gtk::ALIGN_START);
    m_nelmlabel.set_label(_("NELM"));
    m_nelmlabel1.set_halign(Gtk::ALIGN_START);
    m_nelmlabel1.set_label(_("NELM"));
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
    m_minoraxislabel.set_halign(Gtk::ALIGN_START);
    m_minoraxislabel.set_label(_("Minor axis / arcmin"));
    m_majoraxislabel.set_halign(Gtk::ALIGN_START);
    m_majoraxislabel.set_label(_("Major axis / arcmin"));
    m_vmaglabel.set_halign(Gtk::ALIGN_START);
    m_vmaglabel.set_label(_("DSO magnitude"));
    m_bgskylabel.set_halign(Gtk::ALIGN_START);
    m_bgskylabel.set_markup(_("SB<sub>sky</sub>/mag arcsec<sup>-2</sup>"));
    m_dsocontrastlabel.set_halign(Gtk::ALIGN_START);
    m_dsocontrastlabel.set_label(_("Contrast index"));
    m_selectdsolabel.set_halign(Gtk::ALIGN_START);
    m_selectdsolabel.set_label(_("Select DSO"));
    m_dsotypelabel.set_halign(Gtk::ALIGN_START);
    m_dsotypelabel.set_label(_("DSO type"));
    m_dsotype.set_width_chars(12);
    m_dsotype.set_can_focus(false);
    m_dsotype.set_tooltip_text(("Short desciption for the selected DSO."));
    m_dsocontrastindexlabel.set_halign(Gtk::ALIGN_START);
    m_dsocontrastindexlabel.set_label(_("Contrast index"));
    m_dsobrightnesslabel.set_halign(Gtk::ALIGN_START);
    m_dsobrightnesslabel.set_markup(_("SB<sub>DSO</sub> / mag arcsec<sup>-2</sup>"));
    m_seeinglabel.set_label(_("Seeing disk/arcsec"));
    m_dirtlevel.set_tooltip_text(_("Dirt level of the optical elements."));
    m_extinction.set_tooltip_text(_("Extinction due to atmosphere."));
    m_nelm.set_tooltip_text(_("The magnitude of the faintest star that can be seen near zenith."));
    m_nelm1.set_tooltip_text(_("The magnitude of the faintest star that can be seen, (ideally) nearest the observation location."));
    m_sqm.set_tooltip_text(_("Sky brightness. Can be obtained from an SQM meter to obtain the nelm value above."));
    m_bgsky.set_tooltip_text(_("The brightness of the sky background deduced from an NELM measurement."));
    m_dsobrightness.set_tooltip_markup(_("The average surface brightness in mag arcsec<sup>-2</sup> of the the DSO, " 
                                        "deduced from its visual magnitude and size."));
    m_vmag.set_tooltip_text(_("The visual magnitude of DSO."));
    m_dsocontrastindex.set_tooltip_markup(_("The contast index of the DSO. Defined by 100<sup>-1/5</sup>(<i>S</i><sub>DSO</sub> - <i>S</i><sub>sky</sub> )."
                                        " Where <i>S</i> is the surface brighhtness in mag arcsec<sup>-2</sup>."));
    m_minoraxis.set_tooltip_text(_("The DSO minor axis in arc minutes (the smaller of the two axes)."));
    m_majoraxis.set_tooltip_text(_("The DSO major axis in arc minutes (the larger of the two axes)."));
    m_colourbyname.set_tooltip_text(_("The colour index of the star, or specify a value below."));
    m_colour.set_tooltip_text(_("Custom value for the colour index of the star."));
    m_seeing.set_tooltip_text(_("Seeing conditions. 1 - 3 arcsec is typical for good to average skies."));
    m_explevel.set_tooltip_text(_("Experience level of the observer. Range is beginner to expert."));
    m_sitetype.set_tooltip_text(_("Select a preset site or specify custom settings below."));
    m_sitetype1.set_tooltip_text(_("Select a preset site or specify custom settings below."));
    m_zenith.set_tooltip_text(_("Zenith angle, 0 degrees is right above. 90 degrees the horizon."));
    m_age.set_tooltip_text(_("Calculates eye pupil size based on age."));
    m_pupilsize.set_tooltip_text(_("The eye pupil size of the observer. Use the slider above to estimate pupil size if unknown."));
    m_dsocombo.set_tooltip_text(_("Select a DSO object."));
    m_grid.set_hexpand(false);
    m_grid.set_vexpand(false);
    m_grid.set_margin_top(0);
    m_stargrid.set_margin_top(0);
    m_dsogrid.set_hexpand(false);
    m_dsogrid.set_vexpand(false);
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

    m_stargrid.attach(m_sitetypelabel, 0, 0, 2, 1);
    m_stargrid.attach(m_sitetype, 0, 1, 2, 1);
    m_stargrid.attach(m_nelmlabel, 0, 2);
    m_stargrid.attach(m_nelm, 1, 2);
    m_stargrid.attach(m_sqmlabel, 0, 3);
    m_stargrid.attach(m_sqm, 1, 3);
    m_stargrid.attach(m_zenithlabel, 0, 4);
    m_stargrid.attach(m_zenith, 1, 4);
    m_stargrid.attach(m_extinctionlabel, 0, 5);
    m_stargrid.attach(m_extinction, 1, 5);
    m_stargrid.attach(m_colourlabel, 0, 6);
    m_stargrid.attach(m_colourbyname, 1, 6);
    m_stargrid.attach(m_colouradjustlabel, 0, 7);
    m_stargrid.attach(m_colour, 1, 7);
    m_stargrid.attach(m_seeinglabel, 0, 8);
    m_stargrid.attach(m_seeing, 1, 8);

    m_maingrid.attach(m_grid, 0, 0);
    m_maingrid.attach(m_stack, 0, 1);

    m_dirtlevel.insert(0, _("Clean"));
    m_dirtlevel.insert(1, _("Standard"));
    m_dirtlevel.insert(2, _("Dirty"));

    m_sitetype.insert(0, _("Custom"));                    // 6.0
    m_sitetype.insert(1, _("Excellent dark site"));       // 7.8
    m_sitetype.insert(2, _("Typical truly dark site"));   // 7.3
    m_sitetype.insert(3, _("Rural sky"));                 // 6.8
    m_sitetype.insert(4, _("Rural suburban transition")); // 6.3
    m_sitetype.insert(5, _("Suburban sky"));              // 5.8
    m_sitetype.insert(6, _("Bright suburban sky"));       // 5.3
    m_sitetype.insert(7, _("Suburban urban transition")); // 4.8
    m_sitetype.insert(8, _("City"));                      // 4.3
    m_sitetype.insert(9, _("Inner City"));                // 4.0  optimistic for central london :D

    m_sitetype1.insert(0, _("Custom"));                    // 6.0
    m_sitetype1.insert(1, _("Excellent dark site"));       // 7.8
    m_sitetype1.insert(2, _("Typical truly dark site"));   // 7.3
    m_sitetype1.insert(3, _("Rural sky"));                 // 6.8
    m_sitetype1.insert(4, _("Rural suburban transition")); // 6.3
    m_sitetype1.insert(5, _("Suburban sky"));              // 5.8
    m_sitetype1.insert(6, _("Bright suburban sky"));       // 5.3 
    m_sitetype1.insert(7, _("Suburban urban transition")); // 4.8

    m_colourbyname.insert(0, _("Custom"));
    m_colourbyname.insert(1, _("Red"));
    m_colourbyname.insert(2, _("Orange"));
    m_colourbyname.insert(3, _("Yellow"));
    m_colourbyname.insert(4, _("Light Yellow"));
    m_colourbyname.insert(5, _("White"));
    m_colourbyname.insert(6, _("Blue"));


    m_dsogrid.attach(m_sitetypelabel1, 0, 0, 2, 1);
    m_dsogrid.attach(m_sitetype1, 0, 1, 2, 1);
    m_dsogrid.attach(m_nelmlabel1, 0, 2);
    m_dsogrid.attach(m_nelm1, 1, 2);    
    m_dsogrid.attach(m_bgskylabel, 0, 3);
    m_dsogrid.attach(m_bgsky, 1, 3); 
    m_dsogrid.attach(m_selectdsolabel, 0, 4);
    m_dsogrid.attach(m_dsocombo, 1, 4);
    m_dsogrid.attach(m_dsotypelabel, 0, 5);
    m_dsogrid.attach(m_dsotype, 1, 5);   
    m_dsogrid.attach(m_vmaglabel, 0, 6);
    m_dsogrid.attach(m_vmag, 1, 6);
    m_dsogrid.attach(m_dsobrightnesslabel, 0, 7);
    m_dsogrid.attach(m_dsobrightness, 1, 7);
    m_dsogrid.attach(m_minoraxislabel, 0, 8);
    m_dsogrid.attach(m_minoraxis, 1, 8);
    m_dsogrid.attach(m_majoraxislabel, 0, 9);
    m_dsogrid.attach(m_majoraxis, 1, 9);
    m_dsogrid.attach(m_dsocontrastindexlabel, 0, 10);
    m_dsogrid.attach(m_dsocontrastindex, 1, 10);

    fileIO::dbfileIO db;
	db.load_dso_data(m_dsocombo, m_dsocombomodel);

    set_default_values();
    set_signal_handlers();

    AppGlobals::get_keyfile_config(m_frame);
    AppGlobals::frame_style.connect([this](Gtk::ShadowType type){ AppGlobals::set_frame_style(m_frame, type);});
    m_frame.add(m_maingrid);

    m_dsocombo.signal_changed().connect(sigc::mem_fun(*this, &Magbox::dso_changed));

    m_stack.set_transition_type(Gtk::StackTransitionType::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    m_stack.add(m_stargrid, _("Star objects"), _("Star objects"));
    m_stack.add(m_dsogrid, _("DSO objects mode"), _("DSO objects"));


    m_switcher.set_hexpand(false);
    m_stack.set_vexpand(false);
    m_stack.set_hexpand(false);
    m_switcher.set_stack(m_stack);

    return m_frame;
}

void MagBox::Magbox::set_ocular_mode()
{
    m_agelabel.hide();
    m_age.hide();
    m_explevellabel.hide();
    m_explevel.hide();
}

void MagBox::Magbox::set_default_values()
{
    m_explevel.set_draw_value(false);
    m_explevel.set_round_digits(0);
    auto adjust = Gtk::Adjustment::create(6.0, 3.0, 9.0, 1.0, 1.0);
    m_explevel.set_adjustment(adjust);

    m_dirtlevel.set_active(1);
    m_sitetype.set_active(0);
    m_sitetype1.set_active(0);
    m_colourbyname.set_active(5);

    m_age.set_spin_entry(24.0, 0.0, 100.0, 1.0, 2, true);
    m_zenith.set_spin_entry(45.0, 0.0, 85.0, 0.1, 2, true);
    m_colour.set_spin_entry(0.0, -6.0, 2.0, 0.05, 2, true);
    m_extinction.set_spin_entry(0.3, 0.0, 3.0, 0.05, 2, true);
    m_seeing.set_spin_entry(3.0, 0.0, 100.0, 0.1, 2, true);
    m_pupilsize.set_spin_entry(m_calc.calc_eyepupil_from_age(m_age.get_value()), 2.5, 8.0, 0.05, 2, true);
    m_nelm.set_spin_entry(6.0, 4.0, 8.0, 0.05, 2, true);

    m_sqm.set_spin_entry(m_calc.convert_nelm_to_sqm(6.0), m_calc.convert_nelm_to_sqm(4.0),
                         m_calc.convert_nelm_to_sqm(8.0), 0.1, 2.0, true);
    
    m_vmag.set_spin_entry(9.2, 0, 25.0, 0.1, 2, true);
    m_minoraxis.set_spin_entry(3.0, 0.015, 200, 0.1, 2, true);
    m_majoraxis.set_spin_entry(8.0, 0.015, 200, 0.1, 2, true);

    m_nelm1.set_spin_entry(6.0, 4.0, 8.0, 0.05, 2, true);
    m_bgsky.set_spin_entry(m_calc.calc_nelm_brightness_threshold_method(6.0), m_calc.calc_nelm_brightness_threshold_method(4.0),
                         m_calc.calc_nelm_brightness_threshold_method(8.0), 0.1, 2.0, true);
    
    double brightness = m_calc.calc_dso_mag_to_brightness(m_vmag.get_value(), m_minoraxis.get_value(), m_majoraxis.get_value());
    m_dsobrightness.set_spin_entry(brightness, 0, 100, 0.1, 2, true);

    m_dsocontrastindex.set_spin_entry(m_calc.calc_contrast_index(m_bgsky.get_value(), brightness), -100, 100, 0.01, 2, true);
    m_dsocombo.set_active(3);
    m_dsotype.set_text(_("Irregular galaxy"));
}

double MagBox::Magbox::get_optical_dirt_level() const
{
    std::vector<double> m_dirtlevellist = {0.0, 0.15, 0.3};
    double dirt = m_dirtlevellist[m_dirtlevel.get_active_row_number()];
    m_dirtlevellist.clear();
    return dirt;
}
