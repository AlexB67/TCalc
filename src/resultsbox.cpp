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
    m_resultsframe.set_label_align( Gtk::Align::CENTER);
    m_resultsframe.set_label_widget(m_resultsframelabel);
    m_resultsframelabel.set_markup(_("<b>Results</b>"));
    m_resultsframe.set_child(m_resultsgrid);
    Uidefs::set_ui_spacing(m_resultsgrid);
    m_resultsframe.set_hexpand(true);

    create_model_view(_("Calculated"), m_resultsview, m_resultsModel, m_resultCols, true);
    create_model_view(_("Eyepiece data"), m_epview, m_epModel, m_epCols, false);
    create_model_view(_("Telescope data"), m_scopeview, m_scopeModel, m_scopeCols, false);

    m_scrollwin.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_scrollwin.set_can_focus(false);
    m_scrollwin.set_child(m_resultsviewgrid);

    m_resultsviewframe.set_child(m_scrollwin);
  
    m_resultsviewgrid.set_row_spacing(2);
    m_resultsviewgrid.attach(m_resultsview, 0, 0);
    m_resultsviewgrid.attach(m_epview, 0, 1);
    m_resultsviewgrid.attach(m_scopeview, 0, 2);

    m_buttonleft.set_image_from_icon_name("go-previous-symbolic", Gtk::IconSize::INHERIT, true);
    m_buttonleft.set_sensitive(false);
    m_buttonleft.set_tooltip_text(_("The column to use for calculation results, set 1. Useful for comparison purposes."));
    m_buttonright.set_image_from_icon_name("go-next-symbolic", Gtk::IconSize::INHERIT, true);
    m_buttonright.set_tooltip_text(_("The column to use for calculation results, set 2. Useful for comparison purposes."));

    m_buttonbox.set_orientation(Gtk::Orientation::HORIZONTAL);
    m_buttonbox.append(m_buttonleft);
    m_buttonbox.append(m_buttonright);
    m_buttonbox.set_halign(Gtk::Align::CENTER);
    m_resultsgrid.attach(m_resultsviewframe, 0, 0, 7, 1);
    m_resultsgrid.attach(m_selectset1, 4, 1);
    m_resultsgrid.attach(m_buttonbox, 5, 1);
    m_resultsgrid.attach(m_selectset2, 6, 1);

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

    return m_resultsframe;
}

void Resultsbox::create_model_view( const Glib::ustring& header, Gtk::TreeView& view, 
                                    Glib::RefPtr<Gtk::ListStore>& model, const ModelCols& cols, const bool set_sort_column)
{
    model = Gtk::ListStore::create(cols);
    view.set_model(model);
    view.get_selection()->set_mode(Gtk::SelectionMode::NONE);
    view.set_hexpand(true);
    view.set_vexpand(true);
    view.set_grid_lines(Gtk::TreeView::GridLines::BOTH);
    view.set_has_tooltip(true);
    view.set_property("has-tooltip", true);
    view.append_column(header, m_renderertext);
    view.get_column(0)->add_attribute(m_renderertext.property_markup(), m_resultCols.m_results_property);
    
    if(true == set_sort_column)
    {
        view.set_reorderable(true);
        view.get_column(0)->set_sort_column(0);
        view.get_column(0)->set_sort_indicator(true); 
    }
    
    view.get_column(0)->set_resizable(true);
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
    m_renderertext2.property_ellipsize() = Pango::EllipsizeMode::END;
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const double value, 
                            const int precision,  const Glib::ustring &prefix, 
                            const Glib::ustring &postfix, const int resultsset) const
{ 
    size_t index = get_index(propertyname, m_resultsModel);
   
    if (index > m_proplistnames.size() || resultsset < 1 || resultsset > 2) return;

    Glib::ustring stmp = dtostr<double>(value, precision);
    m_resultsModel->children()[index].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[index].set_value<Glib::ustring>(resultsset, prefix + stmp + postfix);
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const Glib::ustring &text, const int resultsset) const
{
    size_t index = get_index(propertyname, m_resultsModel);

    if (index > m_proplistnames.size() || resultsset < 1 || resultsset > 2) return; 

    m_resultsModel->children()[index].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[index].set_value<Glib::ustring>(resultsset,  text);
}

void Resultsbox::append_row(const Glib::ustring &propertyname, const double value, 
                            const int precision,  const Glib::ustring &postfix, const int resultsset) const
{

    size_t index = get_index(propertyname, m_resultsModel);
    
    if (index > m_proplistnames.size() || resultsset < 1 || resultsset > 2 ) return;

    Glib::ustring stmp = dtostr<double>(value, precision);

    m_resultsModel->children()[index].set_value<Glib::ustring>(0, "<i>" + propertyname + "</i> :");
    m_resultsModel->children()[index].set_value<Glib::ustring>(resultsset, stmp + postfix);
}

void Resultsbox::clear(bool reset) const
{
    if (true == reset)
    {
        for (auto &iter : m_resultsModel->children())
        {
            iter.set_value<Glib::ustring>(1, "");
            iter.set_value<Glib::ustring>(2, ""); 
        }

        for (auto &iter : m_epModel->children())
        {
            iter.set_value<Glib::ustring>(1, "");
            iter.set_value<Glib::ustring>(2, ""); 
        }

        for (auto &iter : m_scopeModel->children())
        {
            iter.set_value<Glib::ustring>(1, "");
            iter.set_value<Glib::ustring>(2, ""); 
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
        _("<i>Brand</i> :"), _("<i>Model</i> :"), _("<i>Field of view</i> :"), _("<i>Focal length</i> :"),
        _("<i>field stop</i> :"), _("<i>Eye relief</i> :"), _("<i>Transmission</i> :"),
        _("<i>Barrel size</i> :"), _("<i>Type</i> :"), _("<i>Group #</i> :"),
        _("<i>Element #</i> :"), _("<i>Weight</i> :"), _("<i>Coating</i> :"),
        _("<i>Optical material</i> :")
    };

    m_scopelistnames = 
    {
        _("<i>Brand</i> :"), _("<i>Model</i> :"), _("<i>Aperture</i> :"), _("<i>Focal length</i> :"),
        _("<i>Reflectivity</i> :"), _("<i>Obstruction size</i> :"), 
        _("<i>Type</i> :"), _("<i>Mirror coating</i> :"), _("<i>Mirror material</i> :"),
        _("<i>Lens coating</i> :"), _("<i>Lens material</i> :"), _("<i>Strehl</i> :"), 
        _("<i>Total weight</i> :"), _("<i>Tube weight</i> :"), _("<i>Mount type</i> :"), 
        _("<i>Focuser details</i> :"), _("<i>Finder details</i> :")
    };

    for (auto& iter: m_proplistnames)
    {
        auto row = *(m_resultsModel->append()); 
        row[m_resultCols.m_results_property] = iter; 
    }

    for (auto& iter: m_eplistnames)
    {
        auto row = *(m_epModel->append()); 
        row[m_epCols.m_results_property] = iter; 
    }

    for (auto& iter: m_scopelistnames)
    {
        auto row = *(m_scopeModel->append()); 
        row[m_scopeCols.m_results_property] = iter; 
    }
}

void Resultsbox::get_ep_data(const std::shared_ptr<EpBox::Eyepiecebox>& epbox, const int resultsset) const
{
    bool flag = AppGlobals::LOGFLAG;
    Gtk::TreeModel::Row row;
    
    if (epbox->get_use_entry())
    {
        row = epbox->get_current_row(); // retrieve the current iterator row from model

        if(!row || (row && (epbox->m_emodelentry.get_text() 
                        != row.get_value(epbox->m_ecombomodel.m_epcols.m_epmodel)))) 
        {
            log_msg.emit(flag, LogView::tINFO, _("No Eyepiece selected for computation."));
            log_msg.emit(flag, LogView::tINFO, _("Using custom or default values."));

            m_epModel->children()[0].set_value<Glib::ustring>(resultsset, _("Not specified."));
            m_epModel->children()[1].set_value<Glib::ustring>(resultsset, _("Not specified."));
            
            Glib::ustring tmp;
            tmp = dtostr<double>(epbox->m_efov.get_value(), 2);
            m_epModel->children()[2].set_value<Glib::ustring>(resultsset, tmp + "<sup>o</sup>");

            tmp = dtostr<double>(epbox->m_eflen.get_value(), 2);
            m_epModel->children()[3].set_value<Glib::ustring>(resultsset, tmp + "mm");

            tmp = dtostr<double>(epbox->m_efstop.get_value(), 2);
            (epbox->m_efstop.get_value() < Astrocalc::astrocalc::tSMALL) ?
            m_epModel->children()[4].set_value<Glib::ustring>(resultsset, _("unknown")) :
            m_epModel->children()[4].set_value<Glib::ustring>(resultsset, tmp + _("mm"));


            tmp = dtostr<double>(epbox->m_erelief.get_value(), 2);
            (epbox->m_erelief.get_value() < Astrocalc::astrocalc::tSMALL) ?
            m_epModel->children()[5].set_value<Glib::ustring>(resultsset, _("unknown")) :
            m_epModel->children()[5].set_value<Glib::ustring>(resultsset, tmp + _("mm"));

            tmp = dtostr<double>(epbox->m_etrans.get_value(), 2);
            m_epModel->children()[6].set_value<Glib::ustring>(resultsset, tmp + "%");

            for (int i = 7; i < 14; ++i)
                 m_epModel->children()[i].set_value<Glib::ustring>(resultsset,  _("unknown"));
            
            return;
        }
    }
    else
    {
        const Gtk::TreeModel::iterator iter = epbox->m_emodel->get_active();
        if(!iter) return;

        row = *iter;
    }

    auto set_row = [this](const Glib::ustring& str, const Glib::ustring& unit, const int row, const int resultsset)
    {
        (str.length() && str != "0") ?
        m_epModel->children()[row].set_value<Glib::ustring>(resultsset, str + unit) :
        m_epModel->children()[row].set_value<Glib::ustring>(resultsset, _("unknown"));
    };

    const auto& parent_iter = row.parent();
    parent_iter->get_value(epbox->m_ecombomodel.m_epcols.m_epbrand);

    Glib::ustring stmp = parent_iter->get_value(epbox->m_ecombomodel.m_epcols.m_epbrand);
    m_epModel->children()[0].set_value<Glib::ustring>(resultsset, stmp);

    stmp = row.get_value(epbox->m_ecombomodel.m_epcols.m_epmodel);
    m_epModel->children()[1].set_value<Glib::ustring>(resultsset, stmp);

    stmp = dtostr<double>(epbox->m_efov.get_value(), 2);
    m_epModel->children()[2].set_value<Glib::ustring>(resultsset, stmp + "<sup>o</sup>");

    stmp = dtostr<double>(epbox->m_eflen.get_value(), 2);
    m_epModel->children()[3].set_value<Glib::ustring>(resultsset, stmp + "mm");

    stmp = dtostr<double>(epbox->m_efstop.get_value(), 2);
    (epbox->m_efstop.get_value() < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[4].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[4].set_value<Glib::ustring>(resultsset, stmp + _("mm"));


    stmp = dtostr<double>(epbox->m_erelief.get_value(), 2);
    (epbox->m_erelief.get_value() < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[5].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[5].set_value<Glib::ustring>(resultsset, stmp + _("mm"));

    stmp = dtostr<double>(epbox->m_etrans.get_value(), 2);
    m_epModel->children()[6].set_value<Glib::ustring>(resultsset, stmp + "%");

    stmp = dtostr<double>(row[epbox->m_ecombomodel.m_epcols.m_epbarrel], 2);
    m_epModel->children()[7].set_value<Glib::ustring>(resultsset, stmp + "<i>\"</i>");


    stmp = epbox->m_etype.get_active_text();
    set_row(stmp, "", 8, resultsset);

    stmp = dtostr<int>(row[epbox->m_ecombomodel.m_epcols.m_epgroups], 0);
    set_row(stmp, "", 9, resultsset);

    stmp = dtostr<int>(row[epbox->m_ecombomodel.m_epcols.m_epelements], 0);
    set_row(stmp, "", 10, resultsset);

    stmp = dtostr<double>(row[epbox->m_ecombomodel.m_epcols.m_epweight], 2);
    (row[epbox->m_ecombomodel.m_epcols.m_epweight] < Astrocalc::astrocalc::tSMALL) ?
    m_epModel->children()[11].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_epModel->children()[11].set_value<Glib::ustring>(resultsset, stmp + _("g"));

    stmp = row[epbox->m_ecombomodel.m_epcols.m_epcoating];
    set_row(stmp, "", 12, resultsset);

    stmp = row[epbox->m_ecombomodel.m_epcols.m_epmaterial];
    set_row(stmp, "", 13, resultsset);
}

void Resultsbox::get_scope_data(const std::shared_ptr<ScopeBox::Telescopebox> &scopebox, const int resultsset) const
{
    bool flag = AppGlobals::LOGFLAG;
    Gtk::TreeModel::Row row;
    
    if (scopebox->get_use_entry())
    {
        row = scopebox->get_current_row(); // retrieve the current iterator row from model

        if(!row || (row && (scopebox->m_smodelentry.get_text() 
                        != row.get_value(scopebox->m_scombomodel.m_scopecols.m_smodel)))) 
        {
            log_msg.emit(flag, LogView::tINFO, _("No Telescope selected for computation."));
            log_msg.emit(flag, LogView::tINFO, _("Using custom or default values."));

            m_scopeModel->children()[0].set_value<Glib::ustring>(resultsset, _("Not specified."));
            m_scopeModel->children()[1].set_value<Glib::ustring>(resultsset, _("Not specified."));

            Glib::ustring tmp;
            tmp = dtostr<double>(scopebox->m_saperture.get_value(), 2);
            m_scopeModel->children()[2].set_value<Glib::ustring>(resultsset, tmp + "mm");

            tmp = dtostr<double>(scopebox->m_sflen.get_value(), 2);
            m_scopeModel->children()[3].set_value<Glib::ustring>(resultsset, tmp + "mm");

            if (_("Reflector") == scopebox->m_stype.get_active_text())
                m_scopeModel->children()[4].set_value<Glib::ustring>(0, _("<i>Reflectivity</i> :"));
            else if (_("Refractor") == scopebox->m_stype.get_active_text())
                m_scopeModel->children()[4].set_value<Glib::ustring>(0, _("<i>Transmission</i> :"));
            else if (_("SCT/Mak") == scopebox->m_stype.get_active_text())
                m_scopeModel->children()[4].set_value<Glib::ustring>(0, _("<i>Trans/Reflect</i> :"));
            
            tmp = dtostr<double>(scopebox->m_sreflect.get_value(), 2);
            m_scopeModel->children()[4].set_value<Glib::ustring>(resultsset, tmp + "%");

            tmp = dtostr<double>(scopebox->m_sobstruct.get_value(), 2);
            m_scopeModel->children()[5].set_value<Glib::ustring>(resultsset, tmp + "%");

            m_scopeModel->children()[6].set_value<Glib::ustring>(resultsset, scopebox->m_stype.get_active_text());

            for (int i = 7; i < 17; ++i)
                 m_scopeModel->children()[i].set_value<Glib::ustring>(resultsset,  _("unknown"));
            
            log_msg.emit(flag, LogView::tINFO, _("Calculation completed:"));
            return;
        }
    }
    else
    {
        const Gtk::TreeModel::iterator iter = scopebox->m_smodel->get_active();
        if(!iter) return;

        row = *iter;
    }

    auto set_row = [this](const Glib::ustring& str, const Glib::ustring& unit, const int row, const int resultsset)
    {
        (str.length() && str != "0") ?
        m_scopeModel->children()[row].set_value<Glib::ustring>(resultsset, str + unit) :
        m_scopeModel->children()[row].set_value<Glib::ustring>(resultsset, _("unknown"));
    };
    
    const auto& parent_iter = row.parent();
    Glib::ustring stmp = parent_iter->get_value(scopebox->m_scombomodel.m_scopecols.m_sbrand);
    m_scopeModel->children()[0].set_value<Glib::ustring>(resultsset, stmp);
    
    stmp = static_cast<Glib::ustring>(row[scopebox->m_scombomodel.m_scopecols.m_smodel]);
    m_scopeModel->children()[1].set_value<Glib::ustring>(resultsset, stmp);

    stmp = dtostr<double>(scopebox->m_saperture.get_value(), 2);
    m_scopeModel->children()[2].set_value<Glib::ustring>(resultsset, stmp + "mm");

    stmp = dtostr<double>(scopebox->m_sflen.get_value(), 2);
    m_scopeModel->children()[3].set_value<Glib::ustring>(resultsset, stmp + "mm");

    
    if (_("Reflector") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[4].set_value<Glib::ustring>(0,  _("<i>Reflectivity</i> :"));
    else if (_("Refractor") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[4].set_value<Glib::ustring>(0, _("<i>Transmission</i> :"));
    else if (_("SCT/Mak") == scopebox->m_stype.get_active_text()) 
        m_scopeModel->children()[4].set_value<Glib::ustring>(0, _("<i>Trans/Reflect</i> :"));

    stmp = dtostr<double>(scopebox->m_sreflect.get_value(), 2);
    m_scopeModel->children()[4].set_value<Glib::ustring>(resultsset, stmp + "%");

    stmp = dtostr<double>(scopebox->m_sobstruct.get_value(), 2);
    m_scopeModel->children()[5].set_value<Glib::ustring>(resultsset, stmp + "%");

    m_scopeModel->children()[6].set_value<Glib::ustring>(resultsset, scopebox->m_stype.get_active_text());

    if(scopebox->m_stype.get_active_row_number() != Astrocalc::astrocalc::REFRACTOR) 
    {
        stmp = row[scopebox->m_scombomodel.m_scopecols.m_smirrorcoating];
        set_row(stmp, "", 7, resultsset);

        stmp = row[scopebox->m_scombomodel.m_scopecols.m_smirrormaterial];
        set_row(stmp, "", 8, resultsset);
    }

    if(scopebox->m_stype.get_active_row_number() != Astrocalc::astrocalc::REFLECTOR) 
    {
        stmp = row[scopebox->m_scombomodel.m_scopecols.m_slenscoating];
        set_row(stmp, "", 9, resultsset);

         stmp = row[scopebox->m_scombomodel.m_scopecols.m_slensmaterial];
        set_row(stmp, "", 10, resultsset);
    }

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_sstrehl], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_sstrehl] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[11].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[11].set_value<Glib::ustring>(resultsset, stmp + "");

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_sweight], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_sweight] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[12].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[12].set_value<Glib::ustring>(resultsset, stmp + _("kg"));

    stmp = dtostr<double>(row[scopebox->m_scombomodel.m_scopecols.m_stube_weight], 2);
    (row[scopebox->m_scombomodel.m_scopecols.m_stube_weight] < Astrocalc::astrocalc::tSMALL) ?
    m_scopeModel->children()[13].set_value<Glib::ustring>(resultsset, _("unknown")) :
    m_scopeModel->children()[13].set_value<Glib::ustring>(resultsset, stmp + _("kg"));

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_smount_type];
    set_row(stmp, "", 14, resultsset);

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_sfocuser_type];
    set_row(stmp, "", 15, resultsset);

    stmp = row[scopebox->m_scombomodel.m_scopecols.m_sfinder_type];
    set_row(stmp, "", 16, resultsset);

    log_msg.emit(flag, LogView::tINFO, _("Calculation completed:"));
}

size_t Resultsbox::get_index(const Glib::ustring &propertyname, const Glib::RefPtr<Gtk::ListStore>& liststore) const
{
    size_t index = 0;
    for(auto &i : liststore->children())
    {
        if (i.get_value(m_resultCols.m_results_property) == "<i>" + propertyname + "</i> :") break;
        ++index;
    }

    return index;
}