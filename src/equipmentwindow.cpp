#include "equipmentwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm/i18n.h>
#include <iostream>
#include <iomanip>

BrowserWindow::BrowserWindow(const Glib::RefPtr<Gtk::TreeStore> &eptree_store, EpCombo::EpCombomodel& epmodel)
    : m_eptree_store(eptree_store),
      m_epmodel(epmodel)
{
    headerbar.set_show_title_buttons();
    headerbar.pack_start(switcher);
    headerlabel.set_markup(_("<b>Equipment Browser</b>\n<sub>View eyepiece and telescope details.</sub>"));
    headerbar.set_title_widget(headerlabel);
    set_titlebar(headerbar);
 
    set_hide_on_close(true);
    set_title(_("WIP - Browse eyepieces and telescopes"));
    set_size_request(900, 600);

    Uidefs::set_ui_spacing(grid);
    scrollview.set_margin(Uidefs::BORDER_WIDTH);
    epview.set_expand(true);
    epview.set_model(m_eptree_store);
    epview.set_grid_lines(Gtk::TreeView::GridLines::BOTH);

    epview.append_column(_("Brand"), m_epmodel.m_epcols.m_epbrand);
    epview.get_column(0)->set_resizable(true);
    epview.get_column(0)->set_max_width(250);
    epview.get_column(0)->set_expand(false);
    auto brandcell = epview.get_column(0)->get_first_cell();
    brandcell->set_padding(0, 5);

    epview.append_column(_("Model"), m_epmodel.m_epcols.m_epmodel);
    epview.get_column(1)->set_resizable(true);
    epview.get_column(1)->set_min_width(150);
    epview.get_column(1)->set_expand(false);
    auto modelcell = epview.get_column(1)->get_first_cell();
    modelcell->set_padding(0, 5);

    cells = std::vector<Gtk::CellRendererText>(9);
    cols = std::vector<Gtk::TreeView::Column>(9);

    for (size_t i{0}; i < 6; ++i)
    {
        cols[i].set_resizable(true);
        cols[i].set_expand(false);
        cells[i].set_padding(10, 5);
        epview.append_column(cols[i]);
    }

    cols[0].set_title(_("FOV"));
    epview.get_column(2)->pack_start(cells[0]);
    epview.get_column(2)->add_attribute(cells[0].property_markup(), m_epmodel.m_epcols.m_epfov);
    epview.get_column(2)->set_cell_data_func(cells[0], sigc::mem_fun(*this, &BrowserWindow::on_ep_fov_data));

    cols[1].set_title(_("Focal length"));
    epview.get_column(3)->pack_start(cells[1]);
    epview.get_column(3)->set_cell_data_func(cells[1], sigc::mem_fun(*this, &BrowserWindow::on_ep_flen_data));

    cols[2].set_title(_("Field stop"));
    epview.get_column(4)->pack_start(cells[2]);
    epview.get_column(4)->set_cell_data_func(cells[2], sigc::mem_fun(*this, &BrowserWindow::on_ep_fstop_data));

    cols[3].set_title(_("Eye relief"));
    epview.get_column(5)->pack_start(cells[3]);
    epview.get_column(5)->set_cell_data_func(cells[3], sigc::mem_fun(*this, &BrowserWindow::on_ep_relief_data));

    cols[4].set_title(_("Trans"));
    epview.get_column(6)->pack_start(cells[4]);
    epview.get_column(6)->set_cell_data_func(cells[4], sigc::mem_fun(*this, &BrowserWindow::on_ep_trans_data));

    cols[5].set_title(_("Barrel"));
    epview.get_column(7)->pack_start(cells[5]);
    epview.get_column(7)->set_cell_data_func(cells[5], sigc::mem_fun(*this, &BrowserWindow::on_ep_barrel_data));


    epview.append_column(_("Type"), m_epmodel.m_epcols.m_eptype);
    epview.get_column(8)->set_resizable(true);
    epview.get_column(8)->set_min_width(150);
    epview.get_column(8)->set_expand(false);
    auto typecell = epview.get_column(8)->get_first_cell();
    typecell->set_padding(10, 5);

    for (size_t i{6}; i < 9; ++i)
    {
        cols[i].set_resizable(true);
        cols[i].set_expand(false);
        cells[i].set_padding(10, 5);
        epview.append_column(cols[i]);
    }

    cols[6].set_title(_("Groups"));
    epview.get_column(9)->pack_start(cells[6]);
    epview.get_column(9)->set_cell_data_func(cells[6], sigc::mem_fun(*this, &BrowserWindow::on_ep_group_data));

    cols[7].set_title(_("Elements"));
    epview.get_column(10)->pack_start(cells[7]);
    epview.get_column(10)->set_cell_data_func(cells[7], sigc::mem_fun(*this, &BrowserWindow::on_ep_element_data));

    cols[8].set_title(_("Weight"));
    epview.get_column(11)->pack_start(cells[8]);
    epview.get_column(11)->set_cell_data_func(cells[8], sigc::mem_fun(*this, &BrowserWindow::on_ep_weight_data));

    epview.append_column(_("Coating"), m_epmodel.m_epcols.m_epcoating);
    epview.get_column(12)->set_resizable(true);
    epview.get_column(12)->set_min_width(150);
    epview.get_column(12)->set_expand(false);
    auto coatcell = epview.get_column(12)->get_first_cell();
    coatcell->set_padding(10, 5);

    epview.append_column(_("Material"), m_epmodel.m_epcols.m_epmaterial);
    epview.get_column(13)->set_resizable(true);
    epview.get_column(13)->set_min_width(150);
    epview.get_column(13)->set_expand(false);
    auto materialcell = epview.get_column(13)->get_first_cell();
    materialcell->set_padding(10, 5);

    scrollview.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scrollview.set_expand(true);
    scrollview.set_child(epview);
   // epview.expand_all();
   // epview.get_column(0)->set_visible(false);

    auto controller = Gtk::EventControllerKey::create();
    controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    controller->signal_key_pressed().connect(
        sigc::bind(sigc::mem_fun(*this, &BrowserWindow::on_key_press_event), "capture"), false);
    add_controller(controller);     

    tmplabel.set_text("TODO WIP");
    grid.attach(tmplabel, 0, 0);
    stack.add(scrollview, "Eyepieces", _("Eyepieces"));
    stack.add(grid, "Telescopes", _("Tellescopes"));

    stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);
    switcher.set_hexpand(false);
    stack.set_vexpand(true);
    stack.set_hexpand(true);
    switcher.set_stack(stack);

    set_child(stack);
}

bool BrowserWindow::on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring &)
{
    if (keyval == GDK_KEY_Escape)
        hide();

    return false;
}

void BrowserWindow::on_ep_fov_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[0].property_text() = "";
    else
        cells[0].property_markup() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_epmodel.m_epcols.m_epfov))
        + "<sup> o</sup>";
}

void BrowserWindow::on_ep_flen_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[1].property_text() = "";
    else
        cells[1].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_epmodel.m_epcols.m_epflen))
        + _(" mm");
}

void BrowserWindow::on_ep_fstop_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[2].property_text() = "";
    else
    {
        double fstop = row.get_value(m_epmodel.m_epcols.m_epfstop);
        if (fstop < 1e-09) cells[2].property_text() = _("n/a");
        else cells[2].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), fstop) + _(" mm");
    }
}

void BrowserWindow::on_ep_relief_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[3].property_text() = "";
    else
    {
        double relief = row.get_value(m_epmodel.m_epcols.m_eprelief);
        if (relief < 1e-09) cells[3].property_text() = _("n/a");
        else cells[3].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), relief) + _(" mm");
    }
}

void BrowserWindow::on_ep_trans_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[4].property_text() = "";
    else
    {
        double trans = row.get_value(m_epmodel.m_epcols.m_eptrans);
        if (trans < 1e-09) cells[4].property_text() = _("n/a");
        else cells[4].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), trans) + _("%");
    }
}

void BrowserWindow::on_ep_barrel_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[5].property_text() = "";
    else
    {
        double barrel = row.get_value(m_epmodel.m_epcols.m_epbarrel);
        if (barrel < 1e-09) cells[5].property_text() = _("n/a");
        else cells[5].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), barrel) + _("\"");
    }
}

void BrowserWindow::on_ep_group_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[6].property_text() = "";
    else
    {
        int group = row.get_value(m_epmodel.m_epcols.m_epgroups);
        if (!group) cells[6].property_text() = _("n/a");
        else cells[6].property_text() = Glib::ustring::format(group);
    }
}

void BrowserWindow::on_ep_element_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[7].property_text() = "";
    else
    {
        int elements = row.get_value(m_epmodel.m_epcols.m_epelements);
        if (!elements) cells[7].property_text() = _("n/a");
        else cells[7].property_text() = Glib::ustring::format(elements);
    }
}

void BrowserWindow::on_ep_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        cells[8].property_text() = "";
    else
    {
        double weight = row.get_value(m_epmodel.m_epcols.m_epweight);
        if (weight < 1e-09) cells[8].property_text() = _("n/a");
        else cells[8].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), weight) + _("g");
    }
}