#include "starmagbox.hpp"

using AppGlobals::log_msg;

void MagBox::Magbox::set_signal_handlers()
{
    nelm_connection = m_nelm.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::nelm_changed));
    sqm_connection = m_sqm.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::sqm_changed));

    nelm1_connection = m_nelm1.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::nelm1_changed));
    sqm1_connection = m_bgsky.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::sqm1_changed));

    m_sitetype.signal_changed().connect(sigc::mem_fun(*this, &Magbox::site_type_changed));
    m_sitetype1.signal_changed().connect(sigc::mem_fun(*this, &Magbox::dso_site_type_changed));
    m_colourbyname.signal_changed().connect(sigc::mem_fun(*this, &Magbox::star_colour_changed));

    m_age.signal_value_changed().connect((sigc::track_obj([this]() {
        m_pupilsize.set_value(m_calc.calc_eyepupil_from_age(m_age.get_value()));
    }, *this)));

    m_vmag.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::dso_brightness_changhed));
    m_minoraxis.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::dso_brightness_changhed));
    m_majoraxis.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::dso_brightness_changhed));

    m_dsobrightness.signal_value_changed().connect((sigc::track_obj([this]() {
        m_dsocontrastindex.set_value(m_calc.calc_contrast_index(m_bgsky.get_value(), m_dsobrightness.get_value()));
    }, *this)));

    m_minoraxis.signal_value_changed().connect((sigc::track_obj([this]() {
       if (m_minoraxis.get_value() > m_majoraxis.get_value()) m_majoraxis.set_value(m_minoraxis.get_value());
    }, *this)));
    
    m_majoraxis.signal_value_changed().connect((sigc::track_obj([this]() {
       if (m_majoraxis.get_value() < m_minoraxis.get_value()) m_minoraxis.set_value(m_majoraxis.get_value());
    }, *this)));
}

void MagBox::Magbox::dso_brightness_changhed()
{
    m_dsobrightness.set_value(m_calc.calc_dso_mag_to_brightness(m_vmag.get_value(), 
                                m_minoraxis.get_value(), m_majoraxis.get_value()));
}

void MagBox::Magbox::star_colour_changed()
{
    switch (m_colourbyname.get_active_row_number())
    {
    case 0:
        m_colour.set_value(0.0);
        break;
    case 1:
        m_colour.set_value(1.41);
        break;
    case 2:
        m_colour.set_value(0.82);
        break;
    case 3:
        m_colour.set_value(0.59);
        break;
    case 4:
        m_colour.set_value(0.31);
        break;
    case 5:
        m_colour.set_value(0.0);
        break;
    case 6:
        m_colour.set_value(-0.29);
        break;
    }
}

void MagBox::Magbox::site_type_changed()
{
    m_nelm.set_value(sitevalues[m_sitetype.get_active_row_number()]);
    m_sqm.set_value(m_calc.convert_nelm_to_sqm(m_nelm.get_value()));
}

void MagBox::Magbox::dso_site_type_changed()
{
    m_nelm1.set_value(sitevalues[m_sitetype1.get_active_row_number()]);
    m_bgsky.set_value(m_calc.calc_nelm_brightness_threshold_method(m_nelm1.get_value()));
    m_dsobrightness.set_value(m_calc.calc_dso_mag_to_brightness(m_vmag.get_value(), 
                              m_minoraxis.get_value(), m_majoraxis.get_value()));
    m_dsocontrastindex.set_value(m_calc.calc_contrast_index(m_bgsky.get_value(), m_dsobrightness.get_value()));
}

void MagBox::Magbox::nelm_changed()
{
    sqm_connection.block(); // To prevent nelm and sqm spinbuttons calling each other
    m_sqm.set_value(m_calc.convert_nelm_to_sqm(m_nelm.get_value()));
    sqm_connection.unblock();
}

void MagBox::Magbox::sqm_changed()
{
    nelm_connection.block(); // To prevent nelm and sqm spinbuttons calling each other
    m_nelm.set_value(m_calc.convert_sqm_to_nelm(m_sqm.get_value()));
    nelm_connection.unblock();
}

void MagBox::Magbox::nelm1_changed()
{
    sqm1_connection.block(); // To prevent nelm and sqm spinbuttons calling each other
    m_bgsky.set_value(m_calc.calc_nelm_brightness_threshold_method(m_nelm1.get_value()));
    m_dsocontrastindex.set_value(m_calc.calc_contrast_index(m_bgsky.get_value(), m_dsobrightness.get_value()));
    sqm1_connection.unblock();
}

void MagBox::Magbox::sqm1_changed()
{
    nelm1_connection.block(); // To prevent nelm1 and sqm1 spinbuttons calling each other
    m_nelm1.set_value(m_calc.calc_nelm_threshold_method(m_bgsky.get_value()));
    m_dsocontrastindex.set_value(m_calc.calc_contrast_index(m_bgsky.get_value(), m_dsobrightness.get_value()));
    nelm1_connection.unblock();
}

void MagBox::Magbox::dso_changed()
{
    Gtk::TreeModel::iterator iter = m_dsocombo.get_active();

    if (iter)
    {
        Gtk::TreeModel::Row row = *iter;
        if (row)
        {
            m_minoraxis.set_value(row[m_dsocombomodel.m_dsocols.m_DSOminoraxis]);
            m_majoraxis.set_value(row[m_dsocombomodel.m_dsocols.m_DSOmajoraxis]);
            m_vmag.set_value(row[m_dsocombomodel.m_dsocols.m_DSOvmag]);
            m_dsotype.set_text(row[m_dsocombomodel.m_dsocols.m_DSOtype]);

            Astrocalc::astrocalc m_calc;
            m_bgsky.set_value(m_calc.calc_nelm_brightness_threshold_method(m_nelm1.get_value()));
        }
    }
}
