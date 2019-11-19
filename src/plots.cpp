#include "graphswindow.hpp"
#include "astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include <glibmm/i18n.h>
#include <gtkmm-plplot/plotobject2dtext.h>

void GraphsWindow::plot1()
{
    if (plot)
        canvas.remove_plot(*plot);

    size_t constexpr numpoints = 200;
    std::vector<double> mag(numpoints), lmag(numpoints);
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

    size_t i = 0;
    for (auto &iter : lmag)
    {
        iter = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                      magbox->m_seeing.get_value(), magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0,
                                      scopebox->m_sreflect.get_value() / 100.0, scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(),
                                      magbox->m_nelm.get_value(), magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), dmag,
                                      scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));
        mag[i] = dmag;
        dmag += (maxmag - minmag) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(mag, lmag, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);
    
    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);

    plot->set_axis_title_x(_("#fiM"));
    plot->set_axis_title_y(_("#fi V#fn#dmag#u"));
    xvallabel.set_markup(_("<i>M</i>   "));
    yvallabel.set_markup(_("<i>V</i><sub>mag</sub>"));
    xunit.clear();
    yunit.clear();
    plot->set_plot_title(_("Limiting magnitude versus magnification"));
    plot->set_legend_position(0.04, 0.92);
    plot_data->set_name(_("#fiV#fn#dmag#u = ") + GlibUtils::dtostr<double>(currentvmag, 2) + 
                        _(", #fiD#fn#de#u = ") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm, ") + 
                         _("#fiM#fn#dc#u = ") + GlibUtils::dtostr<double>(currentmag, 2) + _("x")); 

    double xmin, xmax, ymin, ymax;  plot_data->get_extremes(xmin, xmax, ymin, ymax);
    create_gridlines(xmin, xmax, ymin, ymax);

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
};

void GraphsWindow::plot2()
{
    if (plot)
        canvas.remove_plot(*plot);

    size_t constexpr numpoints = 200;
    std::vector<double> zenith(numpoints), lmag(numpoints), lmagoptmin(numpoints), lmagoptmax(numpoints);
    Astrocalc::astrocalc m_calc;
    double dzenith = 0;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);
    double optminmag = m_calc.calc_optimal_minmag(scopebox->m_saperture.get_value());
    double optmaxmag = m_calc.calc_optimal_maxmag(scopebox->m_saperture.get_value());
    double dirt = magbox->get_optical_dirt_level();

    size_t i = 0;
    for (auto &iter : lmag)
    {

        iter = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                      magbox->m_seeing.get_value(), dirt, epbox->m_etrans.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
                                      scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
                                      dzenith, magbox->m_extinction.get_value(), mag,
                                      scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));

        lmagoptmin[i] = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                               magbox->m_seeing.get_value(), dirt, epbox->m_etrans.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
                                               scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
                                               dzenith, magbox->m_extinction.get_value(), optminmag,
                                               scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));

        lmagoptmax[i] = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                               magbox->m_seeing.get_value(), dirt, epbox->m_etrans.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
                                               scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
                                               dzenith, magbox->m_extinction.get_value(), optmaxmag,
                                               scopebox->m_sobstruct.get_value() / 100.0, static_cast<short>(scopebox->m_stype.get_active_row_number()));

        zenith[i] = dzenith;
        dzenith += 85.0 / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(zenith, lmag, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    auto plot_data1 = Gtk::make_managed<Gtk::PLplot::PlotData2D>(zenith, lmagoptmin, Gdk::RGBA("purple"),
                                                                 Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    auto plot_data2 = Gtk::make_managed<Gtk::PLplot::PlotData2D>(zenith, lmagoptmax, Gdk::RGBA("green"),
                                                                 Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);
    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);
    plot->add_data(*plot_data1);
    plot->add_data(*plot_data2);

    xvallabel.set_markup(_("<i>θ</i><sub>zenith</sub> "));
    yvallabel.set_markup(_("<i>V</i><sub>mag</sub>"));
    xunit = _("°");
    yunit.clear();
    plot->set_axis_title_x(_("#fiθ#fn#dzenith#u"));
    plot->set_axis_title_y(_("#fiV#fn#dmag#u"));
    plot->set_plot_title(_("Limiting magnitude versus zenith angle"));
    plot->set_legend_position(0.42, 0.80);

    plot_data->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(mag, 2) + _("x") + _(" #fiD#fn#de#u=") + 
                                        GlibUtils::dtostr<double>(exitpupil, 2) + _("mm"));

    plot_data1->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(optminmag, 2) + _("x") + _(" #fiD#fn#de#u=4mm"));
    plot_data2->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(optmaxmag, 2) + _("x") + _(" #fiD#fn#de#u=2mm"));

    const auto findmax = [](const double a, const double b, const double c) -> double 
    {
        double tmp = a; if (b > tmp) tmp = b; if(c > tmp)tmp = c; return tmp; 
    };

    double max = findmax(lmagoptmin[0], lmagoptmax[0], lmag[0]);
    create_gridlines(zenith[0], zenith[numpoints - 1], lmagoptmin[numpoints - 1], max);

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot3()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), lmag(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);

    size_t i = 0;
    for (auto &iter : lmag)
    {
        iter = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                                      magbox->m_seeing.get_value(), magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0,
                                      scopebox->m_sreflect.get_value() / 100.0, daperture, magbox->m_pupilsize.get_value(), magbox->m_nelm.get_value(),
                                      magbox->m_zenith.get_value(), magbox->m_extinction.get_value(), mag, scopebox->m_sobstruct.get_value() / 100.0,
                                      static_cast<short>(scopebox->m_stype.get_active_row_number()));

        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, lmag, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fi V#fn#dmag#u"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>V</i><sub>mag</sub>"));
    xunit = _(" mm");
    yunit.clear();
    plot->set_plot_title(_("#fn Limiting magnitude versus aperture"));
    plot->set_legend_position(0.04, 0.92);
    plot_data->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(mag, 2) + _("x") +
                        _(" #fiD#fn#de#u=") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm"));

    double xmin, xmax, ymin, ymax;
    plot_data->get_extremes(xmin, xmax, ymin, ymax);
    create_gridlines(xmin, xmax, ymin, ymax);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot4()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), rlimit(numpoints), dlimit(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto &iter : dlimit)
    {
        iter = m_calc.calc_Daws_limit(daperture, optionsbox->m_wavelength.get_value());
        rlimit[i] = m_calc.calc_Rayleigh_limit(daperture, optionsbox->m_wavelength.get_value(),
                                               scopebox->m_sobstruct.get_value() / 100.0);

        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, dlimit, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    auto plot_data1 = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, rlimit, Gdk::RGBA("green"),
                                                                 Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);
    plot->add_data(*plot_data1);

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fiθ#fn / arcsec"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>θ</i> / arcsec"));
    xunit = _(" mm");
    yunit = _(" arcsec");
    plot->set_plot_title(_("#fn Resolution versus aperture"));
    plot->set_legend_position(0.04, 0.05);
    plot_data->set_name(_("#fiθ#fn (Daw's)"));
    plot_data1->set_name(_("#fiθ#fn (Rayleigh)"));
    create_gridlines(aperture[0], aperture[numpoints - 1], dlimit[numpoints - 1], rlimit[0]);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot5()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), lightgrasp(numpoints), lightgraspeffective(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto &iter : lightgrasp)
    {
        iter = m_calc.calc_light_grasp(daperture, magbox->m_pupilsize.get_value());
        lightgraspeffective[i] = m_calc.calc_light_effective_grasp(daperture, magbox->m_pupilsize.get_value(),
                                                                   scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0);

        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, lightgrasp, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    auto plot_data1 = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, lightgraspeffective, Gdk::RGBA("green"),
                                                                 Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);
    plot->add_data(*plot_data1);

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fiL#fn"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>L</i>"));
    xunit = _(" mm");
    yunit.clear();
    plot->set_plot_title(_("#fnLight grasp versus aperture"));
    plot->set_legend_position(0.67, 0.02);
    plot_data->set_name(_("#fiL#fn#dtheoretical"));
    plot_data1->set_name(_("#fiL#fn#deffective"));
    create_gridlines(aperture[0], aperture[numpoints - 1], lightgrasp[numpoints - 1], lightgraspeffective[0]);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot6()
{
    if (plot)
        canvas.remove_plot(*plot);

    size_t constexpr numpoints = 40;
    size_t constexpr numplots = 12;
    size_t constexpr magpoints = 40;

    Astrocalc::astrocalc m_calc;
    double minmag = m_calc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
    double maxmag = m_calc.calc_maxmag(scopebox->m_saperture.get_value());

    double minlogangle = -0.5; // default
    double maxlogangle = 3.0;

    double angle;
    double sb = 5.0;

    std::vector<double> logangle(numpoints);
    std::vector<std::vector<double>> logcontrast(numplots, std::vector<double>(numpoints));

    for (size_t j = 0; j < numplots; ++j)
    {
        size_t i = 0;
        double dlogangle = minlogangle;
        for (auto &iter : logangle)
        {
            angle = pow(10, dlogangle);
            logcontrast[j][i] = log10(m_calc.calc_thrconcs(angle, sb));
            iter = dlogangle;
            dlogangle += (maxlogangle - minlogangle) / (numpoints - 1);
            ++i;
        }
        sb += 2.0;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0, 0.85);

    std::array<Gtk::PLplot::PlotData2D *, numplots> plot_data;

    sb = 5.0;
    for (size_t i = 0; i < numplots; ++i)
    {
        plot_data[i] = Gtk::make_managed<Gtk::PLplot::PlotData2D>(logangle, logcontrast[i], Gdk::RGBA(colour),
                                                                  Gtk::PLplot::LineStyle::CONTINUOUS, 1.5);
        sb += 2.0;
    }

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data[0]);

    for (size_t i = 1; i < numplots; ++i)
        plot->add_data(*plot_data[i]);

    double step = (maxmag - minmag) / (magpoints - 1);
    double dirt = magbox->get_optical_dirt_level();

    std::vector<double> dso_contrast_y, mag_x;

    for (size_t i = 0; i < magpoints; ++i)
    {
        double tmp = log10(magbox->m_minoraxis.get_value() * (minmag + step * i));
        if (tmp > maxlogangle) break;

        mag_x.emplace_back(tmp);
        dso_contrast_y.emplace_back(m_calc.calc_dso_contrast_in_scope(minmag + step * i, scopebox->m_stype.get_active_row_number(), 
                            scopebox->m_saperture.get_value(), scopebox->m_sobstruct.get_value() / 100.0, 
                            scopebox->m_sreflect.get_value() / 100.0, epbox->m_etrans.get_value() / 100.0,
                            dirt, magbox->m_pupilsize.get_value(), magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(),
                            magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value()).second);
    }

    auto dso_contrast_threshold = Gtk::make_managed<Gtk::PLplot::PlotData2D>(mag_x, dso_contrast_y, Gdk::RGBA("purple"),
                                                                             Gtk::PLplot::LineStyle::CONTINUOUS, 1.5);

    dso_contrast_threshold->set_name(_("log(#fiC#do#u#fn/#fiC#fn)"));
    plot->add_data(*dso_contrast_threshold);

    std::vector<double> dso_contrastline_y(2), dso_contrastline_x(2);
    dso_contrastline_y[0] = magbox->m_dsocontrastindex.get_value();
    dso_contrastline_y[1] = dso_contrastline_y[0];
    dso_contrastline_x[0] = minlogangle;
    dso_contrastline_x[1] = maxlogangle;

    auto dso_contrast_line = Gtk::make_managed<Gtk::PLplot::PlotData2D>(dso_contrastline_x, dso_contrastline_y, Gdk::RGBA("green"),
                                                                        Gtk::PLplot::LineStyle::CONTINUOUS, 1.5);
    plot->add_data(*dso_contrast_line);
    dso_contrast_line->set_name("log(#fiC#fn#do#u)");

    Gtk::PLplot::PlotObject2DText *infotext;

    double pos_y = dso_contrastline_y[0] + 0.25;
    if (dso_contrastline_y[0] > logcontrast[numplots - 1][0] -0.5) pos_y -= 0.5;

    infotext = Gtk::make_managed<Gtk::PLplot::PlotObject2DText>(_("log(#fiC#fn#do#u) = ") + 
            GlibUtils::dtostr<double>(magbox->m_dsocontrastindex.get_value(), 2) + _(": NELM = ") + 
            GlibUtils::dtostr<double>(magbox->m_nelm1.get_value(), 2) + _(": #fiS#fn = ") + 
            GlibUtils::dtostr<double>(magbox->m_bgsky.get_value(), 2) + _(" mag arcsec#u-2#d"),
            minlogangle + 0.09, pos_y, Gdk::RGBA("green"));
    
    infotext->set_scale_factor(0.75);
    plot->add_object(*infotext);

    Gtk::PLplot::PlotObject2DText *visibility;

    double currentmag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    
    const std::pair<double, double> currentcontrast = m_calc.calc_dso_contrast_in_scope(currentmag, scopebox->m_stype.get_active_row_number(), 
                            scopebox->m_saperture.get_value(), scopebox->m_sobstruct.get_value() / 100.0, 
                            scopebox->m_sreflect.get_value() / 100.0, epbox->m_etrans.get_value() / 100.0,
                            dirt, magbox->m_pupilsize.get_value(), magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(),
                            magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value());

    double text_pos_y; 
    (dso_contrastline_y[0] > 4.0) ? text_pos_y = logcontrast[0][numpoints - 1] + 0.35 : text_pos_y = logcontrast[numplots -1][0] - 0.35;
 
    visibility = Gtk::make_managed<Gtk::PLplot::PlotObject2DText>(_("log(#fiC#fn) = ") + 
                GlibUtils::dtostr<double>(currentcontrast.second, 2) +
                _(", log(#fiC#do#u#fn/#fiC#fn) = ") + GlibUtils::dtostr<double>(currentcontrast.first, 3) +
                _(", #fiM#fn#dc#u = ") + GlibUtils::dtostr<double>(currentmag, 2) + _("x"),
                minlogangle + 0.30, text_pos_y, Gdk::RGBA("purple"));
    
    visibility->set_scale_factor(0.75);
    plot->add_object(*visibility);

    std::array<Gtk::PLplot::PlotObject2DText *, numplots> plottext;

    sb = 5;    
    for (size_t i = 0; i < numplots; ++i)
    {  
        plottext[i] = Gtk::make_managed<Gtk::PLplot::PlotObject2DText>(GlibUtils::dtostr<double>(sb, 0), minlogangle + 0.09, 
                                                                        logcontrast[i][0] - 0.10, Gdk::RGBA(colour));
        plottext[i]->set_scale_factor(0.65);
        plot->add_object(*plottext[i]);
        plot_data[i]->set_name(_("#fiS#fn=") + GlibUtils::dtostr<double>(sb, 0) + _(" mg″#u-2#d"));
        sb += 2;
    }



    canvas.add_plot(*plot);

    xvallabel.set_markup(_("Log(<i>θ</i>) "));
    yvallabel.set_markup(_("Log(<i>C</i>) "));
    xunit.clear();
    yunit.clear();
    plot->set_axis_title_x(_("log#d10#u(#fiθ#fn)"));
    plot->set_axis_title_y(_("log#d10#u(#fiC#fn)"));
    plot->set_plot_title(_("Log contrast versus angle"));
    create_gridlines(logangle[0], logangle[numpoints - 1], logcontrast[0][numpoints - 1], logcontrast[numplots - 1][0]);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(true);
    plot->set_legend_position(0.60, 0.15);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot7()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), lunarres(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto &iter : lunarres)
    {
        iter = m_calc.calc_lunar_res(daperture);
        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, lunarres, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fiR#fn / km"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>R</i> / km"));
    xunit = _(" mm");
    yunit = _(" km");
    plot->set_plot_title(_("#fn Resolution versus aperture"));
    plot->set_legend_position(0.04, 0.05);
    
    double currentlunarres = m_calc.calc_lunar_res(scopebox->m_saperture.get_value());
    plot_data->set_name(_("#fiR#fn = ") + GlibUtils::dtostr<double>(currentlunarres, 2) + _("km"));


    create_gridlines(aperture[0], aperture[numpoints - 1], lunarres[numpoints - 1], lunarres[0]);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot8()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), aff(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto &iter : aff)
    {
        iter = m_calc.calc_aff(daperture, scopebox->m_sflen.get_value(), optionsbox->m_wavelength.get_value());

        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, aff, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);

    double currentaff = m_calc.calc_aff(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value(), 
                                        optionsbox->m_wavelength.get_value());

    Glib::ustring infotext = _("#fiθ#fn = ") + GlibUtils::dtostr<double>(currentaff, 2) + _("arcmin");

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fiθ#fn / arcmin"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>θ</i> / arcmin"));
    xunit = _(" mm");
    yunit = _(" arcmin");
    plot->set_plot_title(_("#fnAbberation free field versus aperture"));
    plot->set_legend_position(0.04, 0.05);
    plot_data->set_name(infotext);
    create_gridlines(aperture[0], aperture[numpoints - 1], aff[numpoints - 1], aff[0]);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}

void GraphsWindow::plot9()
{
    if (plot)
        canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), brightnessfactor(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);

    size_t i = 0;
    for (auto &iter : brightnessfactor)
    {
        double tmp = m_calc.calc_brightness_factor(daperture, magbox->m_pupilsize.get_value(), mag);
        (tmp < 1.0) ? iter = tmp : iter = 1.0;

        aperture[i] = daperture;
        daperture += (maxaperture - minaperture) / numpoints;
        ++i;
    }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    auto plot_data = Gtk::make_managed<Gtk::PLplot::PlotData2D>(aperture, brightnessfactor, Gdk::RGBA(colour),
                                                                Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data);

    double currentbrightnessfactor = m_calc.calc_brightness_factor(scopebox->m_saperture.get_value(), 
                                                                    magbox->m_pupilsize.get_value(), mag);

    if ( currentbrightnessfactor > 1.0) currentbrightnessfactor = 1.0;

    Glib::ustring infotext =    _("#fiB#fn = ") + GlibUtils::dtostr<double>(currentbrightnessfactor, 2) +
                                _(", #fiD#fn#de#u = ") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm");

    plot->set_axis_title_x(_("#fiD#fn#daperture#u / mm"));
    plot->set_axis_title_y(_("#fiB#fn"));
    xvallabel.set_markup(_("<i>D</i><sub>aperture</sub>"));
    yvallabel.set_markup(_("<i>B</i>"));
    xunit = _(" mm");
    yunit.clear();
    plot->set_plot_title(_("#fnBrightness versus aperture"));
    plot->set_legend_position(0.04, 0.92);
    plot_data->set_name(infotext);
    create_gridlines(aperture[0], aperture[numpoints - 1], brightnessfactor[numpoints - 1], brightnessfactor[0]);
    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
    magbox->set_dso_mode(false);
    if (false == showgraphlegend->get_active()) plot->hide_legend();
}
