#pragma once
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/switch.h>
#include <gtkmm/entrycompletion.h>
#include "loggerbox.hpp"
#include "eyepiecebox.hpp"
#include "telescopebox.hpp"


class SearchWindow : public Gtk::Window
{
public:
    explicit SearchWindow(const std::shared_ptr<EpBox::Eyepiecebox>& epbox,
                          const std::shared_ptr<ScopeBox::Telescopebox>& scopebox);
    virtual ~SearchWindow(){}
    SearchWindow(const SearchWindow&) = delete;
    SearchWindow(SearchWindow&& ) = delete;
    SearchWindow& operator=(const SearchWindow& other) = delete;
    SearchWindow& operator=(SearchWindow&& other) = delete;   

private:
    Gtk::Grid   grid;
    Gtk::Entry  epsearch;
    Gtk::Entry  scopesearch;
    Gtk::Label  epsearchlabel;
    Gtk::Label  scopesearchlabel;
    Gtk::Label  casesensitivelabel;
    Gtk::Switch *casesensitive;
    std::shared_ptr<EpBox::Eyepiecebox>  m_epbox;
    std::shared_ptr<ScopeBox::Telescopebox> m_scopebox;
    bool on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring& phase);
};