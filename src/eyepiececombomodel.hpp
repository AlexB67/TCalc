#pragma once
#include <glibmm.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/entrycompletion.h>
#include <tuple>

namespace EpCombo
  {
  class Epmodelcols : public Gtk::TreeModel::ColumnRecord
  {
    public:
      Gtk::TreeModelColumn<Glib::ustring> m_epmodel;
      Gtk::TreeModelColumn<double> m_epfov;
      Gtk::TreeModelColumn<double> m_epflen;
      Gtk::TreeModelColumn<double> m_epfstop;
      Gtk::TreeModelColumn<double> m_eprelief;
      Gtk::TreeModelColumn<double> m_eptrans;
      Gtk::TreeModelColumn<double> m_epbarrel;
      Gtk::TreeModelColumn<Glib::ustring> m_eptype;
      Gtk::TreeModelColumn<int> m_epgroups;
      Gtk::TreeModelColumn<int> m_epelements;
      Gtk::TreeModelColumn<double> m_epweight;
      Gtk::TreeModelColumn<Glib::ustring> m_epcoating;
      Gtk::TreeModelColumn<Glib::ustring> m_epmaterial;
      
      Epmodelcols()
      {
        add(m_epmodel);
        add(m_epfov);
        add(m_epflen); 
        add(m_epfstop);
        add(m_eprelief);
        add(m_eptrans);
        add(m_epbarrel);
        add(m_eptype);
        add(m_epgroups);
        add(m_epelements);
        add(m_epweight);
        add(m_epcoating);
        add(m_epmaterial);
      }
  };

  class EpCombomodel  // ep stands for eyepiece
  {
    public:
      void create_ep_model();
      void append_ep_to_model(const std::tuple<Glib::ustring, double, double, double, double,
                              double, double, Glib::ustring, int, int, double, Glib::ustring, 
                              Glib::ustring>& epdata) const;

      void add_ep_to_model(const std::tuple<Glib::ustring, double, double, double, double,
                          double, double, Glib::ustring, int, int, double, Glib::ustring, 
                          Glib::ustring>& epdata, bool append = true) const;
     
      void update_ep_model(const std::tuple<Glib::ustring, double, double, double, double,
                          double, double, Glib::ustring, int, int, double, Glib::ustring, 
                          Glib::ustring>& epdata) const;
      
      void remove_ep_from_model(const Glib::ustring& epname) const;
      void swap_ep_rows(const Glib::ustring& epname, bool movedown = true) const;
      void setup_ep_combo_model(Gtk::ComboBox &epcombo);
      void set_ep_completion_model(Gtk::SearchEntry& epsearch);
      void set_case_sensitive(const bool case_sensitive);
      const Glib::RefPtr<Gtk::ListStore>& get_epmodel() const { return m_eptreemodel;}
      Epmodelcols m_epcols;

    private:
      Glib::RefPtr<Gtk::ListStore> m_eptreemodel;
      Glib::RefPtr<Gtk::EntryCompletion> epentrycompletion;
      Gtk::ComboBox*    m_epcombo   = nullptr;
      Gtk::SearchEntry* m_epsearch  = nullptr;
      bool m_case_sensitive = false;

    protected:
      bool on_ep_selected(const Gtk::TreeModel::iterator& iter);
      bool on_ep_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);
      bool on_separator(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::iterator& iter);

  };
}
