#include "graphswindow.hpp"
#include "glibmmcustomutils.hpp"
#include "astrocalclib/astrocalc.hpp"
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
  headerbar.pack_end(*showgraphlegend);
  headerbar.pack_start(searchbutton);
  set_titlebar(headerbar);
  
  epbox = std::make_shared<EpBox::Eyepiecebox>();
  scopebox = std::make_shared<ScopeBox::Telescopebox>();
  magbox = std::make_unique<MagBox::Magbox>();
  optionsbox = std::make_unique<OptionsBox::Optionsbox>();
  graphbox = std::make_unique<CGraph::CairoGraph>();
  scopebox->frame_can_expand(true);

  plotframe.set_label_widget(plotframe_label);
  plotframe_label.set_markup(_("<b>Graph</b>"));
  plotframe.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
  plotframe.set_border_width(Uidefs::BORDER_WIDTH);
  AppGlobals::get_keyfile_config(plotframe);
  AppGlobals::frame_style.connect([this](Gtk::ShadowType type){ AppGlobals::set_frame_style(plotframe, type);});

  plotlist.insert(0, _("Magnitude limit versus magnification"));
  plotlist.insert(1, _("Magnitude limit versus zenith angle"));
  plotlist.insert(2, _("Magnitude limit versus aperture"));
  plotlist.insert(3, _("Resolving power versus aperture"));
  plotlist.insert(4, _("Light gathering versus aperture"));
  plotlist.insert(5, _("DSO visbility"));
  plotlist.insert(6, _("Lunar resolution"));
  plotlist.insert(7, _("Abberation free field versus aperture"));
  plotlist.insert(8, _("Brightness factor"));
  plotlist.set_active(0);
  plotlist.set_hexpand(true);

  showgraphlegend->set_halign(Gtk::ALIGN_CENTER);
  showgraphlegend->set_valign(Gtk::ALIGN_CENTER);
  
  graphbox->get_cursor_grid().set_column_spacing(Uidefs::COLUMN_PADDING);
  graphbox->set_vexpand(true);

  graphbox->get_xvalue_label().set_width_chars(6);
  graphbox->get_yvalue_label().set_width_chars(6);
  
  plotgrid.attach(*graphbox, 0, 0, 3, 1);
  plotgrid.attach(plotlabel, 0, 1);
  plotgrid.attach(plotlist, 1, 1);
  plotgrid.attach(graphbox->get_motion_tracker(), 2, 1);

  plotframe.add(plotgrid);

  controlsgrid.attach(epbox->create_eyepiece_grid(), 0, 0);
  controlsgrid.attach(scopebox->create_telescope_grid(), 0, 1);

  controlsgrid2.attach(magbox->create_mag_grid(), 0, 0);
  controlsgrid2.attach(optionsbox->create_options_grid(), 0, 1);

  windowgrid.attach(plotframe, 0, 0, 6, 6);
  windowgrid.attach(controlsgrid, 6, 0);
  windowgrid.attach(controlsgrid2, 7, 0);

  Uidefs::set_ui_spacing<Gtk::Grid>(plotgrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid2);

  init_plot();

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

  std::vector<Glib::ustring> themes
  {
    "Fade to black", "Black", "Herculean blue", "Midnight blue",
    "Dark", "Default"
  };

  graphtheme = themes[index];
  graphbox->set_theme(graphtheme);
}

void GraphsWindow::init_plot()
{
  try
  {
    std::vector<double> x(4), y(4);
    for ( size_t i = 0; i < x.size(); ++i) 
    {
      x[i] = static_cast<double>(i);
      y[i] = x[i];
    }

    Gdk::RGBA linecolour; linecolour.set_rgba(0.0, 1.0, 1.0, 1.0);
    graphbox->add_series(x, y, linecolour, CairoGraphLineStyle::SOLID_LINE);
  }
  catch(const Glib::Exception& e)
  {
    std::cerr << "Plotlib error encountered: " << e.what() << '\n';
  }
}

void::GraphsWindow::set_plot_theme(const Glib::ustring &themename)
{
  graphtheme = themename;
  graphbox->set_theme(themename);
  plot_data_changed();
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
