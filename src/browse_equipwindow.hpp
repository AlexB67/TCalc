#pragma once
#include "eyepiececombomodel.hpp"
#include "telescopecombomodel.hpp"
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/switch.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
//#include <gtkmm/treemodelfilter.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/cellrenderer.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/checkbutton.h>


class BrowserWindow : public Gtk::Window
{
public:
    explicit BrowserWindow(EpCombo::EpCombomodel& epmodel,
                           ScopeCombo::ScopeCombomodel& scopemodel);

    virtual ~BrowserWindow(){}
    BrowserWindow(const BrowserWindow&) = delete;
    BrowserWindow(BrowserWindow&& ) = delete;
    BrowserWindow& operator=(const BrowserWindow& other) = delete;
    BrowserWindow& operator=(BrowserWindow&& other) = delete;   

private:
    EpCombo::EpCombomodel& m_epmodel;
    ScopeCombo::ScopeCombomodel& m_scopemodel;
    Glib::RefPtr<Gtk::TreeStore> m_eptree_store;
    Glib::RefPtr<Gtk::TreeStore> m_scopetree_store;
    //Glib::RefPtr<Gtk::TreeModelFilter> m_eptree_filter;
    Gtk::HeaderBar headerbar;
    Gtk::Label     headerlabel;
    // eyepiece view widgets
    Gtk::ScrolledWindow epscrollview;
    Gtk::ScrolledWindow scopescrollview;
    Gtk::TreeView epview;
    Gtk::TreeView scopeview;
    Gtk::Grid      ep_grid;
    Gtk::Grid      scope_grid;
    // eyepiece page labels
    Gtk::Label     ep_show_expand_label;
    Gtk::Label     ep_show_fov_label;
    Gtk::Label     ep_show_flen_label;
    Gtk::Label     ep_show_fstop_label;
    Gtk::Label     ep_show_relief_label;
    Gtk::Label     ep_show_trans_label;
    Gtk::Label     ep_show_barrel_label;
    Gtk::Label     ep_show_type_label;
    Gtk::Label     ep_show_groups_label;
    Gtk::Label     ep_show_elements_label;
    Gtk::Label     ep_show_weight_label;
    Gtk::Label     ep_show_coat_label;
    Gtk::Label     ep_show_material_label;
    Gtk::Label     ep_show_model_label;
    // telescope page labels
    Gtk::Label     scope_show_expand_label;
    Gtk::Label     scope_show_aperture_label;
    Gtk::Label     scope_show_flen_label;
    Gtk::Label     scope_show_trans_label;
    Gtk::Label     scope_show_obstruct_label;
    Gtk::Label     scope_show_type_label;
    Gtk::Label     scope_show_mirror_coat_label;
    Gtk::Label     scope_show_mirror_material_label;
    Gtk::Label     scope_show_lens_coat_label;
    Gtk::Label     scope_show_lens_material_label;
    Gtk::Label     scope_show_strehl_label;
    Gtk::Label     scope_show_total_weight_label;
    Gtk::Label     scope_show_tube_weight_label;
    Gtk::Label     scope_show_mount_label;
    Gtk::Label     scope_show_focuser_label;
    Gtk::Label     scope_show_finder_label;
    Gtk::Label     scope_show_model_label;
    // eyepiece page toggles
    Gtk::CheckButton    ep_expand_tree;
    Gtk::CheckButton    ep_show_fov;
    Gtk::CheckButton    ep_show_flen;
    Gtk::CheckButton    ep_show_fstop;
    Gtk::CheckButton    ep_show_relief;
    Gtk::CheckButton    ep_show_trans;
    Gtk::CheckButton    ep_show_barrel;
    Gtk::CheckButton    ep_show_type;
    Gtk::CheckButton    ep_show_groups;
    Gtk::CheckButton    ep_show_elements;
    Gtk::CheckButton    ep_show_weight;
    Gtk::CheckButton    ep_show_coat;
    Gtk::CheckButton    ep_show_material;
    Gtk::CheckButton    ep_show_model;
    // telescope page toggles
    Gtk::CheckButton    scope_expand_tree;
    Gtk::CheckButton    scope_show_aperture;
    Gtk::CheckButton    scope_show_flen;
    Gtk::CheckButton    scope_show_trans;
    Gtk::CheckButton    scope_show_obstruct;
    Gtk::CheckButton    scope_show_type;
    Gtk::CheckButton    scope_show_mirror_coat;
    Gtk::CheckButton    scope_show_mirror_material;
    Gtk::CheckButton    scope_show_lens_coat;
    Gtk::CheckButton    scope_show_lens_material;
    Gtk::CheckButton    scope_show_strehl;
    Gtk::CheckButton    scope_show_total_weight;
    Gtk::CheckButton    scope_show_tube_weight;
    Gtk::CheckButton    scope_show_mount;
    Gtk::CheckButton    scope_show_focuser;
    Gtk::CheckButton    scope_show_finder;
    Gtk::CheckButton    scope_show_model;

    Gtk::StackSwitcher  switcher;
    Gtk::Stack          stack;
    std::vector<Gtk::TreeView::Column> ep_cols;
    std::vector<Gtk::CellRendererText> ep_cells;

    std::vector<Gtk::TreeView::Column> scope_cols;
    std::vector<Gtk::CellRendererText> scope_cells;
    // telescope view widgets

    void make_eyepiece_view();
    void make_telescope_view();
    bool on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring& phase);
    void on_ep_fov_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_flen_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_fstop_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_relief_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_trans_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_barrel_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_group_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_element_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_ep_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    //bool on_show_visible(const Gtk::TreeModel::const_iterator& iter);

    void on_scope_aperture_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_flen_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_trans_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_obstruct_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_type_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_strehl_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_total_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    void on_scope_tube_weight_data(Gtk::CellRenderer *, const Gtk::TreeModel::const_iterator);
    
    void setup_signal_handlers();
};