#include <glibmm/i18n.h>
#include "graphswindow.hpp"
#include "astrocalc.hpp"
#include "glibmmcustomutils.hpp"

void GraphsWindow::plot1()
{
    if(plot) canvas.remove_plot(*plot);

    size_t constexpr numpoints = 200;
    std::vector<double> mag(numpoints), lmag(numpoints);
    Astrocalc::astrocalc m_calc;

    double minmag = m_calc.calc_minmag(scopebox->m_saperture.get_value(), magbox->m_pupilsize.get_value());
    double dmag = minmag;
    double maxmag = m_calc.calc_maxmag(scopebox->m_saperture.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(),
                                m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value()));

    size_t i = 0;
    for (auto& iter : lmag)
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
    
    plot->set_axis_title_x("#fiM");
    plot->set_axis_title_y("#fi V#fn#dmag#u");
    xvallabel.set_markup(_("<i>M</i>   "));
    yvallabel.set_markup(_("<i>V</i><sub>mag</sub>"));
    xunit.clear();
    yunit.clear();
    plot->set_plot_title("Limiting magnitude versus magnification");
    plot->set_legend_position(0.04, 0.92);
    plot_data->set_name("#fiD#fn#de#u = " + GlibUtils::dtostr<double>(exitpupil, 2) + "mm");

    double xmin, xmax, ymin, ymax;  plot_data->get_extremes(xmin, xmax, ymin, ymax);
    create_gridlines(xmin, xmax, ymin, ymax);
    plot->show_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
};

void GraphsWindow::plot2()
{
    if(plot) canvas.remove_plot(*plot);

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
    for (auto& iter : lmag)
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
    xunit = _(" deg");
    yunit.clear();
    plot->set_axis_title_x(_("#fiθ#fn#dzenith#u"));
    plot->set_axis_title_y(_("#fiV#fn#dmag#u"));
    plot->set_plot_title(_("Limiting magnitude versus zenith angle"));
    plot->set_legend_position(0.35, 0.80);
    plot_data->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(mag,2) + _("x")
                        + _(" #fiD#fn#de#u=") + GlibUtils::dtostr<double>(exitpupil, 2) + _("mm"));
    plot_data1->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(optminmag, 2) + _("x") + _(" #fiD#fn#de#u=4mm"));
    plot_data2->set_name(_("#fiM#fn=") + GlibUtils::dtostr<double>(optmaxmag, 2) + _("x") + _(" #fiD#fn#de#u=2mm"));

    const auto findmax = [](const double a, const double b, const double c)->double 
    {double tmp = a; if (b > tmp) tmp = b; if(c > tmp)tmp = c; return tmp;};

    double max = findmax(lmagoptmin[0], lmagoptmax[0], lmag[0]);
    create_gridlines(zenith[0], zenith[numpoints - 1], lmagoptmin[numpoints - 1], max);
    plot->show_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
}

void GraphsWindow::plot3()
{
    if (plot) canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), lmag(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;
    double mag = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
    double exitpupil = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), mag);

    size_t i = 0;
    for (auto& iter : lmag)
    {
        iter = m_calc.calc_lmag_scope(static_cast<int>(magbox->m_explevel.get_value()), magbox->m_colour.get_value(),
                magbox->m_seeing.get_value(),  magbox->get_optical_dirt_level(), epbox->m_etrans.get_value() / 100.0, 
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
    
    double xmin, xmax, ymin, ymax;  plot_data->get_extremes(xmin, xmax, ymin, ymax);
    create_gridlines(xmin, xmax, ymin, ymax);
    plot->show_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
}

void GraphsWindow::plot4()
{
    if (plot) canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), rlimit(numpoints), dlimit(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto& iter : dlimit)
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
    create_gridlines(aperture[0], aperture[numpoints - 1], dlimit[numpoints - 1],  rlimit[0]);
    plot->show_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
}

void GraphsWindow::plot5()
{
    if (plot) canvas.remove_plot(*plot);

    constexpr size_t numpoints = 200;
    std::vector<double> aperture(numpoints), lightgrasp(numpoints), lightgraspeffective(numpoints);
    Astrocalc::astrocalc m_calc;

    double minaperture = 60.0;
    double maxaperture = 450.0;
    double daperture = minaperture;

    size_t i = 0;
    for (auto& iter : lightgrasp)
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
    plot_data->set_name(_("#fiL#fn#dtheoretical#u"));
    plot_data1->set_name(_("#fiL#fn#deffective#u"));
    create_gridlines(aperture[0], aperture[numpoints - 1], lightgrasp[numpoints - 1],  lightgraspeffective[0]);
    plot->show_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
}

void GraphsWindow::plot6()
{
    if (plot) canvas.remove_plot(*plot);

    size_t constexpr numpoints = 40;
    size_t constexpr numplots = 11;
    double minlogangle = -0.5;
    double maxlogangle = 3.1;
    double angle;
    double sb = 4.0;
    std::vector<double> logangle(numpoints);
    std::vector<std::vector<double> > logcontrast(numplots, std::vector<double>(numpoints));

    Astrocalc::astrocalc m_calc;

     for (size_t j = 0; j < numplots; ++j)
      {
          size_t i = 0;
          double dlogangle = minlogangle;
          for(auto& iter : logangle)
          {
              angle = pow(10, dlogangle);
              //            Y = m_calc.calc_thrconls(angle, sb); // lower resolution version
              logcontrast[j][i] = log10(m_calc.calc_thrconcs(angle, sb));
              dlogangle +=  (maxlogangle - minlogangle) / numpoints;
              iter = dlogangle;
              ++i;
          }

          if ( j < 1)
              sb += 5.0;
          else
              sb += 2.0;
      }

    Gdk::RGBA colour;
    colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);

    std::array<Gtk::PLplot::PlotData2D *, 11> plot_data;

    for (size_t i = 0;  i < numplots; ++i)
       plot_data[i] = Gtk::make_managed<Gtk::PLplot::PlotData2D>(logangle, logcontrast[i], Gdk::RGBA(colour),
                                                                    Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);

    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_data[0]);

    for (size_t i = 1;  i < numplots; ++i) plot->add_data(*plot_data[i]);

    xvallabel.set_markup(_("Log(<i>θ</i>) "));
    yvallabel.set_markup(_("Log(<i>C</i>) "));
    xunit.clear();
    yunit.clear();
    plot->set_axis_title_x(_("Log(#fiθ#fn)"));
    plot->set_axis_title_y(_("Log(#fiC#fn)"));
    plot->set_plot_title(_("Log contrast versus angle"));
    create_gridlines(logangle[0], logangle[numpoints - 1], logcontrast[0][numpoints - 1],  logcontrast[numplots - 1][0]);
    plot->hide_legend();

    canvas.add_plot(*plot);
    plot->signal_cursor_motion().connect(sigc::mem_fun(*this, &GraphsWindow::track_cursor));
    set_plot_theme_by_name(graphtheme);
}