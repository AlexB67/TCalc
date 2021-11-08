#pragma once
#include "telescopecombomodel.hpp"
#include "loggerbox.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/combobox.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/sizegroup.h>
#include <glibmm/i18n.h>

namespace ScopeBox
{
class Telescopebox
{

public:
  explicit Telescopebox(bool with_entry = false, bool userdataonly = false);
  Telescopebox(const Telescopebox &) = delete;
  Telescopebox(Telescopebox &&) = delete;
  Telescopebox &operator=(const Telescopebox &other) = delete;
  Telescopebox &operator=(Telescopebox &&) = delete;
  virtual ~Telescopebox() {}

  Gtk::Frame &create_telescope_grid();
  const Gtk::TreeRow& get_current_row() const { return current_row; }
  virtual void init() { if (!m_with_entry) scope_changed(); } 
  virtual void set_default_values();
  bool get_use_entry() const {return m_with_entry;}
  void frame_can_expand(const bool expand) { m_frame.set_vexpand(expand); }

  Gtk::ComboBox *m_smodel;
  Gtk::Entry     m_smodelentry;
  ScopeCombo::ScopeCombomodel m_scombomodel;
  Gtk::ComboBoxText m_stype;
  Ui::SpinEntry m_sflen;
  Ui::SpinEntry m_saperture;
  Ui::SpinEntry m_sfratio;
  Ui::SpinEntry m_sreflect;
  Ui::SpinEntry m_sobstruct;

private:
  Gtk::Label m_sflenlabel{_("Focal length/mm"), Gtk::Align::START};
  Gtk::Label m_saperturelabel{_("Aperture/mm"), Gtk::Align::START};
  Gtk::Label m_sfratiolabel{_("Focal ratio"), Gtk::Align::START};
  Gtk::Label m_sreflectlabel{_("Reflectivity/%"), Gtk::Align::START};
  Gtk::Label m_sobstructlabel{_("Obstruction/%"), Gtk::Align::START};
  Gtk::Label m_stypelabel{_("Telescope type"), Gtk::Align::START};
  bool m_with_entry;
  bool m_userdataonly;

protected:
  Gtk::Grid m_grid;
  Gtk::Frame m_frame;
  Gtk::Label m_framelabel;
  Gtk::Label m_smodellabel{_("Select telescope"), Gtk::Align::CENTER};
  Gtk::CellRendererText m_cell;
  Glib::RefPtr<Gtk::EntryCompletion> scopeentrycompletion;
  Gtk::TreeRow current_row;
  Gtk::TreeNodeChildren::iterator current_iter;
  std::vector<sigc::connection> con;
  void update_sfratio();
  void set_sflen();
  void scope_changed();
  void scope_type_changed();
  virtual void create_scopemodel_connection();
  void create_scope_entry_model();
  void create_scope_combo_model();
  void on_cell_data_changed(const Gtk::TreeModel::const_iterator& iter);
  void set_values_from_model(const Gtk::TreeRow& row);
  bool on_scope_selected(const Gtk::TreeModel::iterator& iter);
  bool on_scope_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);
  void set_custom_scope();
  void reset_smodel_entry();
  void clear_smodel_entry(Gtk::Entry::IconPosition pos);
};
} // namespace ScopeBox
