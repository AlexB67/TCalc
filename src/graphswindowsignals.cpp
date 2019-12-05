#include "graphswindow.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"

void GraphsWindow::set_signal_handlers()
{

    searchbutton.signal_clicked().connect(sigc::mem_fun(*this, &GraphsWindow::search));

    showgraphlegend->property_active().signal_changed().connect([this]()
    {;
        graphbox->show_legend(showgraphlegend->get_active());
        graphbox->update_graph();
    });

    Gtk::Settings::get_default()->property_gtk_theme_name().signal_changed().connect([this]()
    {
        if (Gtk::Settings::get_default()->property_gtk_theme_name().get_value().lowercase() == "adwaita")
            graphbox->set_theme("Adwaita");
        else if (Gtk::Settings::get_default()->property_gtk_theme_name().get_value().lowercase() == "adwaita-dark")
            graphbox->set_theme("Adwaita-dark");
        else
            graphbox->set_theme(graphtheme);
        
        graphbox->update_graph();
    });

    Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().signal_changed().connect([this]() 
    {   
        if(true == Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().get_value())
            graphbox->set_theme("Fade to black");
        else
            graphbox->set_theme("Adwaita");
        
        graphbox->update_graph();
    });

    AppGlobals::update_graphthemes.connect(sigc::mem_fun(*this, &GraphsWindow::set_plot_theme));

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
    auto plotfunc = graphlist[plotlist.get_active_row_number()];
    std::invoke(plotfunc, *this);
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
