#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

namespace Ui
{
  class SpinEntry : public Gtk::SpinButton
  {
  public:
      void set_spin_entry(double value, double min, double max, double incr, int digits, bool isnumeric);
  };
}

namespace Uidefs
{
  constexpr guint ROW_PADDING         = 10;
  constexpr guint BORDER_WIDTH        = 10;
  constexpr guint COLUMN_PADDING      = 10;
  constexpr guint BORDER_WIDTH_SMALL  = 5;

  template <typename T>
  void set_ui_spacing(T &obj) // could be used for grid, or any widget that accepts these
  {
    obj.set_row_spacing(Uidefs::ROW_PADDING);
    obj.set_margin(Uidefs::BORDER_WIDTH);
    obj.set_column_spacing(Uidefs::COLUMN_PADDING);
  }

  template <typename T>
  void set_ui_spacing_small(T &obj) // could be used for grid, or any widget that accepts these
  {
    obj.set_row_spacing(Uidefs::BORDER_WIDTH_SMALL);
    obj.set_margin(Uidefs::BORDER_WIDTH_SMALL);
    obj.set_column_spacing(Uidefs::BORDER_WIDTH_SMALL);
  }
}


