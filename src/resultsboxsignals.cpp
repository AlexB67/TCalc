#include "resultsbox.hpp"
#include "appglobals.hpp"

bool ResultsBox::Resultsbox::set_results_row_tooltip(int x, int y, bool keyboard_tooltip, 
                                                     const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
    Gtk::TreeModel::Path path;
    auto column = m_resultsview.get_column(0);
    int cellx, celly;
    int binx, biny;

    m_resultsview.convert_widget_to_bin_window_coords(x, y, binx, biny);
    
    if(false == m_resultsview.get_path_at_pos(binx, biny, path, column, cellx, celly))
        return false;

    const Gtk::TreeIter iter(m_resultsModel->get_iter(path));

    if(!iter)
        return false;

    const auto row = *iter;

    if (row[m_resultCols.m_results_property] == m_proplistnames[0]) // speed, see m_proplistnames below
        tooltip->set_markup(_("The speed of the telescope. Expressed as <i>f</i>/# . The focal length devided by the aperture."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[1]) //etc.
        tooltip->set_markup(_("The Magnification obtained from the selected eyepiece and telescope combination."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[2])
        tooltip->set_markup(_("Daw's limit. A measure of the telescope's resolving power."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[3]) 
        tooltip->set_markup(_("Rayleigh limit. A measure of the telescope's resolving power."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[4])
        tooltip->set_markup(_("The Field of view in degrees, obtained from the selected eyepiece and telescope combination."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[5]) 
        tooltip->set_markup(_("The exit pupil diameter, obtained from the selected eyepiece and telescope combination."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[6]) 
        tooltip->set_markup(_("The magnification for the selected telescope where the exit pupil is 4mm. "\
                              "An exit pupil in the range of 2 - 4 mm is considered optimal for most comfortable viewing."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[7]) 
        tooltip->set_markup(_("The magnification for the selected telescope where the exit pupil is 2mm. "\
                              "An exit pupil in the range of 2 - 4 mm is considered optimal for most comfortable viewing."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[8]) 
        tooltip->set_markup(_("The eyepiece focal length for the selected telescope where the exit pupil is 2mm. "\
                              "An exit pupil in the range of 2 - 4 mm is considered optimal for most comfortable viewing."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[9]) 
        tooltip->set_markup(_("The eyepiece focal length for the selected telescope where the exit pupil is 4mm. "\
                              "An exit pupil in the range of 2 - 4 mm is considered optimal for most comfortable viewing."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[10])
        tooltip->set_markup(_("The Highest useable magnification not limited by telescope optics, excluding seeing conditions."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[11])
        tooltip->set_markup(_("The lowest useable magnification where "\
                              "the exit pupil diameter does not exceed the observer's eyepupil diameter."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[12])
        tooltip->set_markup(_("The focal length corresponding to the low magnification limit."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[13])
        tooltip->set_markup(_("The focal length corresponding to the high magnification limit."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[14]) 
        tooltip->set_markup(_("Power per inch of aperture at the current magnification."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[15]) 
        tooltip->set_markup(_("The amount of light gathered by the telescope."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[16]) 
        tooltip->set_markup(_("The amount of light gathered by the telescope taking into acount obstruction size, reflectivity and "\
                              "transmission of the optical elements (not including the eyepiece)."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[17]) 
        tooltip->set_markup(_("How bright and object appears through the eyepiece. A value of 1 represents an exit pupil size "\
                              "equal to the observer's eye pupil size."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[18]) 
        tooltip->set_markup(_("The amount of time it takes for an object to drift across the eyepiece at an angle of 0 declination."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[19]) 
        tooltip->set_markup(_("The distance off-axis where optical abberations do not exceed the diffraction limit. Reflectors only."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[20]) 
        tooltip->set_markup(_("The faintest star that can be seen (on the magnitude scale). "\
                            "Larger numbers equate to fainter objects."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[21]) 
        tooltip->set_markup(_("An approximate indication whether a DSO is visisble through the telescope on a logarithmic scale (base 10). If the value is > 0"\
                            " the object is detectable. The larger the value the better the contrast against the sky background."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[22]) 
        tooltip->set_markup(_("Contrast factor. Depends on secondary mirror obstruction size. Higher numbers equate to improved contrast, "\
        "and sharper views, at zero obstruction size the maximum possible value is 5.25."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[23]) 
        tooltip->set_markup(_("The airy disk size, a smaller disk means more resolving power."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[24]) 
        tooltip->set_markup(_("The appoximate peak to value wavefront error, the smaller the value, the better the optical quality."\
        " Typically a value of ~0.25 is diffraction limited."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[25])
        tooltip->set_markup(_("The smallest possible object that can be seen on the moon (excluding seeing conditions)."));
    else if(row[m_resultCols.m_results_property] == m_proplistnames[26]) 
        tooltip->set_markup(_("A list of recommended beginner eyepiece focal lengths for the selected telescope."));
    else
        tooltip->set_markup(_("No tooltip available."));
    

    m_resultsview.set_tooltip_row(tooltip, path);

    return true;
}

bool ResultsBox::Resultsbox::set_scope_row_tooltip(int x, int y, bool keyboard_tooltip, 
                                                     const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
    Gtk::TreeModel::Path path;
    auto column = m_scopeview.get_column(0);
    int cellx, celly;
    int binx, biny;

    m_scopeview.convert_widget_to_bin_window_coords(x, y, binx, biny);
    
    if(false == m_scopeview.get_path_at_pos(binx, biny, path, column, cellx, celly))
        return false;

    const Gtk::TreeIter iter(m_scopeModel->get_iter(path));

    if(!iter)
        return false;

    const auto row = *iter;

    if (row[m_scopeCols.m_results_property] == m_scopelistnames[0])
    {
       Glib::ustring tip = _("The telescope model name");

       if (column->get_title() == _("Telescope data"))
       {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
       }
       else if (column->get_title() == _("Set 1"))
       {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ?
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
       }
       else if (column->get_title() == _("Set 2"))
       {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ?
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
       }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[1])
        tooltip->set_markup(_("The aperture of the telescope in mm"));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[2])
        tooltip->set_markup(_("The Focal length of the telescope in mm"));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[3])
        tooltip->set_markup(_("The Reflectifity of a reflector, or transmission for a refractor " 
	                              "as a percentage. A combination of both for Maksutov/SCT types."));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[4])
        tooltip->set_markup(_("The obtruction size as a percentage of the aperture diameter." 
							" Reflectors and SCT/Maksutov types only."));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[5])
        tooltip->set_markup(_("The type of telescope. Reflector, refractor or SCT/Mak."));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[6])
    {
        Glib::ustring tip = _("The coating used on mirrors");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[7])
    {
        Glib::ustring tip = _("The material used for mirror construction");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[8])
    {
        Glib::ustring tip = _("The type of lens coating");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[9])
       {
        Glib::ustring tip = _("The material (or glass) used for construction of lenses");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[10])
        tooltip->set_markup(_("A measure of the quality of telescope optics, the theoretical maximum value is 1, " 
                              "typically 0.80 - 0.98"));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[11])
        tooltip->set_markup(_("The weight of the telescope in kilograms (including mount)."));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[12])
        tooltip->set_markup(_("The weight of the mount in kilograms."));
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[13])
    {
        Glib::ustring tip = _("The type of mount. Dobsonian, EQ, alt-azimuth, etc");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[14])
    {
        Glib::ustring tip = _("Details of the focuser model");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else if (row[m_scopeCols.m_results_property] == m_scopelistnames[15])
    {
        Glib::ustring tip = _("Details of the finder");
        if (column->get_title() == _("Telescope data"))
        {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
        }
        else if (column->get_title() == _("Set 1"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set1]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
        else if (column->get_title() == _("Set 2"))
        {
            (static_cast<Glib::ustring>(row[m_scopeCols.m_results_set2]).length()) ? 
            tooltip->set_markup(row[m_scopeCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
        }
    }
    else
        tooltip->set_markup(_("No tooltip available."));
    

    m_scopeview.set_tooltip_row(tooltip, path);

    return true;
}

bool ResultsBox::Resultsbox::set_ep_row_tooltip(int x, int y, bool keyboard_tooltip, 
                                                    const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
    Gtk::TreeModel::Path path;
    auto column = m_epview.get_column(0);
    int cellx, celly;
    int binx, biny;

    m_epview.convert_widget_to_bin_window_coords(x, y, binx, biny);
    
    if(false == m_epview.get_path_at_pos(binx, biny, path, column, cellx, celly))
        return false;

    const Gtk::TreeIter iter(m_epModel->get_iter(path));

    if(!iter)
        return false;

    const auto row = *iter;

    if (row[m_epCols.m_results_property] == m_eplistnames[0])
    {
       Glib::ustring tip = _("The eyepiece Model name");

       if (column->get_title() == _("Eyepiece data"))
       {
           tooltip->set_markup(tip + _("."));
           m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
           return true;
       }
       else if (column->get_title() == _("Set 1"))
       {
            (static_cast<Glib::ustring>(row[m_epCols.m_results_set1]).length()) ?
            tooltip->set_markup(row[m_epCols.m_results_set1]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
       }
       else if (column->get_title() == _("Set 2"))
       {
            (static_cast<Glib::ustring>(row[m_epCols.m_results_set2]).length()) ?
            tooltip->set_markup(row[m_epCols.m_results_set2]) : tooltip->set_markup(tip + _("."));
            m_scopeview.set_tooltip_cell(tooltip, &path, nullptr, nullptr);
            return true;
       }
    }
    else if (row[m_epCols.m_results_property] == m_eplistnames[1])
        tooltip->set_markup(_("The field of view of the eyepiece in degrees."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[2])
        tooltip->set_markup(_("The focal length of the eyepiece in mm."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[3])
        tooltip->set_markup(_("The field stop diameter of the eyepiece in mm."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[4])
        tooltip->set_markup(_("The eye relief of the eyepiece in mm."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[5])
        tooltip->set_markup(_("The transmission of the eyepiece as a percentage."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[6])
        tooltip->set_markup(_("The barrel size of the eyepiece in inches."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[7])
        tooltip->set_markup(_("The optical design. Plossl, ortho, " 
                                "or other types specified as multi other."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[8])
        tooltip->set_markup(_("The number of groups in the eyepiece, a group is made up of one or more lenses."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[9])
        tooltip->set_markup(_("The total number of lenses or elements in the eyepiece."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[10])
        tooltip->set_markup(_("The weight in grams of the eyepiece."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[11])
        tooltip->set_markup(_("The type of coating used on optical elements. " 
        "FMC is fully multi coated. FMC-BE is fully multicoated with blackened edges. MC Multi coated. SC single coat etc."));
    else if (row[m_epCols.m_results_property] == m_eplistnames[12])
        tooltip->set_markup(_("The type of glass used for lens construction."));
    else
        tooltip->set_markup(_("No tooltip available."));
    

    m_epview.set_tooltip_row(tooltip, path);

    return true;
}