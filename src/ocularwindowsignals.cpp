#include "ocularwindow.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"
#include <gtkmm/settings.h>

void OcularWindow::set_signal_handlers()
{
  searchbutton.signal_clicked().connect(sigc::mem_fun(*this, &OcularWindow::search));

  nightmode->property_active().signal_changed().connect([this]() {
    if (true == nightmode->get_active())
    {
      provider->load_from_resource("/org/gnome/TCalc/resources/nightmode.css");
      context = get_style_context();
      context->add_provider_for_screen(Gdk::Screen::get_default(),
                                       provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
      auto settings = Gtk::Settings::get_default();
      preferdarktheme = settings->property_gtk_application_prefer_dark_theme().get_value();
      currenttheme = settings->property_gtk_theme_name().get_value();
      settings->property_gtk_application_prefer_dark_theme().set_value(true);
      settings->property_gtk_theme_name().set_value(_("Adwaita-dark"));
    }
    else
    {
      context->remove_provider_for_screen(Gdk::Screen::get_default(), provider);
      auto settings = Gtk::Settings::get_default();
      settings->property_gtk_application_prefer_dark_theme().set_value(preferdarktheme);
      settings->property_gtk_theme_name().set_value(currenttheme);
    }
  });

  magbox->m_dsocombo.signal_changed().connect(sigc::mem_fun(*this, &OcularWindow::dso_changed));

  auto objlist = std::vector{
      scopebox->m_saperture.property_value(),
      scopebox->m_sflen.property_value(),
      epbox->m_eflen.property_value(),
      epbox->m_efov.property_value(),
      epbox->m_efstop.property_value(),
  };

  for (auto &iter : objlist)
    iter.signal_changed().connect(sigc::mem_fun(*this, &OcularWindow::ocular_changed));

  epbox->m_emodel->signal_changed().connect([this]() {
    ocular_changed();
  });

  scopebox->m_smodel->signal_changed().connect([this]() {
    ocular_changed();
  });

  optionsbox->m_uselinearmethod->property_active().signal_changed().connect([this]() {
    ocular_changed();
  });

  optionsbox->m_usefstop->property_active().signal_changed().connect([this]() {
    if (epbox->m_efstop.get_value() > Astrocalc::astrocalc::tSMALL)
      ocular_changed();
    epbox->m_efstop.set_sensitive(optionsbox->m_usefstop->get_active());
  });

  magbox->m_dsocontrastindex.signal_changed().connect(sigc::mem_fun(*this, &OcularWindow::set_contrast_info));
}

void OcularWindow::ocular_changed()
{
  ocularbox.m_efov = epbox->m_efov.get_value();
  set_ocular_info();
  set_contrast_info();
  ocularbox.queue_draw();
}

void OcularWindow::dso_changed()
{
  Gtk::TreeModel::iterator iter = magbox->m_dsocombo.get_active();

  if (iter)
  {
    Gtk::TreeModel::Row row = *iter;
    if (row)
    {
      ocularbox.m_imagefile = static_cast<std::string>(row[magbox->m_dsocombomodel.m_dsocols.m_DSOimagefile]);
      ocularbox.m_efov = epbox->m_efov.get_value();
      ocularbox.m_imagesize = row[magbox->m_dsocombomodel.m_dsocols.m_DSOimagesize];
      ocularbox.queue_draw();
      set_ocular_info();
      set_contrast_info();
    }
  }
}

void OcularWindow::set_contrast_info()
{
  Astrocalc::astrocalc m_calc;

  double threshold =
      m_calc.calc_dso_contrast_in_scope(ocularbox.magnification, scopebox->m_stype.get_active_row_number(), scopebox->m_saperture.get_value(),
                                        scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
                                        epbox->m_etrans.get_value() / 100.0, magbox->get_optical_dirt_level(), magbox->m_pupilsize.get_value(),
                                        magbox->m_nelm1.get_value(), magbox->m_vmag.get_value(),
                                        magbox->m_minoraxis.get_value(), magbox->m_majoraxis.get_value()).first;

  obscontrast.set_text(GlibUtils::dtostr<double>(threshold, 4));
  ocularbox.obscontrast = threshold;
  ocularbox.skyscopebrightness = m_calc.calc_bgsky_in_scope(ocularbox.magnification, scopebox->m_stype.get_active_row_number(), scopebox->m_saperture.get_value(),
                                        scopebox->m_sobstruct.get_value() / 100.0, scopebox->m_sreflect.get_value() / 100.0,
                                        epbox->m_etrans.get_value() / 100.0, magbox->get_optical_dirt_level(), magbox->m_pupilsize.get_value(),
                                        magbox->m_nelm1.get_value());
}

void OcularWindow::set_ocular_info()
{
  using namespace Astrocalc;
  astrocalc m_calc;

  double dtmp = m_calc.calc_exit_pupil(scopebox->m_saperture.get_value(), scopebox->m_sflen.get_value(),
                                       epbox->m_eflen.get_value());
  ocularexitpupil.set_text(GlibUtils::dtostr<double>(dtmp, 2) + _("mm"));
  ocularbox.exitpupil = dtmp;

  dtmp = m_calc.calc_MagL(scopebox->m_sflen.get_value(), epbox->m_eflen.get_value());
  ocularmag.set_text(GlibUtils::dtostr<double>(dtmp, 2) + _("x"));
  ocularbox.magnification = dtmp;

  using namespace AppGlobals;
  short method;

  (true == optionsbox->m_uselinearmethod->get_active()) ? method = astrocalc::USESMALLANGLEMETHOD
                                                        : method = astrocalc::USESTDMETHOD;

  if (true == optionsbox->m_usefstop->get_active())
  {
    dtmp = m_calc.calc_tFOV(scopebox->m_sflen.get_value(), epbox->m_efstop.get_value());

    if (astrocalc::USESTDMETHOD == method)
      dtmp = dtmp * m_calc.calc_kfactor(epbox->m_efov.get_value()) / epbox->m_efov.get_value();
  }
  else
  {
    dtmp = m_calc.calc_tFOV(epbox->m_efov.get_value(), scopebox->m_sflen.get_value(), epbox->m_eflen.get_value(), method);
  }

  ocularbox.m_tfov = dtmp;
  // dtmp /= astrocalc::DEGTOARCMIN;

  ocularfov.set_text(GlibUtils::dtostr<double>(dtmp, 2) + _("' (") + GlibUtils::dtostr<double>(dtmp / astrocalc::DEGTOARCMIN, 4) + _("°)"));
}

void OcularWindow::search()
{
  if (!searchwindow)
  {
    searchwindow = std::make_unique<SearchWindow>(epbox, scopebox);
    searchwindow->set_transient_for(*this);
    searchwindow->set_modal(true);
  }

  searchwindow->present();
}
