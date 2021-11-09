#include "browse_equipwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm/i18n.h>
#include <iostream>
#include <iomanip>

BrowserWindow::BrowserWindow(EpCombo::EpCombomodel& epmodel,
                             ScopeCombo::ScopeCombomodel& scopemodel)
    : m_epmodel(epmodel),
      m_scopemodel(scopemodel)
{
    set_size_request(1024, 768);
    headerbar.set_show_title_buttons();
    headerbar.pack_start(switcher);
    headerlabel.set_markup(_("<b>Equipment Browser</b>\n<sub>View eyepiece and telescope details.</sub>"));
    headerbar.set_title_widget(headerlabel);
    set_titlebar(headerbar);
    set_hide_on_close(true);
    
    make_eyepiece_view();
    make_telescope_view();

    setup_signal_handlers();

    auto controller = Gtk::EventControllerKey::create();
    controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    controller->signal_key_pressed().connect(
        sigc::bind(sigc::mem_fun(*this, &BrowserWindow::on_key_press_event), "capture"), false);
    add_controller(controller);     

   // stack.add(grid, "Telescopes", _("Tellescopes"));
    stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);
    switcher.set_hexpand(false);
    stack.set_vexpand(true);
    stack.set_hexpand(true);
    switcher.set_stack(stack);

    set_child(stack);
}

void BrowserWindow::make_eyepiece_view()
{
    Uidefs::set_ui_spacing(ep_grid);
    epview.set_expand(true);

    //m_eptree_filter = Gtk::TreeModelFilter::create(m_eptree_store, Gtk::TreeModel::Path());
    m_eptree_store = m_epmodel.get_epmodel();

    epview.set_model(m_eptree_store);
    epview.set_grid_lines(Gtk::TreeView::GridLines::BOTH);

    epview.append_column(_("Brand"), m_epmodel.m_epcols.m_epbrand);
    epview.get_column(0)->set_resizable(true);
    epview.get_column(0)->set_expand(false);
    epview.set_expander_column(*epview.get_column(0));
    epview.get_column(0)->get_first_cell()->set_padding(0, 5);

    epview.append_column(_("Model"), m_epmodel.m_epcols.m_epmodel);
    epview.get_column(1)->set_resizable(true);
    epview.get_column(1)->set_expand(false);
    epview.get_column(1)->get_first_cell()->set_padding(5, 5);

    ep_cells = std::vector<Gtk::CellRendererText>(9);
    ep_cols = std::vector<Gtk::TreeView::Column>(9);

    for (size_t i{0}; i < 6; ++i)
    {
        ep_cols[i].set_resizable(true);
        ep_cols[i].set_expand(false);
        ep_cells[i].set_padding(10, 5);
        epview.append_column(ep_cols[i]);
    }

    ep_cols[0].set_title(_("FOV"));
    epview.get_column(2)->pack_start(ep_cells[0]);
    epview.get_column(2)->add_attribute(ep_cells[0].property_markup(), m_epmodel.m_epcols.m_epfov);
    epview.get_column(2)->set_cell_data_func(ep_cells[0], sigc::mem_fun(*this, &BrowserWindow::on_ep_fov_data));

    ep_cols[1].set_title(_("Focal length"));
    epview.get_column(3)->pack_start(ep_cells[1]);
    epview.get_column(3)->set_cell_data_func(ep_cells[1], sigc::mem_fun(*this, &BrowserWindow::on_ep_flen_data));

    ep_cols[2].set_title(_("Field stop"));
    epview.get_column(4)->pack_start(ep_cells[2]);
    epview.get_column(4)->set_cell_data_func(ep_cells[2], sigc::mem_fun(*this, &BrowserWindow::on_ep_fstop_data));

    ep_cols[3].set_title(_("Eye relief"));
    epview.get_column(5)->pack_start(ep_cells[3]);
    epview.get_column(5)->set_cell_data_func(ep_cells[3], sigc::mem_fun(*this, &BrowserWindow::on_ep_relief_data));

    ep_cols[4].set_title(_("Trans"));
    epview.get_column(6)->pack_start(ep_cells[4]);
    epview.get_column(6)->set_cell_data_func(ep_cells[4], sigc::mem_fun(*this, &BrowserWindow::on_ep_trans_data));

    ep_cols[5].set_title(_("Barrel"));
    epview.get_column(7)->pack_start(ep_cells[5]);
    epview.get_column(7)->set_cell_data_func(ep_cells[5], sigc::mem_fun(*this, &BrowserWindow::on_ep_barrel_data));

    epview.append_column(_("Type"), m_epmodel.m_epcols.m_eptype);
    epview.get_column(8)->set_resizable(true);
    epview.get_column(8)->set_expand(false);
    epview.get_column(8)->get_first_cell()->set_padding(10, 5);

    for (size_t i{6}; i < 9; ++i)
    {
        ep_cols[i].set_resizable(true);
        ep_cols[i].set_expand(false);
        ep_cells[i].set_padding(10, 5);
        epview.append_column(ep_cols[i]);
    }

    ep_cols[6].set_title(_("Groups"));
    epview.get_column(9)->pack_start(ep_cells[6]);
    epview.get_column(9)->set_cell_data_func(ep_cells[6], sigc::mem_fun(*this, &BrowserWindow::on_ep_group_data));

    ep_cols[7].set_title(_("Elements"));
    epview.get_column(10)->pack_start(ep_cells[7]);
    epview.get_column(10)->set_cell_data_func(ep_cells[7], sigc::mem_fun(*this, &BrowserWindow::on_ep_element_data));

    ep_cols[8].set_title(_("Weight"));
    epview.get_column(11)->pack_start(ep_cells[8]);
    epview.get_column(11)->set_cell_data_func(ep_cells[8], sigc::mem_fun(*this, &BrowserWindow::on_ep_weight_data));

    epview.append_column(_("Coating"), m_epmodel.m_epcols.m_epcoating);
    epview.get_column(12)->set_resizable(true);
    epview.get_column(12)->set_expand(false);
    auto coatcell = epview.get_column(12)->get_first_cell();
    coatcell->set_padding(10, 5);

    epview.append_column(_("Material"), m_epmodel.m_epcols.m_epmaterial);
    epview.get_column(13)->set_resizable(true);
    epview.get_column(13)->set_expand(false);
    epview.get_column(13)->get_first_cell()->set_padding(10, 5);

    epscrollview.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    epscrollview.set_expand(true);
    epscrollview.set_child(epview);

    ep_show_expand_label.set_halign(Gtk::Align::START);
    ep_show_fov_label.set_halign(Gtk::Align::START);
    ep_expand_tree.set_halign(Gtk::Align::START);
    ep_show_fov.set_halign(Gtk::Align::START);
    ep_show_flen_label.set_halign(Gtk::Align::START);
    ep_show_fstop_label.set_halign(Gtk::Align::START);
    ep_show_relief_label.set_halign(Gtk::Align::START);
    ep_show_trans_label.set_halign(Gtk::Align::START);
    ep_show_barrel_label.set_halign(Gtk::Align::START);
    ep_show_type_label.set_halign(Gtk::Align::START);
    ep_show_groups_label.set_halign(Gtk::Align::START);
    ep_show_elements_label.set_halign(Gtk::Align::START);
    ep_show_weight_label.set_halign(Gtk::Align::START);
    ep_show_coat_label.set_halign(Gtk::Align::START);
    ep_show_material_label.set_halign(Gtk::Align::START);
    ep_show_model_label.set_halign(Gtk::Align::START);

    ep_show_expand_label.set_text(_("Show all"));
    ep_show_fov_label.set_text(_("FOV"));
    ep_show_flen_label.set_text(_("Focal length"));
    ep_show_fstop_label.set_text(_("Field stop"));
    ep_show_relief_label.set_text(_("Eye relief"));
    ep_show_trans_label.set_text(_("Transmission"));
    ep_show_barrel_label.set_text(_("Barrel size"));
    ep_show_type_label.set_text(_("Type"));
    ep_show_groups_label.set_text(_("Groups"));
    ep_show_elements_label.set_text(_("Elements"));
    ep_show_weight_label.set_text(_("Weight"));
    ep_show_coat_label.set_text(_("Coating"));
    ep_show_material_label.set_text(_("Material"));
    ep_show_model_label.set_text(_("Model"));

    ep_expand_tree.set_active(false);
    ep_show_fov.set_active(true);
    ep_show_flen.set_active(true);
    ep_show_fstop.set_active(true);
    ep_show_relief.set_active(true);
    ep_show_trans.set_active(true);
    ep_show_barrel.set_active(true);
    ep_show_type.set_active(true);
    ep_show_groups.set_active(true);
    ep_show_elements.set_active(true);
    ep_show_weight.set_active(true);
    ep_show_coat.set_active(true);
    ep_show_material.set_active(true);
    ep_show_model.set_active(true);

    ep_grid.set_column_homogeneous(true);
    ep_grid.attach(epscrollview, 0, 0, 14, 1);
    ep_grid.attach(ep_show_expand_label, 0, 1, 1, 1);
    ep_grid.attach(ep_expand_tree, 1, 1, 1, 1);
    ep_grid.attach(ep_show_fov_label, 2, 1, 1, 1);
    ep_grid.attach(ep_show_fov, 3, 1, 1, 1);
    ep_grid.attach(ep_show_flen_label, 4, 1, 1, 1);
    ep_grid.attach(ep_show_flen, 5, 1, 1, 1);
    ep_grid.attach(ep_show_fstop_label, 6, 1, 1, 1);
    ep_grid.attach(ep_show_fstop, 7, 1, 1, 1);
    ep_grid.attach(ep_show_relief_label, 8, 1, 1, 1);
    ep_grid.attach(ep_show_relief, 9, 1, 1, 1);
    ep_grid.attach(ep_show_trans_label, 10, 1, 1, 1);
    ep_grid.attach(ep_show_trans, 11, 1, 1, 1);
    ep_grid.attach(ep_show_barrel_label, 12, 1, 1, 1);
    ep_grid.attach(ep_show_barrel, 13, 1, 1, 1);
    ep_grid.attach(ep_show_type_label, 0, 2, 1, 1);
    ep_grid.attach(ep_show_type, 1, 2, 1, 1);
    ep_grid.attach(ep_show_groups_label, 2, 2, 1, 1);
    ep_grid.attach(ep_show_groups, 3, 2, 1, 1);
    ep_grid.attach(ep_show_elements_label, 4, 2, 1, 1);
    ep_grid.attach(ep_show_elements, 5, 2, 1, 1);
    ep_grid.attach(ep_show_weight_label, 6, 2, 1, 1);
    ep_grid.attach(ep_show_weight, 7, 2, 1, 1);
    ep_grid.attach(ep_show_coat_label, 8, 2, 1, 1);
    ep_grid.attach(ep_show_coat, 9, 2, 1, 1);
    ep_grid.attach(ep_show_material_label, 10, 2, 1, 1);
    ep_grid.attach(ep_show_material, 11, 2, 1, 1);
    ep_grid.attach(ep_show_model_label, 12, 2, 1, 1);
    ep_grid.attach(ep_show_model, 13, 2, 1, 1);

    stack.add(ep_grid, "Eyepieces", _("Eyepieces"));
}

void BrowserWindow::make_telescope_view()
{
    Uidefs::set_ui_spacing(scope_grid);
    scopeview.set_expand(true);

    //m_eptree_filter = Gtk::TreeModelFilter::create(m_eptree_store, Gtk::TreeModel::Path());
    m_scopetree_store = m_scopemodel.get_scopemodel();

    scopeview.set_model(m_scopetree_store);
    scopeview.set_grid_lines(Gtk::TreeView::GridLines::BOTH);

    scopeview.append_column(_("Brand"), m_scopemodel.m_scopecols.m_sbrand);
    scopeview.get_column(0)->set_resizable(true);
    scopeview.get_column(0)->set_expand(false);
    scopeview.set_expander_column(*scopeview.get_column(0));
    scopeview.get_column(0)->get_first_cell()->set_padding(0, 5);

    scopeview.append_column(_("Model"), m_scopemodel.m_scopecols.m_smodel);
    scopeview.get_column(1)->set_resizable(true);
    scopeview.get_column(1)->set_expand(false);
    scopeview.get_column(1)->get_first_cell()->set_padding(5, 5);

    scope_cells = std::vector<Gtk::CellRendererText>(8);
    scope_cols = std::vector<Gtk::TreeView::Column>(8);

    for (size_t i{0}; i < 5; ++i)
    {
        scope_cols[i].set_resizable(true);
        scope_cols[i].set_expand(false);
        scope_cells[i].set_padding(10, 5);
        scopeview.append_column(scope_cols[i]);
    }

    scope_cols[0].set_title(_("Aperture"));
    scopeview.get_column(2)->pack_start(scope_cells[0]);
    scopeview.get_column(2)->set_cell_data_func(scope_cells[0], sigc::mem_fun(*this, &BrowserWindow::on_scope_aperture_data));

    scope_cols[1].set_title(_("Focal length"));
    scopeview.get_column(3)->pack_start(scope_cells[1]);
    scopeview.get_column(3)->set_cell_data_func(scope_cells[1], sigc::mem_fun(*this, &BrowserWindow::on_scope_flen_data));

    scope_cols[2].set_title(_("Transmission"));
    scopeview.get_column(4)->pack_start(scope_cells[2]);
    scopeview.get_column(4)->set_cell_data_func(scope_cells[2], sigc::mem_fun(*this, &BrowserWindow::on_scope_trans_data));

    scope_cols[3].set_title(_("Obstruction"));
    scopeview.get_column(5)->pack_start(scope_cells[3]);
    scopeview.get_column(5)->set_cell_data_func(scope_cells[3], sigc::mem_fun(*this, &BrowserWindow::on_scope_obstruct_data));

    scope_cols[4].set_title(_("Type"));
    scopeview.get_column(6)->pack_start(scope_cells[4]);
    scopeview.get_column(6)->set_cell_data_func(scope_cells[4], sigc::mem_fun(*this, &BrowserWindow::on_scope_type_data));

    scopeview.append_column(_("Mirror coat"), m_scopemodel.m_scopecols.m_smirrorcoating);
    scopeview.get_column(7)->set_resizable(true);
    scopeview.get_column(7)->set_expand(false);
    scopeview.get_column(7)->get_first_cell()->set_padding(10, 5);

    scopeview.append_column(_("Mirror material"), m_scopemodel.m_scopecols.m_smirrormaterial);
    scopeview.get_column(8)->set_resizable(true);
    scopeview.get_column(8)->set_expand(false);
    scopeview.get_column(8)->get_first_cell()->set_padding(10, 5);

    scopeview.append_column(_("Lens coat"), m_scopemodel.m_scopecols.m_slenscoating);
    scopeview.get_column(9)->set_resizable(true);
    scopeview.get_column(9)->set_expand(false);
    scopeview.get_column(9)->get_first_cell()->set_padding(10, 5);

    scopeview.append_column(_("Lens material"), m_scopemodel.m_scopecols.m_slensmaterial);
    scopeview.get_column(10)->set_resizable(true);
    scopeview.get_column(10)->set_expand(false);
    scopeview.get_column(10)->get_first_cell()->set_padding(10, 5);

    for (size_t i{5}; i < 8; ++i)
    {
        scope_cols[i].set_resizable(true);
        scope_cols[i].set_expand(false);
        scope_cells[i].set_padding(10, 5);
        scopeview.append_column(scope_cols[i]);
    }
    
    scope_cols[5].set_title(_("Strehl"));
    scopeview.get_column(11)->pack_start(scope_cells[5]);
    scopeview.get_column(11)->set_cell_data_func(scope_cells[5], 
    sigc::mem_fun(*this, &BrowserWindow::on_scope_strehl_data));

    scope_cols[6].set_title(_("Total weight"));
    scopeview.get_column(12)->pack_start(scope_cells[6]);
    scopeview.get_column(12)->set_cell_data_func(scope_cells[6], 
    sigc::mem_fun(*this, &BrowserWindow::on_scope_total_weight_data));

    scope_cols[7].set_title(_("Tube weight"));
    scopeview.get_column(13)->pack_start(scope_cells[7]);
    scopeview.get_column(13)->set_cell_data_func(scope_cells[7], 
    sigc::mem_fun(*this, &BrowserWindow::on_scope_tube_weight_data));

    scopeview.append_column(_("Mount type"), m_scopemodel.m_scopecols.m_smount_type);
    scopeview.get_column(14)->set_resizable(true);
    scopeview.get_column(14)->set_expand(false);
    scopeview.get_column(14)->get_first_cell()->set_padding(10, 5);

    scopeview.append_column(_("Focuser"), m_scopemodel.m_scopecols.m_sfocuser_type);
    scopeview.get_column(15)->set_resizable(true);
    scopeview.get_column(15)->set_expand(false);
    scopeview.get_column(15)->get_first_cell()->set_padding(10, 5);

    scopeview.append_column(_("Finder"), m_scopemodel.m_scopecols.m_sfinder_type);
    scopeview.get_column(16)->set_resizable(true);
    scopeview.get_column(16)->set_expand(false);
    scopeview.get_column(16)->get_first_cell()->set_padding(10, 5);


    scopescrollview.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scopescrollview.set_expand(true);
    scopescrollview.set_child(scopeview);

    scope_show_expand_label.set_halign(Gtk::Align::START);
    scope_show_aperture_label.set_halign(Gtk::Align::START);
    scope_show_flen_label.set_halign(Gtk::Align::START);
    scope_show_trans_label.set_halign(Gtk::Align::START);
    scope_show_obstruct_label.set_halign(Gtk::Align::START);
    scope_show_type_label.set_halign(Gtk::Align::START);
    scope_show_mirror_coat_label.set_halign(Gtk::Align::START);
    scope_show_mirror_material_label.set_halign(Gtk::Align::START);
    scope_show_lens_coat_label.set_halign(Gtk::Align::START);
    scope_show_lens_material_label.set_halign(Gtk::Align::START);
    scope_show_strehl_label.set_halign(Gtk::Align::START);
    scope_show_total_weight_label.set_halign(Gtk::Align::START);
    scope_show_tube_weight_label.set_halign(Gtk::Align::START);
    scope_show_mount_label.set_halign(Gtk::Align::START);
    scope_show_focuser_label.set_halign(Gtk::Align::START);
    scope_show_finder_label.set_halign(Gtk::Align::START);
    scope_show_model_label.set_halign(Gtk::Align::START);

    scope_show_expand_label.set_text(_("Show all"));
    scope_show_aperture_label.set_text(_("Aperture"));
    scope_show_flen_label.set_text(_("Focal length"));
    scope_show_trans_label.set_text(_("Transmission"));
    scope_show_obstruct_label.set_text(_("Obstruction"));
    scope_show_type_label.set_text(_("Type"));
    scope_show_mirror_coat_label.set_text(_("Mirror coat"));
    scope_show_mirror_material_label.set_text(_("Mirror material"));
    scope_show_lens_coat_label.set_text(_("Lens coat"));
    scope_show_lens_material_label.set_text(_("Lens material"));
    scope_show_strehl_label.set_text(_("Strehl"));
    scope_show_total_weight_label.set_text(_("Total weight"));
    scope_show_tube_weight_label.set_text(_("Tube weight"));
    scope_show_mount_label.set_text(_("Mount")); 
    scope_show_focuser_label.set_text(_("Focuser"));
    scope_show_finder_label.set_text(_("Finder"));
    scope_show_model_label.set_text(_("Model"));

    scope_expand_tree.set_active(false);
    scope_show_aperture.set_active(true);
    scope_show_flen.set_active(true);
    scope_show_trans.set_active(true);
    scope_show_obstruct.set_active(true);
    scope_show_type.set_active(true);
    scope_show_mirror_coat.set_active(true);
    scope_show_mirror_material.set_active(true);
    scope_show_lens_coat.set_active(true);
    scope_show_lens_material.set_active(true);
    scope_show_strehl.set_active(true);
    scope_show_total_weight.set_active(true);
    scope_show_tube_weight.set_active(true);
    scope_show_mount.set_active(true);
    scope_show_focuser.set_active(true);
    scope_show_finder.set_active(true);
    scope_show_model.set_active(true);

    scope_show_trans.set_tooltip_text(_("In the case of reflectors this column refers to reflectivity."));

    scope_grid.set_column_homogeneous(true);
    scope_grid.attach(scopescrollview, 0, 0, 12, 1);
    scope_grid.attach(scope_show_expand_label, 0, 1, 1, 1);
    scope_grid.attach(scope_expand_tree, 1, 1, 1, 1);
    scope_grid.attach(scope_show_aperture_label, 2, 1, 1, 1);
    scope_grid.attach(scope_show_aperture, 3, 1, 1, 1);
    scope_grid.attach(scope_show_flen_label, 4, 1, 1, 1);
    scope_grid.attach(scope_show_flen, 5, 1, 1, 1);
    scope_grid.attach(scope_show_trans_label, 6, 1, 1, 1);
    scope_grid.attach(scope_show_trans, 7, 1, 1, 1);
    scope_grid.attach(scope_show_obstruct_label, 8, 1, 1, 1);
    scope_grid.attach(scope_show_obstruct, 9, 1, 1, 1);
    scope_grid.attach(scope_show_type_label, 10, 1, 1, 1);
    scope_grid.attach(scope_show_type, 11, 1, 1, 1);
    scope_grid.attach(scope_show_mirror_coat_label, 0, 2, 1, 1);
    scope_grid.attach(scope_show_mirror_coat, 1, 2, 1, 1);
    scope_grid.attach(scope_show_mirror_material_label, 2, 2, 1, 1);
    scope_grid.attach(scope_show_mirror_material, 3, 2, 1, 1);
    scope_grid.attach(scope_show_lens_coat_label, 4, 2, 1, 1);
    scope_grid.attach(scope_show_lens_coat, 5, 2, 1, 1);
    scope_grid.attach(scope_show_lens_material_label, 6, 2, 1, 1);
    scope_grid.attach(scope_show_lens_material, 7, 2, 1, 1);
    scope_grid.attach(scope_show_strehl_label, 8, 2, 1, 1);
    scope_grid.attach(scope_show_strehl, 9, 2, 1, 1);
    scope_grid.attach(scope_show_total_weight_label, 10, 2, 1, 1);
    scope_grid.attach(scope_show_total_weight, 11, 2, 1, 1);
    scope_grid.attach(scope_show_tube_weight_label, 2, 3, 1, 1);
    scope_grid.attach(scope_show_tube_weight, 3, 3, 1, 1);
    scope_grid.attach(scope_show_mount_label, 4, 3, 1, 1);
    scope_grid.attach(scope_show_mount, 5, 3, 1, 1);
    scope_grid.attach(scope_show_focuser_label, 6, 3, 1, 1);
    scope_grid.attach(scope_show_focuser, 7, 3, 1, 1);
    scope_grid.attach(scope_show_finder_label, 8, 3, 1, 1);
    scope_grid.attach(scope_show_finder, 9, 3, 1, 1);
    scope_grid.attach(scope_show_model_label, 10, 3, 1, 1);
    scope_grid.attach(scope_show_model, 11, 3, 1, 1);
    

    stack.add(scope_grid, "Telescopes", _("Telescopes"));
}