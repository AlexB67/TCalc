#include "graphswindow.hpp"
#include "glibmmcustomutils.hpp"
#include "astrocalc.hpp"
#include "appglobals.hpp"
#include <glibmm/i18n.h>
#include <filesystem>

GraphsWindow::GraphsWindow()
{
  searchbutton.set_image_from_icon_name("edit-find-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  searchbutton.set_tooltip_text(_("Search for an eyepiece or telescope model."));
  headerbar.set_title(_("Interactive graphs"));
  headerbar.set_subtitle(_("An astronomy tool for telescopes and eyepieces."));
  headerbar.set_show_close_button();
  showgraphlegend = Gtk::make_managed<Gtk::Switch>();
  showgraphlegend->set_active(true);
  showgraphlegend->set_tooltip_text(_("Show the graph legend."));
  headerbar.pack_start(searchbutton);
  headerbar.pack_end(*showgraphlegend);
  set_titlebar(headerbar);
  
  epbox = std::make_shared<EpBox::Eyepiecebox>();
  scopebox = std::make_shared<ScopeBox::Telescopebox>();
  magbox = std::make_unique<MagBox::Magbox>();
  optionsbox = std::make_unique<OptionsBox::Optionsbox>();
  scopebox->frame_can_expand(true);

  plotframe.set_label_widget(plotframe_label);
  plotframe_label.set_markup(_("<b>Graph</b>"));
  plotframe.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
  plotframe.set_border_width(Uidefs::BORDER_WIDTH);
  AppGlobals::get_keyfile_config(plotframe);
  AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(plotframe);});
 
  canvas.set_hexpand(true);
  canvas.set_vexpand(true);

  plotlist.insert(0, _("limiting magnitude versus magnification"));
  plotlist.insert(1, _("limiting magnitude versus zenith angle"));
  plotlist.insert(2, _("limiting magnitude versus aperture"));
  plotlist.insert(3, _("Resolving power versus aperture"));
  plotlist.insert(4, _("Light gathering versus aperture"));
  plotlist.insert(5, _("DSO visbility"));
  plotlist.insert(6, _("Lunar resolution"));
  plotlist.insert(7, _("Abberation free field versus aperture"));
  plotlist.insert(8, _("Brightness factor"));
  plotlist.set_active(0);

  xval.set_width_chars(14);
  xval.set_editable(false);
  xval.set_can_focus(false);
  yval.set_width_chars(14);
  yval.set_editable(false);
  yval.set_can_focus(false);
  
  plotgrid.attach(canvas, 0, 0, 6, 1);
  plotgrid.attach(plotlabel, 0, 1);
  plotgrid.attach(plotlist, 1, 1);
  plotgrid.attach(xvallabel, 2, 1);
  plotgrid.attach(xval, 3, 1);
  plotgrid.attach(yvallabel, 4, 1);
  plotgrid.attach(yval, 5, 1);

  plotframe.add(plotgrid);

  controlsgrid.attach(epbox->create_eyepiece_grid(), 0, 0);
  controlsgrid.attach(scopebox->create_telescope_grid(), 0, 1);

  controlsgrid2.attach(magbox->create_mag_grid(), 0, 0);
  controlsgrid2.attach(optionsbox->create_options_grid(), 0, 1);

  windowgrid.attach(plotframe, 0, 0, 6, 5);
  windowgrid.attach(controlsgrid, 6, 0);
  windowgrid.attach(controlsgrid2, 7, 0);

  Uidefs::set_ui_spacing<Gtk::Grid>(plotgrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid2);

  init_plot();
  canvas.set_can_focus(false);

  add(windowgrid);
  set_signal_handlers();
  show_all_children();
  get_config();
  epbox->init();
  scopebox->init();
  magbox->set_default_mode();
  magbox->set_stack_transition_time(0);
}

void  GraphsWindow::get_config()
{ 
  if (false == std::filesystem::exists(AppGlobals::configpath.c_str())) return;

  Glib::KeyFile keyfile;
  keyfile.load_from_file(AppGlobals::configpath);

  int index = keyfile.get_integer("Appearance", "graphtheme");

  std::vector<Glib::ustring> themes{_("Default"), _("Natural blend"), _("Natural blend dark")};

  graphtheme = themes[index];
}

void GraphsWindow::create_gridlines(const double xmin, const double xmax, const double ymin, const double ymax) const
{

  // x, y axis grid lines

  double posy[3] = {(ymax + ymin) / 2.0,  (ymax + ymin) / 2.0 - (ymax - ymin) / 4.0, (ymax + ymin) / 2.0 + (ymax - ymin) / 4.0};
  double posx[3] = {(xmax + xmin) / 2.0,  (xmax + xmin) / 2.0 - (xmax - xmin) / 4.0, (xmax + xmin) / 2.0 + (xmax - xmin) / 4.0};

  std::vector<Gtk::PLplot::PlotObject2DLine *> gridlinesx;
  std::vector<Gtk::PLplot::PlotObject2DLine *> gridlinesy;

  for (size_t i = 0; i < 3; ++i)
  {
    gridlinesy.emplace_back(Gtk::make_managed<Gtk::PLplot::PlotObject2DLine>(Gtk::ORIENTATION_HORIZONTAL, 
                            posy[i], Gdk::RGBA("grey"), Gtk::PLplot::LineStyle::SHORT_DASH_SHORT_GAP, 0.5));
    
    plot->add_object(*gridlinesy[i]);

    gridlinesx.emplace_back(Gtk::make_managed<Gtk::PLplot::PlotObject2DLine>(Gtk::ORIENTATION_VERTICAL, 
                            posx[i], Gdk::RGBA("grey"), Gtk::PLplot::LineStyle::SHORT_DASH_SHORT_GAP, 0.5));
    
    plot->add_object(*gridlinesx[i]);
  }
}

void GraphsWindow::init_plot()
{
  try
  {
    std::vector<double> x(2), y(2);
    auto plot_dummy = Gtk::make_managed<Gtk::PLplot::PlotData2D>(x, y, Gdk::RGBA("white"), Gtk::PLplot::LineStyle::CONTINUOUS, 2.0);
    plot = Gtk::make_managed<Gtk::PLplot::Plot2D>(*plot_dummy);
    canvas.add_plot(*plot);
    plot->remove_data(*plot_dummy);
  }
  catch(const Glib::Exception& e)
  {
    std::cerr << "gtkmm PLplot error encountered: " << e.what() << '\n';
  }
}

void::GraphsWindow::set_plot_theme()
{
  Gdk::RGBA colour;
  colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
  plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
  plot->set_region_selection_width(0.5);
  
  bool themeisdark = false;
  Glib::ustring themename = Gtk::Settings::get_default()->property_gtk_theme_name().get_value();
  size_t found = themename.lowercase().find("dark");

  if (Glib::ustring::npos != found) themeisdark = true;
  
  if (true == themeisdark || true == Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().get_value())
  {
    canvas.set_background_color(Gdk::RGBA("black"));
    plot->set_axes_color(Gdk::RGBA("white"));
    plot->set_titles_color(Gdk::RGBA("white"));
    plot->set_region_selection_color(Gdk::RGBA("white"));
    plot->set_legend_background_color(Gdk::RGBA("black"));
    plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
    
  }
  else
  {
    canvas.set_background_color(Gdk::RGBA("white"));
    plot->set_axes_color(Gdk::RGBA("black"));
    plot->set_titles_color(Gdk::RGBA("black"));
    plot->set_region_selection_color(Gdk::RGBA("black"));
    plot->set_legend_background_color(Gdk::RGBA("white"));
    plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
  }
}

void::GraphsWindow::set_plot_theme_by_name(const Glib::ustring& themename) 
{
  Gdk::RGBA colour;
  colour.set_rgba(18.0 / 255.0, 83.0 / 255.0, 158.0 / 255.0);
  plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
  plot->set_region_selection_width(0.5);
  
  
  if (_("Natural blend dark")  == themename) // adwaita dark
  {
    Gdk::RGBA colour;
    colour.set_rgba(53.0 / 255.0, 53.0 / 255.0, 53.0 / 255.0, 0.0);
    canvas.set_background_color(Gdk::RGBA(colour));
    plot->set_axes_color(Gdk::RGBA("white"));
    plot->set_titles_color(Gdk::RGBA("white"));
    plot->set_region_selection_color(Gdk::RGBA("white"));
    plot->set_legend_background_color(Gdk::RGBA(colour));
    plot->set_legend_bounding_box_color(Gdk::RGBA("white"));
    
  }
  else if(_("Natural blend")  == themename)
  {
    Gdk::RGBA colour;
    colour.set_rgba(240.0 / 255.0, 240.0 / 255.0, 24.0 / 255.0, 0.0);
    canvas.set_background_color(Gdk::RGBA(colour));
    plot->set_axes_color(Gdk::RGBA("black"));
    plot->set_titles_color(Gdk::RGBA("black"));
    plot->set_region_selection_color(Gdk::RGBA("black"));
    plot->set_legend_background_color(Gdk::RGBA(colour));
    plot->set_legend_bounding_box_color(Gdk::RGBA("black"));
  }
  else if(_("White on black")  == themename)
  {
    canvas.set_background_color(Gdk::RGBA("black"));
    plot->set_axes_color(Gdk::RGBA("white"));
    plot->set_titles_color(Gdk::RGBA("white"));
    plot->set_region_selection_color(Gdk::RGBA("white"));
    plot->set_legend_background_color(Gdk::RGBA("black"));
    plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
    
  }
  else if(_("Black on white")  == themename)
  {
    canvas.set_background_color(Gdk::RGBA("white"));
    plot->set_axes_color(Gdk::RGBA("black"));
    plot->set_titles_color(Gdk::RGBA("black"));
    plot->set_region_selection_color(Gdk::RGBA("black"));
    plot->set_legend_background_color(Gdk::RGBA("white"));
    plot->set_legend_bounding_box_color(Gdk::RGBA(colour));
  }
  else if(_("Default")  == themename)
  { 
    set_plot_theme();
  }
}

bool GraphsWindow::on_key_press_event(GdkEventKey* key_event)
{
  
  if(key_event->keyval == GDK_KEY_Escape)
  {
    hide();
    return true;
  }
  else if ((key_event->keyval == GDK_KEY_f) && (key_event->state & GDK_CONTROL_MASK))
  {
    search();
    return true;
  }
  else if ((key_event->keyval == GDK_KEY_z) && (key_event->state & GDK_CONTROL_MASK))
  {
    plot_data_changed();
    return true;
  }

  
  return Gtk::Window::on_key_press_event(key_event);
}

