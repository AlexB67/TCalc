#include "plotter.hpp"

Gtk::Grid &CGraph::CairoGraph::create_graph()
{
    set_hexpand(true);
    set_vexpand(true);
    set_size_request(256, 256); // give it something to begin with

    xvalue->set_can_focus(false);
    yvalue->set_can_focus(false);
    xvalue->set_hexpand(true);
    yvalue->set_hexpand(true);
    xvalue->set_editable(false);
    yvalue->set_editable(false);
    // xvalue->set_sensitive(false);
    // yvalue->set_sensitive(false);

    xvaluelabel->set_halign(Gtk::Align::END);
    yvaluelabel->set_halign(Gtk::Align::END);
    xvaluelabel->set_markup(_("<i>X</i>  = "));
    yvaluelabel->set_halign(Gtk::Align::END);
    yvaluelabel->set_markup(_("<i>Y</i>  = "));

    grid->set_row_spacing(20);
    grid->set_column_spacing(20);
    grid->attach(*this, 0, 0, 4, 1);
    grid->attach(*xvaluelabel, 0, 1);
    grid->attach(*xvalue, 1, 1);
    grid->attach(*yvaluelabel, 2, 1);
    grid->attach(*yvalue, 3, 1);

    return *grid;
}

Gtk::Grid &CGraph::CairoGraph::get_motion_tracker()
{
    xvalue->set_width_chars(12);
    yvalue->set_width_chars(12);
    cursor_grid->attach(*xvaluelabel, 0, 0);
    cursor_grid->attach(*xvalue, 1, 0);
    cursor_grid->attach(*yvaluelabel, 2, 0);
    cursor_grid->attach(*yvalue, 3, 0);
    cursor_grid->set_can_target(false);

    return *cursor_grid;
}