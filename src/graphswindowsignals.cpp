#include "graphswindow.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"

void GraphsWindow::set_signal_handlers()
{

    searchbutton.signal_clicked().connect(sigc::mem_fun(*this, &GraphsWindow::search));

    showgraphlegend->property_active().signal_changed().connect([this]()
    {;
        (true == showgraphlegend->get_active()) ? plot->show_legend() : plot->hide_legend();
    });

    Gtk::Settings::get_default()->property_gtk_theme_name().signal_changed().connect([this]()
    {
        graphtheme = "Default";
        set_plot_theme();
    });

    Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().signal_changed().connect([this]() 
    {   
        if(true == Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().get_value())
        graphtheme = "Default";
        set_plot_theme();
    });

    AppGlobals::update_graphthemes.connect(sigc::mem_fun(*this, &GraphsWindow::set_plot_theme_by_name));

    plotlist.signal_changed().connect(sigc::mem_fun(*this, &GraphsWindow::plot_data_changed));

    auto objlist = std::vector
    {
        scopebox->m_saperture.property_value(),
        scopebox->m_sflen.property_value(),
        scopebox->m_sobstruct.property_value(),
        scopebox->m_sreflect.property_value(),
        epbox->m_etrans.property_value(),
        epbox->m_eflen.property_value(),
        epbox->m_efstop.property_value(),
        magbox->m_zenith.property_value(),
        magbox->m_pupilsize.property_value(),
        magbox->m_colour.property_value(),
        magbox->m_nelm.property_value(),
        magbox->m_seeing.property_value(),
        magbox->m_extinction.property_value(),
        magbox->m_dsocontrastindex.property_value(),
        magbox->m_dsobrightness.property_value(),
        optionsbox->m_wavelength.property_value()
    };

    for (auto &iter: objlist)
        iter.signal_changed().connect(sigc::mem_fun(*this, &GraphsWindow::plot_data_changed));

    optionsbox->m_uselinearmethod->property_active().signal_changed().connect([this]() {
        plot_data_changed();
    });

    optionsbox->m_usefstop->property_active().signal_changed().connect([this]() {
        if (epbox->m_efstop.get_value() > Astrocalc::astrocalc::tSMALL) plot_data_changed();
        epbox->m_efstop.set_sensitive(optionsbox->m_usefstop->get_active());
    });

    magbox->m_dirtlevel.signal_changed().connect(sigc::mem_fun(*this, &GraphsWindow::plot_data_changed));
}

void GraphsWindow::plot_data_changed()
{
    xval.set_text("");
    yval.set_text("");

    auto plotfunc = graphlist[plotlist.get_active_row_number()];
    std::invoke(plotfunc, *this);
}

void GraphsWindow::track_cursor(double x, double y) 
{
        xval.set_text(_(" = ") + GlibUtils::dtostr<double>(x, 4) + xunit);
        yval.set_text(_(" = ") + GlibUtils::dtostr<double>(y, 4) + yunit);
}

void GraphsWindow::search()
{
    if (!searchwindow)
		{
			searchwindow = std::make_unique<SearchWindow>(epbox, scopebox);
			searchwindow->set_transient_for(*this);
			searchwindow->set_modal(true);
		}
		
	searchwindow->present();
}
