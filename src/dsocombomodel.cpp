#include "dsocombomodel.hpp"

void DSOCombomodel::create_dso_model_combo(Gtk::ComboBox& Combo)
{
    m_dsoCombo = &Combo;
    m_dsotreemodel = Gtk::ListStore::create(m_dsocols);
    m_dsoCombo->set_model(m_dsotreemodel);
}

void DSOCombomodel::setup_dso_combo()
{
    m_dsoCombo->pack_start(m_dsocols.m_DSOname);
    m_dsoCombo->set_active(0);
}

void DSOCombomodel::append_dso_to_model(const int dos_id, const Glib::ustring &dso_name, const std::string &dso_imagefile, const double dso_vmag,
            const double dso_minoraxis, const double dso_majoraxis, const double dso_imagesize, const Glib::ustring &dso_type) const
{   
  Gtk::TreeModel::Row row = *(m_dsotreemodel->append());
  row[m_dsocols.m_DSOid] = dos_id;
  row[m_dsocols.m_DSOname] = dso_name;
  row[m_dsocols.m_DSOimagefile] = dso_imagefile;
  row[m_dsocols.m_DSOvmag] = dso_vmag;
  row[m_dsocols.m_DSOminoraxis] = dso_minoraxis;
  row[m_dsocols.m_DSOmajoraxis] = dso_majoraxis;
  row[m_dsocols.m_DSOimagesize] = dso_imagesize;
  row[m_dsocols.m_DSOtype] = dso_type;
}
