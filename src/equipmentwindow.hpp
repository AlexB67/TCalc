#pragma once
#include "eyepiececombomodel.hpp"
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/switch.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/cellrenderer.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/stackswitcher.h>


class BrowserWindow : public Gtk::Window
{
public:
    explicit BrowserWindow(const Glib::RefPtr<Gtk::TreeStore>& eptree_store, EpCombo::EpCombomodel& epmodel);
    virtual ~BrowserWindow(){}
    BrowserWindow(const BrowserWindow&) = delete;
    BrowserWindow(BrowserWindow&& ) = delete;
    BrowserWindow& operator=(const BrowserWindow& other) = delete;
    BrowserWindow& operator=(BrowserWindow&& other) = delete;   

private:
    Glib::RefPtr<Gtk::TreeStore> m_eptree_store;
    EpCombo::EpCombomodel& m_epmodel;
    Gtk::TreeView epview;
    Gtk::ScrolledWindow scrollview;
    Gtk::Grid   grid;
    Gtk::HeaderBar headerbar;
    Gtk::Label     headerlabel;
    Gtk::Label     tmplabel;
    Gtk::StackSwitcher  switcher;
    Gtk::Stack          stack;
    std::vector<Gtk::TreeView::Column> cols;
    std::vector<Gtk::CellRendererText> cells;

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
};