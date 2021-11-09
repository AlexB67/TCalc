#include "browse_equipwindow.hpp"
#include "astrocalclib/astrocalc.hpp"
#include <glibmm/i18n.h>
#include <iostream>
#include <iomanip>

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
        ep_cells[0].property_text() = "";
    else
        ep_cells[0].property_markup() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_epmodel.m_epcols.m_epfov))
        + "<sup> o</sup>";
}

void BrowserWindow::on_ep_flen_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[1].property_text() = "";
    else
        ep_cells[1].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_epmodel.m_epcols.m_epflen))
        + _(" mm");
}

void BrowserWindow::on_ep_fstop_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[2].property_text() = "";
    else
    {
        double fstop = row.get_value(m_epmodel.m_epcols.m_epfstop);
        if (fstop < 1e-09) ep_cells[2].property_text() = _("");
        else ep_cells[2].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), fstop) + _(" mm");
    }
}

void BrowserWindow::on_ep_relief_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[3].property_text() = "";
    else
    {
        double relief = row.get_value(m_epmodel.m_epcols.m_eprelief);
        if (relief < 1e-09) ep_cells[3].property_text() = _("");
        else ep_cells[3].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), relief) + _(" mm");
    }
}

void BrowserWindow::on_ep_trans_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[4].property_text() = "";
    else
    {
        double trans = row.get_value(m_epmodel.m_epcols.m_eptrans);
        if (trans < 1e-09) ep_cells[4].property_text() = _("");
        else ep_cells[4].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), trans) + _("%");
    }
}

void BrowserWindow::on_ep_barrel_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[5].property_text() = "";
    else
    {
        double barrel = row.get_value(m_epmodel.m_epcols.m_epbarrel);
        if (barrel < 1e-09) ep_cells[5].property_text() = _("");
        else ep_cells[5].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), barrel) + _("\"");
    }
}

void BrowserWindow::on_ep_group_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[6].property_text() = "";
    else
    {
        int group = row.get_value(m_epmodel.m_epcols.m_epgroups);
        if (!group) ep_cells[6].property_text() = _("");
        else ep_cells[6].property_text() = Glib::ustring::format(group);
    }
}

void BrowserWindow::on_ep_element_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[7].property_text() = "";
    else
    {
        int elements = row.get_value(m_epmodel.m_epcols.m_epelements);
        if (!elements) ep_cells[7].property_text() = _("");
        else ep_cells[7].property_text() = Glib::ustring::format(elements);
    }
}

void BrowserWindow::on_ep_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_epmodel.m_epcols.m_epbrand).length())
        ep_cells[8].property_text() = "";
    else
    {
        double weight = row.get_value(m_epmodel.m_epcols.m_epweight);
        if (weight < 1e-09) ep_cells[8].property_text() = _("");
        else ep_cells[8].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), weight) + _("g");
    }
}

// bool BrowserWindow::on_show_visible(const Gtk::TreeModel::const_iterator& iter)
// {
//     if (!ep_filter_entry.get_text_length())
//         return true;
//     if (iter)
//     {
//         auto piter = iter->parent();
//         if (iter->get_value(m_epmodel.m_epcols.m_epbrand).length()) return true;
        
//         // if (parent_iter)
//         // {
//             auto row = *iter;
//             size_t found = row.get_value(m_epmodel.m_epcols.m_epbrand).lowercase().find(ep_filter_entry.get_text());
//             if (found != Glib::ustring::npos)
//             {
//                 std::clog << row.get_value(m_epmodel.m_epcols.m_epmodel) << "\n";
//                 return true;
//             }
//         // 1}
//     }

//     return false;
// }
void BrowserWindow::on_scope_aperture_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;
    
    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[0].property_text() = "";
    else
        scope_cells[0].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_scopemodel.m_scopecols.m_saperture))
        + _(" mm");
}

void BrowserWindow::on_scope_flen_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[1].property_text() = "";
    else
        scope_cells[1].property_text() = 
        Glib::ustring::format(std::fixed, std::setprecision(2), row.get_value(m_scopemodel.m_scopecols.m_saperture))
        + _(" mm");
}

void BrowserWindow::on_scope_trans_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[2].property_text() = "";
    else
    {
        double trans_reflect = row.get_value(m_scopemodel.m_scopecols.m_sreflect);

        if (trans_reflect < 1e-09)
            scope_cells[2].property_text() = ""; 
        else
            scope_cells[2].property_text() = 
            Glib::ustring::format(std::fixed, std::setprecision(2), trans_reflect) + _("%");
    }
}

void BrowserWindow::on_scope_obstruct_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[3].property_text() = "";
    else
    {
        double obstruct = row.get_value(m_scopemodel.m_scopecols.m_sobstruct);

        if (obstruct < 1e-09)
            scope_cells[3].property_text() = "";
        else if (row.get_value(m_scopemodel.m_scopecols.m_stype) == Astrocalc::astrocalc::REFRACTOR)
            scope_cells[3].property_text() = "";
        else
            scope_cells[3].property_text() = 
            Glib::ustring::format(std::fixed, std::setprecision(2), obstruct) + _("%");
    }
}

void BrowserWindow::on_scope_type_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[4].property_text() = "";
    else
    {
        int type = row.get_value(m_scopemodel.m_scopecols.m_stype);

        if (type == Astrocalc::astrocalc::REFLECTOR)
            scope_cells[4].property_text() = "Reflector";
        else if (row.get_value(m_scopemodel.m_scopecols.m_stype) == Astrocalc::astrocalc::REFRACTOR)
            scope_cells[4].property_text() = "Refractor";
        else if (row.get_value(m_scopemodel.m_scopecols.m_stype) == Astrocalc::astrocalc::SCTMAK)
            scope_cells[4].property_text() = "Mak/SCT";
        else
            scope_cells[4].property_text() = "";
    }
}

void BrowserWindow::on_scope_strehl_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[5].property_text() = "";
    else
    {
        double strehl = row.get_value(m_scopemodel.m_scopecols.m_sstrehl);

        if (strehl > Astrocalc::astrocalc::tSMALL)
            scope_cells[5].property_text() = Glib::ustring::format(std::fixed, std::setprecision(2), strehl);
        else
            scope_cells[5].property_text() = "";
    }
}

void BrowserWindow::on_scope_total_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[6].property_text() = "";
    else
    {
        double weight = row.get_value(m_scopemodel.m_scopecols.m_sweight);

        if (weight > Astrocalc::astrocalc::tSMALL)
            scope_cells[6].property_text() = Glib::ustring::format(std::fixed, std::setprecision(2), weight)
                                         + (_(" kg"));
        else
            scope_cells[6].property_text() = "";
    }
}

void BrowserWindow::on_scope_tube_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator iter)
{
    if (!iter) return;
    const auto row = *iter;

    if (row.get_value(m_scopemodel.m_scopecols.m_sbrand).length())
        scope_cells[7].property_text() = "";
    else
    {
        double weight = row.get_value(m_scopemodel.m_scopecols.m_stube_weight);

        if (weight > Astrocalc::astrocalc::tSMALL)
            scope_cells[7].property_text() = Glib::ustring::format(std::fixed, std::setprecision(2), weight)
                                         + (_(" kg"));
        else
            scope_cells[7].property_text() = "";
    }
}



void BrowserWindow::setup_signal_handlers()
{   // eyepiece page
    ep_expand_tree.property_active().signal_changed().connect((sigc::track_obj([this]()
    {   if (ep_expand_tree.get_active()) epview.expand_all();
        else 
        {
            epview.scroll_to_column(*epview.get_column(0));
            epview.collapse_all();
        }
    }, *this)));

    ep_show_model.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(1)->set_visible(ep_show_model.get_active());}, *this)));

    ep_show_fov.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(2)->set_visible(ep_show_fov.get_active());}, *this)));

    ep_show_flen.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(3)->set_visible(ep_show_flen.get_active());}, *this)));

    ep_show_fstop.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(4)->set_visible(ep_show_fstop.get_active());}, *this)));

    ep_show_relief.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(5)->set_visible(ep_show_relief.get_active());}, *this)));

    ep_show_trans.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(6)->set_visible(ep_show_trans.get_active());}, *this)));

    ep_show_barrel.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(7)->set_visible(ep_show_barrel.get_active());}, *this)));

    ep_show_type.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(8)->set_visible(ep_show_type.get_active());}, *this)));

    ep_show_groups.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(9)->set_visible(ep_show_groups.get_active());}, *this)));

    ep_show_elements.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(10)->set_visible(ep_show_elements.get_active());}, *this)));

    ep_show_weight.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(11)->set_visible(ep_show_weight.get_active());}, *this)));

    ep_show_coat.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(12)->set_visible(ep_show_coat.get_active());}, *this)));

    ep_show_material.property_active().signal_changed().connect((sigc::track_obj([this]()
    { epview.get_column(13)->set_visible(ep_show_material.get_active());}, *this)));
    
    // scope page
    scope_expand_tree.property_active().signal_changed().connect((sigc::track_obj([this]()
    {   if (scope_expand_tree.get_active()) scopeview.expand_all();
        else
        { 
            scopeview.scroll_to_column(*scopeview.get_column(0));
            scopeview.collapse_all(); 
        }
    }, *this)));

    scope_show_model.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(1)->set_visible(scope_show_model.get_active());}, *this)));

    scope_show_aperture.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(2)->set_visible(scope_show_aperture.get_active());}, *this)));

    scope_show_flen.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(3)->set_visible(scope_show_flen.get_active());}, *this)));

    scope_show_trans.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(4)->set_visible(scope_show_trans.get_active());}, *this)));

    scope_show_obstruct.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(5)->set_visible(scope_show_obstruct.get_active());}, *this)));

    scope_show_type.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(6)->set_visible(scope_show_type.get_active());}, *this)));

    scope_show_mirror_coat.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(7)->set_visible(scope_show_mirror_coat.get_active());}, *this)));

    scope_show_mirror_material.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(8)->set_visible(scope_show_mirror_material.get_active());}, *this)));

    scope_show_lens_coat.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(9)->set_visible(scope_show_lens_coat.get_active());}, *this)));

    scope_show_lens_material.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(10)->set_visible(scope_show_lens_material.get_active());}, *this)));

    scope_show_strehl.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(11)->set_visible(scope_show_strehl.get_active());}, *this)));

    scope_show_total_weight.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(12)->set_visible(scope_show_total_weight.get_active());}, *this)));

    scope_show_tube_weight.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(13)->set_visible(scope_show_tube_weight.get_active());}, *this)));

    scope_show_mount.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(14)->set_visible(scope_show_mount.get_active());}, *this)));

    scope_show_focuser.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(15)->set_visible(scope_show_focuser.get_active());}, *this)));

    scope_show_finder.property_active().signal_changed().connect((sigc::track_obj([this]()
    { scopeview.get_column(16)->set_visible(scope_show_finder.get_active());}, *this)));
}