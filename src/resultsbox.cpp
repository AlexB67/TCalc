#include "resultsbox.hpp"
#include "gtkmmcustomutils.hpp"
#include "glibmmcustomutils.hpp"
#include "appglobals.hpp"
#include "astrocalclib/astrocalc.hpp"

using GlibUtils::dtostr;
using namespace ResultsBox;
using AppGlobals::log_msg;

Gtk::Frame &Resultsbox::create_results_grid()
{
    m_resultsframe.set_label_align( Gtk::ALIGN_CENTER,  Gtk::ALIGN_CENTER);
    m_resultsframe.set_label_widget(m_resultsframelabel);
    m_resultsframelabel.set_markup(_("<b>Results</b>"));
    m_resultsframe.add(m_resultsgrid);
    Uidefs::set_ui_spacing(m_resultsgrid);
    m_resultsframe.set_hexpand(true);

    create_model_view(_("Calculated"), m_resultsview, m_resultsModel, m_resultCols);
    create_model_view(_("Eyepiece data"), m_epview, m_epModel, m_epCols);
    create_model_view(_("Telescope data"), m_scopeview, m_scopeModel, m_scopeCols);

    m_scrollwin.set_policy(Gtk::PolicyType::POLICY_NEVER, Gtk::PolicyType::POLICY_ALWAYS);
    m_scrollwin.set_can_focus(false);
    m_scrollwin.add(m_resultsviewgrid);

    m_resultsviewframe.add(m_scrollwin);
  
    m_resultsviewgrid.set_row_spacing(2);
    m_resultsviewgrid.attach(m_resultsview, 0, 0);
    m_resultsviewgrid.attach(m_epview, 0, 1);
    m_resultsviewgrid.attach(m_scopeview, 0, 2);

    m_buttonleft.set_image_from_icon_name("go-previous-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_buttonleft.set_sensitive(false);
    m_buttonleft.set_tooltip_text(_("The column to use for calculation results, set 1. Useful for comparison purposes."));
    m_buttonright.set_image_from_icon_name("go-next-symbolic", Gtk::ICON_SIZE_BUTTON, true);
    m_buttonright.set_tooltip_text(_("The column to use for calculation results, set 2. Useful for comparison purposes."));

    m_buttonbox.set_layout(Gtk::BUTTONBOX_EXPAND);
    m_buttonbox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_buttonbox.add(m_buttonleft);
    m_buttonbox.add(m_buttonright);
   
    m_resultsgrid.attach(m_resultsviewframe, 0, 0, 3, 1);
    m_resultsgrid.attach(m_selectset1,  0, 1);
    m_resultsgrid.attach(m_buttonbox, 1, 1);
    m_resultsgrid.attach(m_selectset2, 2, 1);

    m_resultsview.signal_query_tooltip().connect(sigc::mem_fun(*this, &Resultsbox::set_results_row_tooltip), true);
    m_scopeview.signal_query_tooltip().connect(sigc::mem_fun(*this, &Resultsbox::set_scope_row_tooltip), true);
    m_epview.signal_query_tooltip().connect(sigc::mem_fun(*this, &Resultsbox::set_ep_row_tooltip), true);

    m_buttonright.signal_clicked().connect([this]()
    {
        m_buttonleft.set_sensitive(true);
        m_buttonright.set_sensitive(false);
        AppGlobals::resultsset = 2;
        log_msg.emit(AppGlobals::LOGFLAG, LogView::tINFO, _("Results set 2 selected."));
    });

    m_buttonleft.signal_clicked().connect([this]()
    {
        m_buttonleft.set_sensitive(false);
        m_buttonright.set_sensitive(true);
        AppGlobals::resultsset = 1;
        log_msg.emit(AppGlobals::LOGFLAG, LogView::tINFO, _("Results set 1 selected."));
    });

    init_property_names();

    AppGlobals::get_keyfile_config(m_resultsframe);
    AppGlobals::frame_style.connect([this](Gtk::ShadowType type){ AppGlobals::set_frame_style(m_resultsframe, type);});

    return m_resultsframe;
}

void Resultsbox::create_model_view( const Glib::ustring& header, Gtk::TreeView& view, 
                                    Glib::RefPtr<Gtk::ListStore>& model, const ModelCols& cols)
{
    model = Gtk::ListStore::create(cols);
    view.set_model(model);
    view.get_selection()->set_mode(Gtk::SELECTION_NONE);
    view.set_hexpand(true);
    view.set_vexpand(true);
    view.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
    view.set_has_tooltip(true);

    view.set_property("has-tooltip", true);
    view.append_column(header, m_renderertext);
    view.get_column(0)->add_attribute(m_renderertext.property_markup(), m_resultCols.m_results_property);
    view.get_column(0)->set_sort_column(0);
    view.get_column(0)->set_resizable(true);
    view.get_column(0)->set_sort_indicator(true);
    view.get_column(0)->set_min_width(180);
    view.get_column(0)->set_expand(true);

    view.append_column(_("Set 1"), m_renderertext2);
    view.get_column(1)->add_attribute(m_renderertext2.property_markup(), m_resultCols.m_results_set1);
    view.get_column(1)->set_resizable(true);
    view.get_column(1)->set_min_width(120);
    view.get_column(1)->set_expand(true);
  
    view.append_column(_("Set 2"), m_renderertext2);
    view.get_column(2)->add_attribute(m_renderertext2.property_markup(), m_resultCols.m_results_set2);
    view.get_column(2)->set_resizable(true);
    view.get_column(2)->set_min_width(120);
    view.get_column(2)->set_expand(true);

    m_renderertext.set_padding(10, 5);
    m_renderertext2.property_ellipsize() = Pango::ELLIPSIZE_END;
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const double value, 
                            const int precision,  const Glib::ustring &prefix, 
                            const Glib::ustring &postfix, const int resultsset)
{
    if (rowcount > m_proplistnames.size() || resultsset < 1 || resultsset > 2) return; 

    Glib::ustring stmp = dtostr<double>(value, precision);

    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(resultsset, prefix + stmp + postfix);
    ++rowcount;
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const Glib::ustring &text, const int resultsset)
{
    if (rowcount > m_proplistnames.size() || resultsset < 1 || resultsset > 2) return; 

    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(resultsset,  text);
    ++rowcount;
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const double value, 
                                        const int precision,  const Glib::ustring &postfix, const int resultsset)
{
    if (rowcount > m_proplistnames.size() || resultsset < 1 || resultsset > 2 ) return;

    Glib::ustring stmp = dtostr<double>(value, precision);

    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[rowcount].set_value<Glib::ustring>(resultsset, stmp + postfix);
    ++rowcount;
}

void Resultsbox::clear(bool reset)
{
    rowcount = 0;

    if (true == reset)
    {
        for (auto &iter : m_resultsModel->children())
        {
            iter->set_value<Glib::ustring>(1, "");
            iter->set_value<Glib::ustring>(2, ""); 
        }

        for (auto &iter : m_epModel->children())
        {
            iter->set_value<Glib::ustring>(1, "");
            iter->set_value<Glib::ustring>(2, ""); 
        }

        for (auto &iter : m_scopeModel->children())
        {
            iter->set_value<Glib::ustring>(1, "");
            iter->set_value<Glib::ustring>(2, ""); 
        }
    }
}

void Resultsbox::init_property_names()
{
    // populate property names

    m_proplistnames = 
    {
        _("<i>Speed</i> :"), _("<i>Magnification</i> :"), _("<i>Daw's limit</i> :"),
        _("<i>Rayleigh's limit</i> :"), _("<i>Field of view</i> :"), _("<i>Exit pupil</i> :"),
        _("<i>Opt min mag</i> :"), _("<i>Opt max mag</i> :"), _("<i>Opt min focal length</i> :"),
        _("<i>Opt max focal length</i> :"), _("<i>High mag limit</i> :"), _("<i>Low mag limit</i> :"), 
        _("<i>Max focal length</i> :"), _("<i>Min focal length</i> :"), _("<i>PPI</i> :"), _("<i>Light grasp</i> :"), 
        _("<i>Light grasp effective</i> :"), _("<i>Brightness factor</i> :"), _("<i>Drift time</i> :"), 
        _("<i>Collimation tol</i> :"),_("<i>Limiting magnitude</i> :"), _("<i>Visibility threshold</i> :"), 
        _("<i>Contrast factor</i> :"), _("<i>Airy disk diam</i> :"), _("<i>PV from Strehl</i> :"),
        _("<i>Lunar resolution</i> :"),_("<i>Ocular list</i> :")
    };

    m_eplistnames = 
    {
        _("<i>Model</i> :"), _("<i>Field of view</i> :"), _("<i>Focal length</i> :"),
        _("<i>field stop</i> :"), _("<i>Eye relief</i> :"), _("<i>Transmission</i> :"),
        _("<i>Barrel size</i> :"), _("<i>Type</i> :"), _("<i>Group #</i> :"),
        _("<i>Element #</i> :"), _("<i>Weight</i> :"), _("<i>Coating</i> :"),
        _("<i>Optical material</i> :")
    };

    m_scopelistnames = 
    {
        _("<i>Model</i> :"), _("<i>Aperture</i> :"), _("<i>Focal length</i> :"),
        _("<i>Reflectivity</i> :"), _("<i>Obstruction size</i> :"), 
        _("<i>Type</i> :"), _("<i>Mirror coating</i> :"), _("<i>Mirror material</i> :"),
        _("<i>Lens coating</i> :"), _("<i>Lens material</i> :"), _("<i>Strehl Ratio</i> :"), 
        _("<i>Total weight</i> :"), _("<i>Mount weight</i> :"), _("<i>Mount type</i> :"), 
        _("<i>Focuser details</i> :"), _("<i>Finder details</i> :") 
    };

    for (auto& iter: m_proplistnames)
    {
        const auto row = *(m_resultsModel->append()); 
        row[m_resultCols.m_results_property] = iter; 
    }

    for (auto& iter: m_eplistnames)
    {
        const auto row = *(m_epModel->append()); 
        row[m_epCols.m_results_property] = iter; 
    }

    for (auto& iter: m_scopelistnames)
    {
        const auto row = *(m_scopeModel->append()); 
        row[m_scopeCols.m_results_property] = iter; 
    }
}

void Resultsbox::get_ep_data(const std::shared_ptr<EpBox::Eyepiecebox>& epbox, const int resultsset)
{
    const Gtk::TreeModel::iterator iter = epbox->m_emodel->get_active();

    if(!iter) return;

    const auto row = *iter;

    auto set_row = [this](const Glib::ustring& str, const Glib::ustring& unit, const int row, const int resultsset)
    {
        (str.length() && str != "0") ?
        m_epModel->children()[row].set_value<Glib::ustring>(resultsset, str + unit) :
        m_epModel->children()[row].set_value<Glib::ustring>(resultsset, _("unknown"));
    };

    Glib::ustring stmp = static_cast<Glib::ustring>(row[epbox->m_ecombomodel.m_epcols.m_epmodel]);
    m_epModel->children()[0].set_value<Glib::ustring>(resultsset, stmp);

    stmp = dtostr<double>(epbox->m_efov.get_value(), 2);
    m_epModel->children()[1].set_value<Glib::ustring>(resultsset, stmp + "<sup>o</sup>");

    stmp = dtostr<double>(epbox->m_eflen.get_value(), 2);
    m_epModel->children()[2].set_value<Glib::ustring>(resultsset, stmp + "mm");

    stmp = dtostr<double>(epbox->m_efstop.get_value(), 2);
    (epbox->m_efstop.get_value() < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[3].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[3].set_value<Glib::ustring>(resultsset, stmp + _("mm"));


    stmp = dtostr<double>(epbox->m_erelief.get_value(), 2);
    (epbox->m_erelief.get_value() < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[4].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[4].set_value<Glib::ustring>(resultsset, stmp + _("mm"));

    stmp = dtostr<double>(epbox->m_etrans.get_value(), 2);
    m_epModel->children()[5].set_value<Glib::ustring>(resultsset, stmp + "%");

    stmp = dtostr<double>(row[epbox->m_ecombomodel.m_epcols.m_epbarrel], 2);
    m_epModel->children()[6].set_value<Glib::ustring>(resultsset, stmp + "<i>\"</i>");


    stmp = epbox->m_etype.get_active_text();
    set_row(stmp, "", 7, resultsset);

    stmp = dtostr<int>(row[epbox->m_ecombomodel.m_epcols.m_epgroups], 0);
    set_row(stmp, "", 8, resultsset);

    stmp = dtostr<int>(row[epbox->m_ecombomodel.m_epcols.m_epelements], 0);
    set_row(stmp, "", 9, resultsset);

    stmp = dtostr<double>(row[epbox->m_ecombomodel.m_epcols.m_epweight], 2);
    (row[epbox->m_ecombomodel.m_epcols.m_epweight] < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[10].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[10].set_value<Glib::ustring>(resultsset, stmp + _("g"));

    stmp = row[epbox->m_ecombomodel.m_epcols.m_epcoating];
    set_row(stmp, "", 11, resultsset);

    stmp = row[epbox->m_ecombomodel.m_epcols.m_epmaterial];
    set_row(stmp, "", 12, resultsset);
}

void Resultsbox::get_scope_data(const std::shared_ptr<ScopeBox::Telescopebox> &scopebox, const int resultsset)
{
    const auto iter = scopebox->m_smodel->get_active();

    if(!iter) return;
    
    const auto row = *iter;

    auto set_row = [this](const Glib::ustring& str, const Glib::ustring& unit, const int row, const int resultsset)
    {
        (str.length() && str != "0") ?
        m_scopeModel->children()[row].set_value<Glib::ustring>(resultsset, str + unit) :
        m_scopeModel->children()[row].set_value<Glib::ustring>(resultsset, _("unknown"));
    };

    Glib::ustring stmp = static_cast<Glib::ustring>(row[scopebox->m_scombomodel.m_scopecols.m_smodel]);
    m_scopeModel->children()[0].set_value<Glib::ustring>(resultsset, stmp);

    stmp = dtostr<double>(scopebox->m_saperture.get_value(), 2);
    m_scopeModel->children()[1].set_value<Glib::ustring>(resultsset, stmp + "mm");

    stmp = dtostr<double>(scopebox->m_sflen.get_value(), 2);
    m_scopeModel->children()[2].set_value<Glib::ustring>(resultsset, stmp + "mm");

    
    if (_("Reflector") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[3].set_value<Glib::ustring>(0,  _("<i>Reflectivity</i> :"));
    else if (_("Refractor") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[3].set_value<Glib::ustring>(0, _("<i>Transmission</i> :"));
    else if (_("SCT/Mak") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[3].set_value<Glib::ustring>(0, _("<i>Trans/Reflect</i> :"));

    stmp = dtostr<double>(scopebox->m_sreflect.get_value(), 2);
    m_scopeModel->children()[3].set_value<Glib::ustring>(resultsset, stmp + "%");

    stmp = dtostr<double>(scopebox->m_sobstruct.get_value(), 2);
    m_scopeModel->children()[4].set_value<Glib::ustring>(resultsset, stmp + "%");

    m_scopeModel->children()[5].set_value<Glib::ustring>(resultsset, scopebox->m_stype.get_active_text());

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_smirrorcoating];
    set_row(stmp, "", 6, resultsset);

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_smirrormaterial];
    set_row(stmp, "", 7, resultsset);

    if(scopebox->m_stype.get_active_row_number() != Astrocalc::astrocalc::REFLECTOR) 
    {
        stmp = row[scopebox->m_scombomodel.m_scopecols.m_slenscoating];
        set_row(stmp, "", 8, resultsset);

         stmp = row[scopebox->m_scombomodel.m_scopecols.m_slensmaterial];
        set_row(stmp, "", 9, resultsset);
    }

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_sstrehl], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_sstrehl] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[10].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[10].set_value<Glib::ustring>(resultsset, stmp + "");

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_sweight], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_sweight] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[11].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[11].set_value<Glib::ustring>(resultsset, stmp + _("kg"));

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_smount_weight], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_sweight] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[12].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[12].set_value<Glib::ustring>(resultsset, stmp + _("kg"));

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_smount_type];
    set_row(stmp, "", 13, resultsset);

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_sfocuser_type];
    set_row(stmp, "", 14, resultsset);

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_sfinder_type];
    set_row(stmp, "", 15, resultsset);
}
