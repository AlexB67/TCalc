#pragma once
#include "eyepiececombomodel.hpp"
#include "gtkmmcustomutils.hpp"
#include "loggerbox.hpp"
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <glibmm/i18n.h>

namespace EpBox
{
class Eyepiecebox
{

public:
  explicit Eyepiecebox(bool with_entry = false, bool userdataonly = false);
  virtual ~Eyepiecebox() {}
  Eyepiecebox(const Eyepiecebox &) = delete;
  Eyepiecebox(Eyepiecebox&& ) = delete;
  Eyepiecebox &operator=(const Eyepiecebox &other) = delete;
  Eyepiecebox &operator=(Eyepiecebox&& ) = delete;

  Gtk::Frame &create_eyepiece_grid();
  const Gtk::TreeRow& get_current_row() const { return current_row; }
  // call this afer all widgets are shown 
  virtual void init() { if (!m_with_entry) ep_changed();}
  
  virtual void set_default_values();
  void enable_fstop(const bool is_enabled) { m_efstop.set_sensitive(is_enabled); }
  bool get_use_entry() const {return m_with_entry;}
  Gtk::ComboBox *m_emodel;
  Gtk::Entry            m_emodelentry;
  EpCombo::EpCombomodel m_ecombomodel;
  Gtk::ComboBoxText m_etype;
  Ui::SpinEntry m_efov;
  Ui::SpinEntry m_eflen;
  Ui::SpinEntry m_efstop;
  Ui::SpinEntry m_erelief;
  Ui::SpinEntry m_etrans;

private:
  Gtk::Label m_efovlabel{_("FOV/degrees"), Gtk::Align::START};
  Gtk::Label m_eflenlabel{_("Focal length/mm"), Gtk::Align::START};
  Gtk::Label m_efstoplabel{_("Field stop/mm"), Gtk::Align::START};
  Gtk::Label m_erelieflabel{_("Eye relief/mm"), Gtk::Align::START};
  Gtk::Label m_etranslabel{_("Transmission/%"), Gtk::Align::START};
  Gtk::Label m_etypelabel{_("Eyepiece type"), Gtk::Align::START};
  int get_eyepiece_type(const Gtk::TreeModel::Row &row) const;
  bool m_with_entry;
  bool m_userdataonly;

protected:
  Gtk::Grid   m_grid;
  Gtk::Frame  m_frame;
  Gtk::Label  m_framelabel;
  Gtk::Label  m_emodellabel{_("Select eyepiece"), Gtk::Align::CENTER};
  Gtk::CellRendererText m_cell;
  Glib::RefPtr<Gtk::EntryCompletion> epentrycompletion;
  Gtk::TreeRow current_row;
  Gtk::TreeModel::iterator current_iter;
  std::vector<sigc::connection> con;
  void ep_changed();
  virtual void create_epmodel_connection();
  void create_ep_entry_model();
  void create_ep_combo_model();
  void ep_type_changed();
  bool on_ep_selected(const Gtk::TreeModel::iterator& iter);
  bool on_ep_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);
  void on_cell_data_changed(const Gtk::TreeModel::const_iterator& iter);
  void set_values_from_model(const Gtk::TreeRow& row);
  void set_custom_ep();
  void reset_emodel_entry();
  void clear_emodel_entry(Gtk::Entry::IconPosition pos);
};
} // namespace EpBox
