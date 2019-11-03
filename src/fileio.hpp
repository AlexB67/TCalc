#include "telescopecombomodel.hpp"
#include "eyepiececombomodel.hpp"
#include "dsocombomodel.hpp"

namespace fileIO
{
  class dbfileIO
  {
  public:
    void load_ep_data(Gtk::ComboBox &epcombobox,
                      EpCombo::EpCombomodel &epcombomodel,
                      const bool userdataonly = false);

    void load_scope_data(Gtk::ComboBox &scopecombobox,
                        ScopeCombo::ScopeCombomodel &scopecombomodel,
                        const bool userdataonly = false);

    void load_dso_data(Gtk::ComboBox &dsocombobox, DSOCombomodel &dsocombomodel) const;

    void write_ep_user_data(const Gtk::ComboBox& epcombobox, 
                            const EpCombo::EpCombomodel &epcombomodel) const;
    
    void write_scope_user_data( const Gtk::ComboBox& scopecombobox, 
                                const  ScopeCombo::ScopeCombomodel &scopecombomodel) const;

  private:
    void read_ep_file(const Gtk::ComboBox &epcombobox,
                      const Glib::ustring &path,
                      const EpCombo::EpCombomodel &epcombomodel);

    void read_scope_file(const Gtk::ComboBox &scopecombobox,
                         const Glib::ustring &path,
                         const ScopeCombo::ScopeCombomodel &scopecombomodel);
  };
} // namespace fileIO