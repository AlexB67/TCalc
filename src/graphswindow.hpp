#pragma once
#include <gtkmm/window.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/settings.h>
#include <gtkmm/switch.h>
#include "plotlib/plotter.hpp"
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
    void set_plot_theme(const Glib::ustring& themename);
    
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
    Gtk::Label plotlabel{_("Select graph :")};
    Glib::ustring graphtheme = "Default";
    Gtk::ComboBoxText   plotlist;
    std::unique_ptr<CGraph::CairoGraph> graphbox;
    std::shared_ptr<EpBox::Eyepiecebox>     epbox;
    std::shared_ptr<ScopeBox::Telescopebox> scopebox;
    std::unique_ptr<MagBox::Magbox>         magbox;
    std::unique_ptr<OptionsBox::Optionsbox> optionsbox;
    std::shared_ptr<LogView::LoggerView>    logbox;
    std::unique_ptr<SearchWindow> searchwindow;
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
        &GraphsWindow::plot8,
        &GraphsWindow::plot9,
    };

    void plot1();
    void plot2();
    void plot3();
    void plot4();
    void plot5();
    void plot6();
    void plot7();
    void plot8();
    void plot9();

    void init_plot();
    void set_signal_handlers();
    bool on_key_press_event(GdkEventKey* event) override;
    void plot_data_changed();
    void search();
};
