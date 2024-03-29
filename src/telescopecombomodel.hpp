#pragma once
#include <glibmm.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/entry.h>
#include <gtkmm/entrycompletion.h>
#include <gtkmm/cellrenderertext.h>


namespace ScopeCombo
{
  struct Scopemodelcols : public Gtk::TreeModel::ColumnRecord
  {
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
    Gtk::TreeModelColumn<double> m_stube_weight;
    Gtk::TreeModelColumn<Glib::ustring> m_smount_type;
    Gtk::TreeModelColumn<Glib::ustring> m_sfocuser_type;
    Gtk::TreeModelColumn<Glib::ustring> m_sfinder_type;

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
      add(m_stube_weight);
      add(m_smount_type);
      add(m_sfocuser_type);
      add(m_sfinder_type);
    }
  };

  struct Scopelistcols : public Gtk::TreeModel::ColumnRecord
  {
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
      Scopemodelcols m_scopecols;
      Scopelistcols m_scopecompletioncols;
      
      void create_scope_model();

      void append_scope_to_model (const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata, bool ischild = false);

      void update_scope_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata, const Glib::ustring& oldname) const;

      void add_scope_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, int, 
                                  Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, double, double, double,  
                                  Glib::ustring, Glib::ustring, Glib::ustring>& scopedata) const;
      
      void remove_scope_from_model(const Glib::ustring &scopename) const;
      void swap_scope_rows(const Glib::ustring& scopename) const;
      const Glib::RefPtr<Gtk::TreeStore>& get_scopemodel() const { return m_scopetreemodel;}
      const Glib::RefPtr<Gtk::ListStore>& get_scope_list_model() const { return  m_scopelistmodel;}
    
    private:
      Glib::RefPtr<Gtk::TreeStore>        m_scopetreemodel;
      Glib::RefPtr<Gtk::ListStore>        m_scopelistmodel;
      Gtk::TreeModel::Row parent_row;
    
    protected:
      bool on_scope_selected(const Gtk::TreeModel::iterator& iter);
      bool on_scope_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);
      void on_cell_data_changed(const Gtk::TreeModel::const_iterator& iter);
  };
}

