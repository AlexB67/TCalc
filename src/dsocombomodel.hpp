#pragma once
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>

class DSOmodelcols : public Gtk::TreeModel::ColumnRecord
{
public:
  Gtk::TreeModelColumn<int> m_DSOid;
  Gtk::TreeModelColumn<Glib::ustring> m_DSOname;
  Gtk::TreeModelColumn<std::string> m_DSOimagefile;
  Gtk::TreeModelColumn<double> m_DSOvmag;
  Gtk::TreeModelColumn<double> m_DSOminoraxis;
  Gtk::TreeModelColumn<double> m_DSOmajoraxis;
  Gtk::TreeModelColumn<double> m_DSOimagesize;
  Gtk::TreeModelColumn<Glib::ustring> m_DSOtype;

  DSOmodelcols()
  {
    add(m_DSOid);
    add(m_DSOname);
    add(m_DSOimagefile);
    add(m_DSOvmag);
    add(m_DSOminoraxis);
    add(m_DSOmajoraxis);
    add(m_DSOimagesize);
    add(m_DSOtype);
  }
};

class DSOCombomodel
{
public:
  void create_dso_model_combo(Gtk::ComboBox &Combo);
  void append_dso_to_model(const int dos_id, const Glib::ustring &dso_name, const std::string &dso_imagefile, const double dso_vmag,
                           const double dso_minoraxis, const double dso_majoraxis, const double  dso_imagesize, 
                           const Glib::ustring &dso_type) const;
  void setup_dso_combo();
  DSOmodelcols m_dsocols;

private:
  Glib::RefPtr<Gtk::ListStore> m_dsotreemodel;
  Gtk::ComboBox *m_dsoCombo = nullptr;
};
