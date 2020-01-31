#include "graphswindow.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include <glibmm/i18n.h>

void GraphsWindow::plot1()
{
    size_t constexpr numpoints = 200;
    Astrocalc::astrocalc m_calc;

    double minmag = m_calc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
    double dmag = minmag;
    double maxmag = m_calc.calc_maxmag(scopebox->m_saperture.get_value());
    
    double currentmag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double currentvmag = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                        magbox->m_seeing.get_value(), magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0,
                        scopebox->m_sreflect.get_value() / 100.0, scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(),
                        magbox->m_nelm.get_value(), magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), currentmag,
                        scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));
    
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), currentmag);

    graphbox->init_plots(1);

    for (size_t i = 0; i < numpoints; ++i)
    {
        double lmag = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                      magbox->m_seeing.get_value(), magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0,
                                      scopebox->m_sreflect.get_value() / 100.0, scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(),
                                      magbox->m_nelm.get_value(), magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), dmag,
                                      scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));
        
        graphbox->add_point(0, dmag, lmag);
        dmag += (maxmag - minmag) / (numpoints - 1);
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>M</i>", "<i>V</i><sub>mag</sub>");
    graphbox->set_title("Limiting magnitude versus magnification");

    Gdk::RGBA colour;
    if (graphtheme == "Herculean blue") colour.set_rgba(0.0, 1.0, 1.0, 1.0);
    else colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
    
    graphbox->set_line_colour(0, colour);
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    graphbox->add_single_legend(_("<i>V</i><sub>mag</sub> = ") + GlibUtils::dtostr<double>(currentvmag, 2) + 
            _(", <i>D</i><sub>e</sub> = ") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm, ") + 
            _("<i>M</i><sub>c</sub> = ") + GlibUtils::dtostr<double>(currentmag, 2) + _("x"), CairoGraphPos::LEGEND_BOTTOM, false);

    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot2()
{
    size_t constexpr numpoints = 200;
    size_t constexpr numplots = 3;
    Astrocalc::astrocalc m_calc;
    std::array<double, 3> mag;

    mag[0] = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    mag[1] = m_calc.calc_optimal_minmag(scopebox->m_saperture.get_value());
    mag[2] = m_calc.calc_optimal_maxmag(scopebox->m_saperture.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag[0]);
    double dirt = magbox->get_optical_dirt_level();
    
    graphbox->init_plots(numplots);

    for ( size_t j = 0; j < numplots; ++j)
    {
        double dzenith = 0;
        for (size_t i = 0; i < numpoints; ++i)
        {
            double lmag = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
            magbox->m_seeing.get_value(), dirt, epbox->m_etrans.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
            scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
            dzenith, magbox->m_extinction.get_value(), mag[j], scopebox->m_sobstruct.get_value() / 100.0, 
            static_cast<short>(scopebox->m_stype.get_active_row_number()));
            
            graphbox->add_point(j, dzenith, lmag); 
            dzenith += 85.0 / (numpoints - 1);
        }
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>θ</i>", "<i>V</i><sub>mag</sub>", "Nimbus Roman");
    graphbox->set_title("Limiting magnitude versus zenith angle");
    
    std::vector<Glib::ustring> legends(numplots);
    legends[0] = (_("<i>M</i> = ") + GlibUtils::dtostr<double>(mag[0], 2) + _("x,") + _(" <i>D</i><sub>e</sub> = ") + 
                                     GlibUtils::dtostr<double>(exitpupil, 2) + _("mm"));
    legends[1] = (_("<i>M</i> = ") + GlibUtils::dtostr<double>(mag[1], 2) + _("x,") + _(" <i>D</i><sub>e</sub> = 4mm"));
    legends[2] = (_("<i>M</i> = ") + GlibUtils::dtostr<double>(mag[2], 2) + _("x,") + _(" <i>D</i><sub>e</sub> = 2mm"));

    graphbox->add_multi_legends(legends, 0.0, 0.775);
    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot3()
{
    constexpr size_t numpoints = 200;
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);
    
    graphbox->init_plots(1);

    for (size_t i = 0; i < numpoints; ++i)
    {
        double lmag = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                      magbox->m_seeing.get_value(), magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0,
                                      scopebox->m_sreflect.get_value() / 100.0, daperture, magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
                                      magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), mag, scopebox->m_sobstruct.get_value() / 100.0,
                                      static_cast<short>(scopebox->m_stype.get_active_row_number()));

        graphbox->add_point(0, daperture, lmag);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>V</i><sub>mag</sub>", "Nimbus Roman");
    graphbox->set_title("Limiting magnitude versus aperture");
    
    Gdk::RGBA colour;
    if (graphtheme == "Herculean blue") colour.set_rgba(0.0, 1.0, 1.0, 1.0);
    else colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    graphbox->set_line_colour(0, colour);
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    graphbox->add_single_legend(_("<i>M</i> =") + GlibUtils::dtostr<double>(mag, 2) + _("x,") +
            _(" <i>D</i><sub>e</sub> = ") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm"), CairoGraphPos::LEGEND_TOP, false);

    
    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot4()
{
    constexpr size_t numpoints = 200;
    constexpr size_t numplots = 2;
    Astrocalc::astrocalc m_calc;
    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    graphbox->init_plots(numplots);
    
    for (size_t i = 0; i < numpoints; ++ i)
    {
        double daws = m_calc.calc_Daws_limit(daperture, optionsbox->m_wavelength.get_value());
        double rayleigh = m_calc.calc_Rayleigh_limit(daperture, optionsbox->m_wavelength.get_value(),
                                                    scopebox->m_sobstruct.get_value() / 100.0);

        graphbox->add_point(0, daperture, daws);
        graphbox->add_point(1, daperture, rayleigh);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>θ</i> / <i>\"</i>");
    graphbox->set_title("Resolution versus aperture");
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    std::vector<Glib::ustring> legends(numplots);
    legends[1]  = _("<i>θ</i><sub>r</sub> (Rayleigh)");
    legends[0]  = _("<i>θ</i><sub>d</sub> (Daw's)");
    graphbox->add_multi_legends(legends, 0.70, 0.01);
    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot5()
{
    constexpr size_t numpoints  = 200;
    constexpr size_t numplots   = 2;
    Astrocalc::astrocalc m_calc;
    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    graphbox->init_plots(numplots);

    for (size_t i = 0; i < numpoints; ++i)
    {
        double lgrasp = m_calc.calc_light_grasp(daperture, magbox->m_pupilsize.get_value());
        double lgrasp_eff = m_calc.calc_light_effective_grasp(daperture, magbox->m_pupilsize.get_value(),
                            scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0);

        graphbox->add_point(0, daperture, lgrasp);
        graphbox->add_point(1, daperture, lgrasp_eff);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>L</i>");
    graphbox->set_title("Light gathering versus aperture");
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    std::vector<Glib::ustring> legends(numplots);
    legends[0] = _("<i>L</i><sub> theoretical</sub>");
    legends[1] = _("<i>L</i><sub> effective</sub>");
   
    graphbox->add_multi_legends(legends, 0.0, 0.01);

    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot6()
{
    size_t constexpr numpoints = 40;
    size_t constexpr numplots = 14;
    Astrocalc::astrocalc m_calc;
    double minmag = m_calc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
    double maxmag = m_calc.calc_maxmag(scopebox->m_saperture.get_value());
    double minlogangle = -0.5;
    double maxlogangle =  3.25;
    double angle;
    double sb = 5.0;

    graphbox->init_plots(numplots);

    for (size_t j = 0; j < numplots - 2; ++j)
    {
        double dlogangle = minlogangle;
        for (size_t i = 0; i < numpoints; ++i)
        {
            angle = pow(10, dlogangle);
            double logcontrast = log10(m_calc.calc_thrconcs(angle, sb));
            graphbox->add_point(j, dlogangle, logcontrast);        
            dlogangle += (maxlogangle - minlogangle) / (numpoints - 1);
        }
        sb += 2.0;
    }

    double step = (maxmag - minmag) / (numpoints - 1);
    double dirt = magbox->get_optical_dirt_level();

    for (size_t i = 0; i < numpoints; ++i)
    {
        double objangle = log10(magbox->m_minoraxis.get_value() * (minmag + step * i));
        double objcontrast = (m_calc.calc_dso_contrast_in_scope(minmag + step * i, scopebox->m_stype.get_active_row_number(),
        scopebox->m_saperture.get_value(), scopebox->m_sobstruct.get_value() / 100.0,
        scopebox->m_sreflect.get_value() / 100.0, epbox->m_etrans.get_value() / 100.0,
        dirt, magbox->m_pupilsize.get_value(), magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(),
        magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value()).second);
        graphbox->add_point(numplots - 2, objangle, objcontrast, false);
    }

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("log(<i>θ</i>)", "log(<i>C</i> )");
    graphbox->set_title("Log contrast versus angle");
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    double dso_contrastline_y0 = magbox->m_dsocontrastindex.get_value();
    double dso_contrastline_y1 = dso_contrastline_y0;
    double dso_contrastline_x0 = minlogangle;
    double dso_contrastline_x1 = maxlogangle;
    graphbox->add_point(numplots - 1, dso_contrastline_x0, dso_contrastline_y0, false);
    graphbox->add_point(numplots - 1, dso_contrastline_x1, dso_contrastline_y1, false);


    Gdk::RGBA colour; colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0); 
    for(size_t j = 0; j < numplots - 2; ++j) graphbox->set_line_colour(j, colour);
    colour.set("purple");
    graphbox->set_line_colour(numplots - 2, colour);
    colour.set("green");
    graphbox->set_line_colour(numplots - 1, colour);
    

    double currentmag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());

    const std::pair<double, double> currentcontrast =
        m_calc.calc_dso_contrast_in_scope(currentmag, scopebox->m_stype.get_active_row_number(),
        scopebox->m_saperture.get_value(), scopebox->m_sobstruct.get_value() / 100.0,
        scopebox->m_sreflect.get_value() / 100.0, epbox->m_etrans.get_value() / 100.0,
        dirt, magbox->m_pupilsize.get_value(), magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(),
        magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value());

    std::vector<Glib::ustring> legends(numplots, "");
    legends[numplots - 2] = (_("log(<i>C</i> ) = ") +
                         GlibUtils::dtostr<double>(currentcontrast.second, 3) +
                         _(", log(<i>C</i><sub>o</sub>/<i>C</i> ) = ") + GlibUtils::dtostr<double>(currentcontrast.first, 3) +
                         _(", <i>M</i><sub>c</sub> = ") + GlibUtils::dtostr<double>(currentmag, 2) + _("x"));

    legends[numplots - 1] = _("log(<i>C</i><sub>o</sub>) = ") +
                        GlibUtils::dtostr<double>(magbox->m_dsocontrastindex.get_value(), 3) + _(": NELM = ") +
                        GlibUtils::dtostr<double>(magbox->m_nelm1.get_value(), 2) + _(": <i>S</i> = ") +
                        GlibUtils::dtostr<double>(magbox->m_bgsky.get_value(), 2) + _(" mag arcsec<sup>-2</sup>");

    graphbox->set_legend_scale(0.70);
    graphbox->add_multi_legends(legends, 0.05, 0.01);

    // Add contour lines text
    std::array ypos = {0.03, 0.125, 0.21, 0.26, 0.325, 0.425, 0.500, 0.560, 0.60, 0.624, 0.645, 0.67};
    std::vector<std::tuple<Glib::ustring, double, double, double, bool>> textobjects(ypos.size());

    unsigned int sb_contour = 27.0; 
    size_t j = 0;
    for ( auto& i: textobjects)
    {
        std::get<0>(i) = GlibUtils::dtostr<unsigned int>(sb_contour, 0); 
        std::get<1>(i) = 0.01; 
        std::get<2>(i) = ypos[j];
        std::get<3>(i) = 0.62;
        std::get<4>(i) = false;
        sb_contour -= 2.0;
        j++;
    }

    graphbox->add_text_objects(textobjects);
    magbox->set_dso_mode(true);
    graphbox->update_graph();
}

void GraphsWindow::plot7()
{
    constexpr size_t numpoints = 200;
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    graphbox->init_plots(1);

    for (size_t i = 0; i < numpoints; ++i)
    {
        double lunarres = m_calc.calc_lunar_res(daperture);
        graphbox->add_point(0, daperture, lunarres);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    double currentlunarres = m_calc.calc_lunar_res(scopebox->m_saperture.get_value());

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>R</i> / km");
    graphbox->set_title("Lunar resolution versus aperture");
    
    Gdk::RGBA colour;
    if (graphtheme == "Herculean blue") colour.set_rgba(0.0, 1.0, 1.0, 1.0);
    else colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
    graphbox->set_line_colour(0, colour);
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    graphbox->add_single_legend(_("<i>R</i> = ") + GlibUtils::dtostr<double>(currentlunarres, 2) + _(" km"), 
                                    CairoGraphPos::LEGEND_BOTTOM, false);
    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot8()
{
    constexpr size_t numpoints = 200;
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    graphbox->init_plots(1);
    for (size_t i = 0; i < numpoints; ++i)
    {
        double aff = m_calc.calc_aff(daperture, scopebox->m_sflen.get_value(), optionsbox->m_wavelength.get_value());

        graphbox->add_point(0, daperture, aff);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    double currentaff = m_calc.calc_aff(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value(), 
                                         optionsbox->m_wavelength.get_value());

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>θ</i> / <i>'</i>", "Nimbus Roman");
    graphbox->set_title("Aberration free field versus aperture");
    
    Gdk::RGBA colour;
    if (graphtheme == "Herculean blue") colour.set_rgba(0.0, 1.0, 1.0, 1.0);
    else colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
    graphbox->set_line_colour(0, colour);
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 2);

    graphbox->add_single_legend(_("<i>θ</i> = ") + GlibUtils::dtostr<double>(currentaff, 2) + _("<i>'</i>"), 
                                    CairoGraphPos::LEGEND_BOTTOM, false);
    magbox->set_dso_mode(false);
    graphbox->update_graph();
}

void GraphsWindow::plot9()
{
    constexpr size_t numpoints = 200;
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);

    graphbox->init_plots(1);
    for (size_t i = 0; i < numpoints; ++i)
    {
        double brightnessfactor = m_calc.calc_brightness_factor(daperture, magbox->m_pupilsize.get_value(), mag);
        if (brightnessfactor  > 1.0) brightnessfactor  = 1.0;
        graphbox->add_point(0, daperture, brightnessfactor);
        daperture += (maxaperture - minaperture) / (numpoints - 1);
    }

    double currentbrightness = m_calc.calc_brightness_factor(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), mag);

    graphbox->show_legend(showgraphlegend->get_active());
    graphbox->set_axes_labels("<i>D</i><sub>a</sub> / mm", "<i>B</i>", "Nimbus Roman");
    graphbox->set_title("Brightness factor versus aperture");
    Gdk::RGBA colour;
    if (graphtheme == "Herculean blue") colour.set_rgba(0.0, 1.0, 1.0, 1.0);
    else colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
    graphbox->set_line_colour(0, colour);
    graphbox->set_tick_label_format_x(false, 2);
    graphbox->set_tick_label_format_y(false, 4);

    graphbox->add_single_legend(_("<i>B</i> = ") + GlibUtils::dtostr<double>(currentbrightness, 4) + 
                                _(", <i>M</i><sub>c</sub> = ") + GlibUtils::dtostr<double>(mag, 2) + _("x, <i>D</i><sub>e</sub> = ") + 
                                GlibUtils::dtostr<double>(exitpupil, 2) + _(" mm"),
                                CairoGraphPos::LEGEND_TOP, false);

    magbox->set_dso_mode(false);
    graphbox->update_graph();
}
