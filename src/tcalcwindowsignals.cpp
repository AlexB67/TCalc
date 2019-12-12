#include "tcalcwindow.hpp"
#include "appglobals.hpp"
#include "glibmmcustomutils.hpp"
#include "astrocalclib/astrocalc.hpp"
#include <iostream>
#include <gtkmm/aboutdialog.h>
#include <glibmm/i18n.h>
#include <gtkmm/builder.h>
#include <gtkmm/stylecontext.h>


// for signals

void TcalcWindow::set_signal_handlers()
{
	using namespace AppGlobals;

	calcbox->m_clearbutton.signal_clicked().connect([this]() {
		logbox->clear_log();
		resultsbox->clear(true);
	});

	optionsbox->m_usefstop->property_active().signal_changed().connect([this]() {
		epbox->enable_fstop(optionsbox->m_usefstop->get_active());

		if (true == optionsbox->m_usefstop->get_active())
			logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Eyepiece field stop data enabled for calculations."));
		else
			logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Eyepiece field stop data disabled for calculations."));
	});

	optionsbox->m_uselinearmethod->property_active().signal_changed().connect([this]() {
		if (true == optionsbox->m_uselinearmethod->get_active())
			logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Linear method enabled for FOV calculations."));
		else
			logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Linear method disabled for FOV calculations."));
	});

	calcbox->m_calcbutton.signal_clicked().connect(sigc::mem_fun(*this, &TcalcWindow::create_results));

	searchbutton.signal_clicked().connect(sigc::mem_fun(*this, &TcalcWindow::search));

	auto objlist = std::vector{
		scopebox->m_saperture.property_value(),
		scopebox->m_sflen.property_value(),
		scopebox->m_sobstruct.property_value(),
		scopebox->m_sreflect.property_value(),
		epbox->m_etrans.property_value(),
		epbox->m_eflen.property_value(),
		magbox->m_zenith.property_value(),
		magbox->m_pupilsize.property_value(),
		magbox->m_colour.property_value(),
		magbox->m_nelm.property_value(),
		magbox->m_dsocontrastindex.property_value(),
		magbox->m_seeing.property_value(),
		magbox->m_extinction.property_value(),
		optionsbox->m_wavelength.property_value()};

	for (auto &iter : objlist)
		iter.signal_changed().connect([this]() { if (true == AppGlobals::IMODE) create_results(); });

	magbox->m_dirtlevel.signal_changed().connect([this]() {if (true == AppGlobals::IMODE) create_results();});
}

void TcalcWindow::create_results()
{
	Astrocalc::astrocalc m_astrocalc;
	bool flag = AppGlobals::LOGFLAG;
	const int set = AppGlobals::resultsset;
	Glib::ustring stmp;
	double dtmp;

	resultsbox->clear();

	logbox->setlogtext(flag, LogView::tINFO, _("Calculating results:"));

	Glib::ustring prefix = _("<i>f</i>/");
	Glib::ustring postfix;
	dtmp = m_astrocalc.calc_fratio(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value());

	if (dtmp < 5)
	{
		postfix = _(" (Fast)");
	}
	else if (dtmp > 5 && dtmp < 8)
	{
		postfix = _(" (Medium)");
	}
	else
	{
		postfix = _(" (Slow)");
	}

	resultsbox->append_row(_("Speed"), dtmp, 2, prefix, postfix, set);

	double dmag = m_astrocalc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
	resultsbox->append_row(_("Magnification"), dmag, 2, "x", set);

	if (dmag < m_astrocalc.calc_maxmag(scopebox->m_saperture.get_value()) &&
		dmag > m_astrocalc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value()))
		logbox->setlogtext(flag, LogView::tINFO, _("Magnification in acceptable range."));
	else if (dmag > m_astrocalc.calc_maxmag(scopebox->m_saperture.get_value()))
		logbox->setlogtext(flag, LogView::tWARN, _("Magnification exceeds acceptable range."));
	else if (dmag < m_astrocalc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value()))
		logbox->setlogtext(flag, LogView::tWARN, _("Magnification less than minimum magnifcation."));
	else if (dmag > 200 && dmag < 250)
	{
		logbox->setlogtext(flag, LogView::tINFO, _("Magnification high."));
		logbox->setlogtext(flag, LogView::tINFO, _("Good seeing conditions required."));
	}
	else if (dmag > 250 && dmag < 300)
	{
		logbox->setlogtext(flag, LogView::tWARN, _("Magnification very high."));
		logbox->setlogtext(flag, LogView::tWARN, _("Excellent seeing conditions required."));
	}

	else if (dmag > 300)
	{
		logbox->setlogtext(flag, LogView::tWARN, _("Magnification extremely high."));
		logbox->setlogtext(flag, LogView::tWARN, _("Exceptional seeing conditions required."));
		logbox->setlogtext(flag, LogView::tWARN, _("Observing limited by atmospheric conditions."));
	}

	if (magbox->m_zenith.get_value() <= 60)
	{
		logbox->setlogtext(flag, LogView::tINFO, _("Zenith angle in acceptable range."));
	}
	else if (magbox->m_zenith.get_value() >= 60 && magbox->m_zenith.get_value() <= 80)
	{
		logbox->setlogtext(flag, LogView::tINFO, _("Zenith angle high."));
		logbox->setlogtext(flag, LogView::tINFO, _("Atmospheric conditions compromise viewing."));
	}
	else
	{
		logbox->setlogtext(flag, LogView::tWARN, _("Zenith angle very high."));
		logbox->setlogtext(flag, LogView::tWARN, _("Atmospheric conditions compromise viewing."));
	}

	dtmp = m_astrocalc.calc_Daws_limit(scopebox->m_saperture.get_value(), optionsbox->m_wavelength.get_value());
	resultsbox->append_row(_("Daw's limit"), dtmp, 4, "<i>'</i>", set);

	dtmp = m_astrocalc.calc_Rayleigh_limit(scopebox->m_saperture.get_value(), optionsbox->m_wavelength.get_value(),
										   scopebox->m_sobstruct.get_value() / 100.0);
	resultsbox->append_row(_("Rayleigh's limit"), dtmp, 4, "<i>'</i>", set);

	double tFOV = calctFOV();

	resultsbox->append_row(_("Field of view"), tFOV, 4, _("<sup>o</sup>"), set);

	dtmp = m_astrocalc.calc_exit_pupil(scopebox->m_saperture.get_value(),
									   scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());

	if (dtmp < 0.5)
		logbox->setlogtext(flag, LogView::tWARN, _("Exit pupil size is low, light dimming."));
	else if (dtmp >= magbox->m_pupilsize.get_value())
		logbox->setlogtext(flag, LogView::tWARN, _("Exit pupil larger than eye pupil, light loss."));
	else if (dtmp >= 2.0 && dtmp <= 4.0)
		logbox->setlogtext(flag, LogView::tINFO, _("Exit pupil in optimal range."));
	else if (dtmp >= 0.5 && dtmp < magbox->m_pupilsize.get_value())
		logbox->setlogtext(flag, LogView::tINFO, _("Exit pupil in acceptable range."));

	resultsbox->append_row(_("Exit pupil"), dtmp, 2, _("mm"), set);

	dtmp = m_astrocalc.calc_optimal_minmag(scopebox->m_saperture.get_value());
	resultsbox->append_row(_("Opt min mag"), dtmp, 2, _("x"), set);

	dtmp = m_astrocalc.calc_optimal_maxmag(scopebox->m_saperture.get_value());
	resultsbox->append_row(_("Opt max mag"), dtmp, 2, _("x"), set);

	dtmp = m_astrocalc.calc_optimal_min_flen(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value());
	resultsbox->append_row("Opt min focal length", dtmp, 2, "mm", set);

	dtmp = m_astrocalc.calc_optimal_max_flen(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value());
	resultsbox->append_row(_("Opt max focal length"), dtmp, 2, "mm", set);

	dtmp = m_astrocalc.calc_maxmag(scopebox->m_saperture.get_value());
	resultsbox->append_row(_("High mag limit"), dtmp, 2, _("x"), set);

	dtmp = m_astrocalc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
	resultsbox->append_row(_("Low mag limit"), dtmp, 2, _("x"), set);

	dtmp = scopebox->m_sflen.get_value() / m_astrocalc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
	resultsbox->append_row(_("Max focal length"), dtmp, 2, _("mm"), set);

	dtmp = scopebox->m_sflen.get_value() / m_astrocalc.calc_maxmag(scopebox->m_saperture.get_value());
	resultsbox->append_row(_("Min focal length"), dtmp, 2, _("mm"), set);

	dtmp = m_astrocalc.calc_PPI(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value(),
								scopebox->m_saperture.get_value());
	resultsbox->append_row(_("PPI"), dtmp, 2, _("x"), set);

	dtmp = m_astrocalc.calc_light_grasp(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
	resultsbox->append_row(_("Light grasp"), dtmp, 1, "", set);

	dtmp = m_astrocalc.calc_light_effective_grasp(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(),
												  scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0);
	resultsbox->append_row(_("Light grasp effective"), dtmp, 1, "", set);

	dtmp = m_astrocalc.calc_brightness_factor(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(),
											  scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
	
	resultsbox->append_row(_("Brightness factor"), dtmp, 4, "", set);

	dtmp = tFOV * Astrocalc::astrocalc::DEGTOARCMIN / (0.2507);
	resultsbox->append_row(_("Drift time"), dtmp, 1, "s", set);

	if(Astrocalc::astrocalc::REFLECTOR == scopebox->m_stype.get_active_row_number())
	{
		dtmp = m_astrocalc.calc_aff(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value(),
								 optionsbox->m_wavelength.get_value()) /  2.0;
		resultsbox->append_row(_("Collimation tol"), dtmp, 2, _("<i>\"</i>"), set);
	}
	else
	{
		resultsbox->append_row(_("Collimation tol"), "", set);
	}
	

	double dirt = magbox->get_optical_dirt_level();

	dtmp = m_astrocalc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
									   magbox->m_seeing.get_value(), dirt, epbox->m_etrans.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
									   scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
									   magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), dmag,
									   scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));

	resultsbox->append_row(_("Limiting magnitude"), dtmp, 2, "", set);

	
	dtmp =  m_astrocalc.calc_dso_contrast_in_scope(dmag, scopebox->m_stype.get_active_row_number(), scopebox->m_saperture.get_value(), 
			scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0, epbox->m_etrans.get_value() / 100.0,
			dirt, magbox->m_pupilsize.get_value(), magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(), 
			magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value()).first;

	resultsbox->append_row(_("Visibility threshold"), dtmp, 4, "", set);

	dtmp = m_astrocalc.calc_contrast_factor(scopebox->m_sobstruct.get_value() / 100.0);

	resultsbox->append_row(_("Contrast factor"), dtmp, 2, "", set);

	dtmp = m_astrocalc.calc_airydiskdiam(optionsbox->m_wavelength.get_value(), scopebox->m_sflen.get_value(),
										 scopebox->m_saperture.get_value(), scopebox->m_sobstruct.get_value() / 100.0);

	resultsbox->append_row(_("Airy disk diam"), 1000.0 * dtmp, 3, _("&#x03BC;m"), set);

	dtmp = m_astrocalc.calc_lunar_res(scopebox->m_saperture.get_value());

	resultsbox->append_row(_("Lunar resolution"), dtmp, 2, _("km"), set);

	std::tuple<double, double, double> ocularlist = m_astrocalc.calc_ocular_list(magbox->m_pupilsize.get_value(), 
													scopebox->m_saperture.get_value(),scopebox->m_sflen.get_value());

	Glib::ustring oculars = GlibUtils::dtostr<double>(std::get<0>(ocularlist), 0) + _(",") +
				GlibUtils::dtostr<double>(std::get<1>(ocularlist), 0) + _(",") +
				GlibUtils::dtostr<double>(std::get<2>(ocularlist), 1);
	
	resultsbox->append_row(_("Ocular list"), oculars + _("mm"), set);

	logbox->setlogtext(flag, LogView::tINFO, _("Calculation completed:"));

	resultsbox->get_ep_data(epbox, set);
	resultsbox->get_scope_data(scopebox, set);
}

void TcalcWindow::prefs()
{
	if (!prefswindow)
	{
		prefswindow = std::make_unique<PrefsWindow>();
		prefswindow->set_transient_for(*this);
		prefswindow->set_modal(true);

		prefswindow->showtime_ref().property_active().signal_changed().connect([this](){
        logbox->set_log_timestamp(prefswindow->showtime_ref().get_active());
        });

        prefswindow->showcolour_ref().property_active().signal_changed().connect([this](){
        logbox->set_log_colour(prefswindow->showcolour_ref().get_active());
        });

        prefswindow->usemonospace_ref().property_active().signal_changed().connect([this](){
        logbox->set_use_monospace(prefswindow->usemonospace_ref().get_active());
        });
	}

	prefswindow->present();
}

void TcalcWindow::search()
{
	if (!searchwindow)
	{
		searchwindow = std::make_unique<SearchWindow>(epbox, scopebox);
		searchwindow->set_transient_for(*this);
		searchwindow->set_modal(true);
	}

	searchwindow->present();
}

void TcalcWindow::equipment()
{
	if (!equipwindow)
	{
		equipwindow = std::make_unique<EquipWindow>(m_app);
		equipwindow->set_transient_for(*this);
		equipwindow->set_modal(true);
	}

	equipwindow->present();
}

void TcalcWindow::about()
{
	Gtk::AboutDialog aboutdialog;
	aboutdialog.set_transient_for(*this);

	aboutdialog.set_logo(Gdk::Pixbuf::create_from_resource("/org/gnome/TCalc/resources/tcalc.png", 128, 128, true));
	aboutdialog.set_program_name(_("GNOME TCalc"));
	aboutdialog.set_version("0.0.5");
	aboutdialog.set_copyright("Alexander Borro");
	aboutdialog.set_comments(_("An astronomy tool for telescopes and eyepieces for Gnome. " 
								"\nThe brother of the Qt version TCalc."));
	aboutdialog.set_license("GPL v3.0    http://www.gnu.org/licenses");
	aboutdialog.set_website("http://www.gtkmm.org");
	aboutdialog.set_website_label("gtkmm website");

	std::vector<Glib::ustring> list_authors;
	list_authors.push_back("Alexander Borro");
	aboutdialog.set_authors(list_authors);
	int id = aboutdialog.run();
	aboutdialog.present();

	switch (id)
	{
	case Gtk::RESPONSE_OK:
	case Gtk::RESPONSE_CLOSE:
	case Gtk::RESPONSE_CANCEL:
	case Gtk::RESPONSE_DELETE_EVENT:
		aboutdialog.hide();
		break;
	default:
		break;
	}
}

void TcalcWindow::shortcuts()
{
		if (!shortcutswindow)
		{
			auto builder = Gtk::Builder::create();
			builder->add_from_resource("/org/gnome/TCalc/resources/tcalcshortcuts.ui");
        	builder->get_widget<Gtk::ShortcutsWindow>("shortcuts-tcalc", shortcutswindow);
        	shortcutswindow->set_transient_for(*this);
    		shortcutswindow->set_position(Gtk::WIN_POS_CENTER_ALWAYS);
		}

		shortcutswindow->present();
}

void TcalcWindow::close()
{

	if(shortcutswindow)
	{
		shortcutswindow->hide();
		delete shortcutswindow;
	}

	if (graphswindow){
		graphswindow->hide();
	}

	if (ocularwindow)
		ocularwindow->hide();

	if (equipwindow)
		equipwindow->hide();

	if (searchwindow)
		searchwindow->hide();

	if (prefswindow)
		prefswindow->hide();

	hide();
}
