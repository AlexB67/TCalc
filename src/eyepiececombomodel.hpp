#pragma once
#include <glibmm.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/entrycompletion.h>
#include <tuple>

namespace EpCombo
{
  struct Epmodelcols : public Gtk::TreeModel::ColumnRecord
  {
    Gtk::TreeModelColumn<Glib::ustring> m_epbrand;
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
      add(m_epbrand);
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

  struct Eplistcols : public Gtk::TreeModel::ColumnRecord
  {
    Gtk::TreeModelColumn<Glib::ustring> m_epbrand;
    Gtk::TreeModelColumn<Glib::ustring> m_epmodel;
    Eplistcols()
    {
      add(m_epbrand);
      add(m_epmodel);
    }
  };

  class EpCombomodel  // ep stands for eyepiece
  {
    public:
      Epmodelcols m_epcols;
      Eplistcols m_epcompletioncols;

      void create_ep_model();
      void append_ep_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                              double, double, Glib::ustring, int, int, double, Glib::ustring, 
                              Glib::ustring>& epdata, bool ischild = false);

      void add_ep_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                          double, double, Glib::ustring, int, int, double, Glib::ustring, 
                          Glib::ustring>& epdata) const;
     
      void update_ep_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                          double, double, Glib::ustring, int, int, double, Glib::ustring, 
                          Glib::ustring>& epdata, const Glib::ustring& oldname) const;
      
      void remove_ep_from_model(const Glib::ustring& epname) const;
      void swap_ep_rows(const Glib::ustring& epname) const;

      const Glib::RefPtr<Gtk::TreeStore>& get_epmodel() const { return m_eptreemodel;}
      const Glib::RefPtr<Gtk::ListStore>& get_ep_list_model() const { return  m_eplistmodel;}

    private:
      Glib::RefPtr<Gtk::TreeStore> m_eptreemodel;
      Glib::RefPtr<Gtk::ListStore> m_eplistmodel;
      Gtk::TreeModel::Row parent_row;
  };
}
