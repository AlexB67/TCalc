#include "starmagbox.hpp"

using AppGlobals::log_msg;

void MagBox::Magbox::set_signal_handlers()
{   
     nelm_connection = m_nelm.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::nelm_changed));
     sqm_connection = m_sqm.signal_value_changed().connect(sigc::mem_fun(*this, &Magbox::sqm_changed));
     m_sitetype.signal_changed().connect(sigc::mem_fun(*this, &Magbox::site_type_changed));
     m_colourbyname.signal_changed().connect(sigc::mem_fun(*this, &Magbox::star_colour_changed));
     
     m_age.signal_value_changed().connect([this]()
     {
        m_pupilsize.set_value(m_calc.calc_eyepupil_from_age(m_age.get_value()));
     });
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

