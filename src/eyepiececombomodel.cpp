#include "eyepiececombomodel.hpp"
#include <iostream>

using namespace EpCombo;

void EpCombomodel::create_ep_model()
{
    m_eptreemodel = Gtk::ListStore::create(m_epcols);
}

bool EpCombomodel::on_separator(const Glib::RefPtr<Gtk::TreeModel>& model, const Gtk::TreeModel::iterator& iter)
{
    if (iter)
    {
        const auto row = *iter;
        if ("separator" == row[m_epcols.m_eptype]) return true; // reuse the type column for separators,
    }

    return false;
}

void EpCombomodel::swap_ep_rows(const Glib::ustring& epname, bool movedown) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator previousiter;

    for (iter = m_eptreemodel->children().begin(); iter != m_eptreemodel->children().end(); ++iter)
    {
        if (epname == iter->get_value(m_epcols.m_epmodel))
        {
            previousiter = iter;
            break;
        }
    }

    if (true == movedown) 
        iter++; 
    else 
        iter--;

    const Gtk::TreeRow prow = *previousiter;
    const Gtk::TreeRow row = *iter;

    std::tuple<double, double, double, double, double, double, Glib::ustring, int, int, double, 
    Glib::ustring, Glib::ustring> epdata = 
    {
        prow[m_epcols.m_epfov], prow[m_epcols.m_epflen], prow[m_epcols.m_epfstop], prow[m_epcols.m_eprelief], 
        prow[m_epcols.m_eptrans], prow[m_epcols.m_epbarrel], prow[m_epcols.m_eptype], prow[m_epcols.m_epgroups],
        prow[m_epcols.m_epelements], prow[m_epcols.m_epweight], prow[m_epcols.m_epcoating], 
        prow[m_epcols.m_epmaterial]
    }; // note, we omit the model name 

    if (iter && previousiter) // swap values of the two rows
    {
        prow[m_epcols.m_epmodel] = static_cast<Glib::ustring>(row[m_epcols.m_epmodel]);
        prow[m_epcols.m_epfov] = static_cast<double>(row[m_epcols.m_epfov]);
        prow[m_epcols.m_epflen] = static_cast<double>(row[m_epcols.m_epflen]);
        prow[m_epcols.m_epfstop] = static_cast<double>(row[m_epcols.m_epfstop]);
        prow[m_epcols.m_eprelief] = static_cast<double>(row[m_epcols.m_eprelief]);
        prow[m_epcols.m_eptrans] = static_cast<double>(row[m_epcols.m_eptrans]);
        prow[m_epcols.m_epbarrel] = static_cast<double>(row[m_epcols.m_epbarrel]);
        prow[m_epcols.m_eptype] = static_cast<Glib::ustring>(row[m_epcols.m_eptype]);
        prow[m_epcols.m_epgroups] = static_cast<int>(row[m_epcols.m_epgroups]);
        prow[m_epcols.m_epelements] = static_cast<int>(row[m_epcols.m_epelements]);
        prow[m_epcols.m_epweight] = static_cast<double>(row[m_epcols.m_epweight]);
        prow[m_epcols.m_epcoating] = static_cast<Glib::ustring>(row[m_epcols.m_epcoating]);
        prow[m_epcols.m_epmaterial] = static_cast<Glib::ustring>(row[m_epcols.m_epmaterial]);

        row[m_epcols.m_epmodel] = epname;
        row[m_epcols.m_epfov] = std::get<0>(epdata);
        row[m_epcols.m_epflen] = std::get<1>(epdata);
        row[m_epcols.m_epfstop] = std::get<2>(epdata);
        row[m_epcols.m_eprelief] = std::get<3>(epdata);
        row[m_epcols.m_eptrans] = std::get<4>(epdata);
        row[m_epcols.m_epbarrel] = std::get<5>(epdata);
        row[m_epcols.m_eptype] = std::get<6>(epdata);
        row[m_epcols.m_epgroups] = std::get<7>(epdata);
        row[m_epcols.m_epelements] = std::get<8>(epdata);
        row[m_epcols.m_epweight] = std::get<9>(epdata);
        row[m_epcols.m_epcoating] = std::get<10>(epdata);
        row[m_epcols.m_epmaterial] = std::get<11>(epdata);
        m_epcombo->set_active(previousiter);
    }
}

void EpCombomodel::append_ep_to_model(const std::tuple<Glib::ustring, double, double, double, double,
                                        double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                        Glib::ustring>& epdata) const
{
    const Gtk::TreeModel::Row row = *(m_eptreemodel->append());
    row[m_epcols.m_epmodel] = std::get<0>(epdata);
    row[m_epcols.m_epfov] = std::get<1>(epdata);
    row[m_epcols.m_epflen] = std::get<2>(epdata);
    row[m_epcols.m_epfstop] = std::get<3>(epdata);
    row[m_epcols.m_eprelief] = std::get<4>(epdata);
    row[m_epcols.m_eptrans] = std::get<5>(epdata);
    row[m_epcols.m_epbarrel] = std::get<6>(epdata);
    row[m_epcols.m_eptype] = std::get<7>(epdata);
    row[m_epcols.m_epgroups] = std::get<8>(epdata);
    row[m_epcols.m_epelements] = std::get<9>(epdata);
    row[m_epcols.m_epweight] = std::get<10>(epdata);
    row[m_epcols.m_epcoating] = std::get<11>(epdata);
    row[m_epcols.m_epmaterial] = std::get<12>(epdata);
}

void EpCombomodel::add_ep_to_model(const std::tuple<Glib::ustring, double, double, double, double,
                                    double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                    Glib::ustring>& epdata, bool append) const
{

    Gtk::TreeModel::iterator iter;

    for (iter = m_eptreemodel->children().begin(); iter != m_eptreemodel->children().end(); ++iter)
        if ("separator" == (*iter)[m_epcols.m_eptype]) break;

    if (iter)
    {
        Gtk::TreeRow row;
        (true == append) ? row = *(m_eptreemodel->append()) : row = *(m_eptreemodel->insert(*iter));
        row[m_epcols.m_epmodel] = std::get<0>(epdata);
        row[m_epcols.m_epfov] = std::get<1>(epdata);
        row[m_epcols.m_epflen] = std::get<2>(epdata);
        row[m_epcols.m_epfstop] = std::get<3>(epdata);
        row[m_epcols.m_eprelief] = std::get<4>(epdata);
        row[m_epcols.m_eptrans] = std::get<5>(epdata);
        row[m_epcols.m_epbarrel] = std::get<6>(epdata);
        row[m_epcols.m_eptype] = std::get<7>(epdata);
        row[m_epcols.m_epgroups] = std::get<8>(epdata);
        row[m_epcols.m_epelements] = std::get<9>(epdata);
        row[m_epcols.m_epweight] = std::get<10>(epdata);
        row[m_epcols.m_epcoating] = std::get<11>(epdata);
        row[m_epcols.m_epmaterial] = std::get<12>(epdata);
    }
}

void EpCombomodel::remove_ep_from_model(const Glib::ustring &epname) const
{
    for (auto &i : m_eptreemodel->children())
    {
        if (epname == i->get_value(m_epcols.m_epmodel))
        {
            m_eptreemodel->erase(i);
            break;
        }
    } 
}

void EpCombomodel::update_ep_model(const std::tuple<Glib::ustring, double, double, double, double,
                                    double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                    Glib::ustring>& epdata) const
{

    // TODO create a std::map of index to name in the combo model to avoid searching like this, this is temporary.

    Gtk::TreeModel::iterator iter;

    for (iter = m_eptreemodel->children().begin(); iter != m_eptreemodel->children().end(); ++iter)
        if (std::get<0>(epdata) == iter->get_value(m_epcols.m_epmodel)) break;

    if(iter)
    {
        const auto row = *iter;
        row[m_epcols.m_epmodel] = std::get<0>(epdata);
        row[m_epcols.m_epfov] = std::get<1>(epdata);
        row[m_epcols.m_epflen] = std::get<2>(epdata);
        row[m_epcols.m_epfstop] = std::get<3>(epdata);
        row[m_epcols.m_eprelief] = std::get<4>(epdata);
        row[m_epcols.m_eptrans] = std::get<5>(epdata);
        row[m_epcols.m_epbarrel] = std::get<6>(epdata);
        row[m_epcols.m_eptype] = std::get<7>(epdata);
        row[m_epcols.m_epgroups] = std::get<8>(epdata);
        row[m_epcols.m_epelements] = std::get<9>(epdata);
        row[m_epcols.m_epweight] = std::get<10>(epdata);
        row[m_epcols.m_epcoating] = std::get<11>(epdata);
        row[m_epcols.m_epmaterial] = std::get<12>(epdata);
    }
}

void EpCombomodel::setup_ep_combo_model(Gtk::ComboBox &epcombo)
{
    m_epcombo = &epcombo;
    m_epcombo->set_model(m_eptreemodel);
    m_epcombo->set_row_separator_func(sigc::mem_fun(*this, &EpCombomodel::on_separator));

    m_epcombo->pack_start(m_epcols.m_epmodel);
    m_epcombo->set_active(0);

    auto renderer = static_cast<Gtk::CellRendererText*>(m_epcombo->get_first_cell());
    renderer->property_ellipsize() = Pango::ELLIPSIZE_END;
    renderer->set_fixed_size(300, -1);
}

void EpCombomodel::set_ep_completion_model(Gtk::SearchEntry &epsearch)
{
    m_epsearch = &epsearch;
    epentrycompletion = Gtk::EntryCompletion::create();
    epentrycompletion->set_model(m_eptreemodel);
    epentrycompletion->set_text_column(m_epcols.m_epmodel);
    epentrycompletion->set_minimum_key_length(2);
    epentrycompletion->set_popup_completion(true);
    epentrycompletion->set_inline_completion(true);
    m_epsearch->set_completion(epentrycompletion);
    epentrycompletion->signal_match_selected().connect(sigc::mem_fun(*this, &EpCombomodel::on_ep_selected), false);
    epentrycompletion->set_match_func(sigc::mem_fun(*this, &EpCombomodel::on_ep_completion_match));
}

bool EpCombomodel::on_ep_selected(const Gtk::TreeModel::iterator &iter)
{
    if (iter)
    {
        const auto row = *iter;
        m_epcombo->set_active(row);
        m_epsearch->set_text(row[m_epcols.m_epmodel]);
        return true;
    }

    return false;
}

void EpCombomodel::set_case_sensitive(const bool case_sensitive)
{
    // to make auto completion case sensitive
    m_case_sensitive = case_sensitive;
}

bool EpCombomodel::on_ep_completion_match(const Glib::ustring &key, const Gtk::TreeModel::iterator &iter)
{
    if (iter)
    {
        size_t found;
        const auto row = *iter;

        if (true == m_case_sensitive)
            found = static_cast<Glib::ustring>(row[m_epcols.m_epmodel]).find(m_epsearch->get_text());
        else
            found = static_cast<Glib::ustring>(row[m_epcols.m_epmodel]).lowercase().find(key);

        if (found != Glib::ustring::npos)
            return true;
    }

    return false;
}
