#pragma once
#include <gtkmm-plplot/canvas.h>
#include <gtkmm-plplot/plot2d.h>
#include <gtkmm-plplot/plotobject2dtext.h>
#include <gtkmm-plplot/plotobject2dline.h>
#include <gtkmm/window.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/settings.h>
#include <gtkmm/switch.h>
#include "eyepiecebox.hpp"
#include "telescopebox.hpp"
#include "starmagbox.hpp"
#include "loggerbox.hpp"
#include "optionsbox.hpp"
#include "searchwindow.hpp"

class GraphsWindow : public Gtk::Window
{
public:
    explicit GraphsWindow();
    virtual ~GraphsWindow(){}
    GraphsWindow(const GraphsWindow&) = delete;
    GraphsWindow(GraphsWindow&& ) = delete;
    GraphsWindow& operator=(const GraphsWindow& other) = delete;
    GraphsWindow& operator=(GraphsWindow&& other) = delete;
    void set_plot_theme();
    void set_plot_theme_by_name(const Glib::ustring& themename);
    
private:
    Gtk::HeaderBar  headerbar;
    Gtk::Button searchbutton;
    Gtk::Switch *showgraphlegend;
    Gtk::Grid windowgrid;
    Gtk::Grid plotgrid;
    Gtk::Grid controlsgrid;
    Gtk::Grid controlsgrid2;
    Gtk::Frame plotframe{_("Graph")};
    Gtk::Label plotframe_label{};
    Gtk::Label plotlabel{_("Select plot:")};
    Gtk::Label xvallabel;
    Gtk::Label yvallabel;
    Gtk::Entry xval;
    Gtk::Entry yval;
    Glib::ustring xunit;
    Glib::ustring yunit;
    Glib::ustring graphtheme = "Default";
    Gtk::ComboBoxText   plotlist;
    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::Plot2D *plot = nullptr;
    std::shared_ptr<EpBox::Eyepiecebox>     epbox;
    std::shared_ptr<ScopeBox::Telescopebox> scopebox;
    std::unique_ptr<MagBox::Magbox>         magbox;
    std::unique_ptr<OptionsBox::Optionsbox> optionsbox;
    std::shared_ptr<LogView::LoggerView>    logbox;
    std::unique_ptr<SearchWindow> searchwindow;
    void create_gridlines(const double xmin, const double xmax, const double ymin, const double ymax) const;
    void get_config();
    
    const std::vector<std::function<void(GraphsWindow &)> > graphlist = 
    {
        &GraphsWindow::plot1,
        &GraphsWindow::plot2,
        &GraphsWindow::plot3,
        &GraphsWindow::plot4,
        &GraphsWindow::plot5,
        &GraphsWindow::plot6,
        &GraphsWindow::plot7,
    };

    void plot1();
    void plot2();
    void plot3();
    void plot4();
    void plot5();
    void plot6();
    void plot7();

    void init_plot();
    void set_signal_handlers();
    bool on_key_press_event(GdkEventKey* event) override;
    void plot_data_changed();
    void search();
    void track_cursor(double x, double y);
};
