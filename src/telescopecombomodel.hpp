#pragma once
#include <glibmm.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/entry.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/entrycompletion.h>


namespace ScopeCombo
{
  class Scopemodelcols : public Gtk::TreeModel::ColumnRecord
  {
    public:
      Gtk::TreeModelColumn<Glib::ustring> m_sbrand;
      Gtk::TreeModelColumn<Glib::ustring> m_smodel;
      Gtk::TreeModelColumn<double> m_saperture;
      Gtk::TreeModelColumn<double> m_sflen;
      Gtk::TreeModelColumn<double> m_sobstruct;
      Gtk::TreeModelColumn<double> m_sreflect;
      Gtk::TreeModelColumn<int> m_stype;
      Gtk::TreeModelColumn<Glib::ustring> m_smirrorcoating;
      Gtk::TreeModelColumn<Glib::ustring> m_smirrormaterial;
      Gtk::TreeModelColumn<Glib::ustring> m_slenscoating;
      Gtk::TreeModelColumn<Glib::ustring> m_slensmaterial;
      Gtk::TreeModelColumn<double> m_sstrehl;
      Gtk::TreeModelColumn<double> m_sweight;



      
      Scopemodelcols()
      {
        add(m_sbrand);
        add(m_smodel); 
        add(m_saperture); 
        add(m_sflen);
        add(m_sobstruct);
        add(m_sreflect);
        add(m_stype);
        add(m_smirrorcoating);
        add(m_smirrormaterial);
        add(m_slenscoating);
        add(m_slensmaterial);
        add(m_sstrehl);
        add(m_sweight);
      }
      
  };

  class Scopelistcols : public Gtk::TreeModel::ColumnRecord
  {
    public:
      Gtk::TreeModelColumn<Glib::ustring> m_sbrand;
      Gtk::TreeModelColumn<Glib::ustring> m_smodel;
      Scopelistcols()
      {
        add(m_sbrand);
        add(m_smodel);
      }
  };

  class ScopeCombomodel
  {
    public:
      void create_scope_model();

      void append_scope_to_model (const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                                  double, double>& scopedata, bool ischild = false);

      void update_scope_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                              Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                              double, double>& scopedata, const Glib::ustring& oldname) const;

      void add_scope_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                              Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
                              double, double>& scopedata) const;
      
      void remove_scope_from_model(const Glib::ustring &scopename) const;
      void swap_scope_rows(const Glib::ustring& scopename) const;
      void setup_scope_combo_model(Gtk::ComboBox& scopecombo);
      void set_scope_completion_model(Gtk::SearchEntry &scopesearch);
      void set_case_sensitive(const bool case_sensitive);
      const Glib::RefPtr<Gtk::TreeStore>& get_scopemodel() const { return m_scopetreemodel;}
      Scopemodelcols m_scopecols;
      Scopelistcols m_scopecompletioncols;
    
    private:
      Gtk::TreeModel::Row parent_row;
      Glib::RefPtr<Gtk::TreeStore>        m_scopetreemodel;
      Glib::RefPtr<Gtk::ListStore>        m_scopelistmodel;
      Glib::RefPtr<Gtk::EntryCompletion>  scopeentrycompletion;
      Gtk::ComboBox     *m_scopecombo   = nullptr;
      Gtk::SearchEntry  *m_scopesearch  = nullptr;
      bool m_case_sensitive             = false;
    
    protected:
      bool on_scope_selected(const Gtk::TreeModel::iterator& iter);
      bool on_scope_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);
  };
}

