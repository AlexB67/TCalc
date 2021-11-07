#include "eyepiececombomodel.hpp"
#include <iostream>
#include <glibmm/i18n.h>

using namespace EpCombo;

void EpCombomodel::create_ep_model()
{
    m_eptreemodel = Gtk::TreeStore::create(m_epcols);
    m_eplistmodel = Gtk::ListStore::create(m_epcompletioncols); // use a list model for auto completion.
}

void EpCombomodel::swap_ep_rows(const Glib::ustring& epname) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator previousiter;
    Gtk::TreeModel::iterator parentiter;


    for(parentiter = m_eptreemodel->children().begin(); parentiter != m_eptreemodel->children().end(); ++parentiter)
        if ((*parentiter)[m_epcols.m_epbrand] == _("User")) break;

    bool found = false;

    for (iter = parentiter->children().begin(); iter != parentiter->children().end(); ++iter)
    {
        if (epname == iter->get_value(m_epcols.m_epmodel))
        {
            previousiter = iter;
            iter--;
            found = true;
        }

        if(found) break;
    }

    Gtk::TreeRow prow = *previousiter;
    Gtk::TreeRow row = *iter;

    std::tuple<double, double, double, double, double, double, Glib::ustring, int, int, double, 
    Glib::ustring, Glib::ustring> epdata = 
    {
        prow[m_epcols.m_epfov], prow[m_epcols.m_epflen], prow[m_epcols.m_epfstop], prow[m_epcols.m_eprelief], 
        prow[m_epcols.m_eptrans], prow[m_epcols.m_epbarrel], prow[m_epcols.m_eptype], prow[m_epcols.m_epgroups],
        prow[m_epcols.m_epelements], prow[m_epcols.m_epweight], prow[m_epcols.m_epcoating], 
        prow[m_epcols.m_epmaterial]
    }; // note, we omit the model name and brand

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
    }
}

void EpCombomodel::append_ep_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                                        double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                        Glib::ustring>& epdata, bool ischild) // used for loading
{
    if (true == ischild)
    {
        auto childrow = *(m_eptreemodel->append(parent_row.children()));
       // brand is omitted for children row, otherwise they appear in the combo popup 2nd column .. unwanted
        childrow[m_epcols.m_epmodel] = std::get<1>(epdata);
        childrow[m_epcols.m_epfov] = std::get<2>(epdata);
        childrow[m_epcols.m_epflen] = std::get<3>(epdata);
        childrow[m_epcols.m_epfstop] = std::get<4>(epdata);
        childrow[m_epcols.m_eprelief] = std::get<5>(epdata);
        childrow[m_epcols.m_eptrans] = std::get<6>(epdata);
        childrow[m_epcols.m_epbarrel] = std::get<7>(epdata);
        childrow[m_epcols.m_eptype] = std::get<8>(epdata);
        childrow[m_epcols.m_epgroups] = std::get<9>(epdata);
        childrow[m_epcols.m_epelements] = std::get<10>(epdata);
        childrow[m_epcols.m_epweight] = std::get<11>(epdata);
        childrow[m_epcols.m_epcoating] = std::get<12>(epdata);
        childrow[m_epcols.m_epmaterial] = std::get<13>(epdata);

        // used by search completion
        auto listrow = *(m_eplistmodel->append());
        listrow[m_epcompletioncols.m_epbrand] = std::get<0>(epdata);
        listrow[m_epcompletioncols.m_epmodel] = std::get<1>(epdata);
    }
    else
    {
        parent_row = *(m_eptreemodel->append());
        // Parent holds the brand name and will be appended by child rows above.
        parent_row[m_epcols.m_epbrand] = std::get<0>(epdata);
        parent_row[m_epcols.m_epmodel] = ""; // stops text != NULL warnings
    }
}

void EpCombomodel::add_ep_to_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                                    double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                    Glib::ustring>& epdata) const // Used by equipment editor
{

    Gtk::TreeModel::iterator iter;
    bool found = false;
    
    if(m_eptreemodel->children().size() > 0)
    {
        for (iter = m_eptreemodel->children().begin(); iter !=m_eptreemodel->children().end(); ++iter)
        {
            if (iter->get_value(m_epcols.m_epbrand) == _("User")) 
            {
                found = true;
            }

            if(found) break;
        }
    }
    
    if (false == found) // No user eyepieces were aded yet so add the "User" category
    {
        Gtk::TreeRow row = *(m_eptreemodel->prepend());
        row[m_epcols.m_epbrand] = _("User");
        row[m_epcols.m_epmodel] = ""; // stops text != NULL warnings
    }

    if (false == found) iter = m_eptreemodel->children().begin();
    
    if (iter)
    {
        Gtk::TreeRow row = *(m_eptreemodel->append(iter->children()));
        row[m_epcols.m_epmodel] = std::get<1>(epdata);
        row[m_epcols.m_epfov] = std::get<2>(epdata);
        row[m_epcols.m_epflen] = std::get<3>(epdata);
        row[m_epcols.m_epfstop] = std::get<4>(epdata);
        row[m_epcols.m_eprelief] = std::get<5>(epdata);
        row[m_epcols.m_eptrans] = std::get<6>(epdata);
        row[m_epcols.m_epbarrel] = std::get<7>(epdata);
        row[m_epcols.m_eptype] = std::get<8>(epdata);
        row[m_epcols.m_epgroups] = std::get<9>(epdata);
        row[m_epcols.m_epelements] = std::get<10>(epdata);
        row[m_epcols.m_epweight] = std::get<11>(epdata);
        row[m_epcols.m_epcoating] = std::get<12>(epdata);
        row[m_epcols.m_epmaterial] = std::get<13>(epdata);
    }

    // Update completion list
    auto listrow = *(m_eplistmodel->append());
    listrow[m_epcompletioncols.m_epbrand] = _("User");
    listrow[m_epcompletioncols.m_epmodel] = std::get<1>(epdata);
}

void EpCombomodel::remove_ep_from_model(const Glib::ustring &epname) const
{

    Gtk::TreeModel::iterator iter;

    if (0 == m_eptreemodel->children().size()) return;

    for (iter = m_eptreemodel->children().begin(); iter !=m_eptreemodel->children().end(); ++iter)
        if (iter->get_value(m_epcols.m_epbrand) == _("User")) break;

    bool deleted = false;
    for (auto& iter2 : iter->children())
    {
        if (epname == iter2.get_value(m_epcols.m_epmodel))
        {
            if(iter2) m_eptreemodel->erase(iter2.get_iter());
            deleted = true;
        }

        if(deleted) break;
    }

    // If there are no more user eyepieces left delete the User category (parent)
    if (0 == iter->children().size()) m_eptreemodel->erase(iter);

     // remove from completion list
    deleted = false;
    for (auto &i : m_eplistmodel->children())
    {
        if (epname == i.get_value(m_epcompletioncols.m_epmodel))
        {
            m_eplistmodel->erase(i.get_iter());
            deleted = true;
        }

        if(deleted) break;
    }
}

void EpCombomodel::update_ep_model(const std::tuple<Glib::ustring, Glib::ustring, double, double, double, double,
                                    double, double, Glib::ustring, int, int, double, Glib::ustring, 
                                    Glib::ustring>& epdata, const Glib::ustring& oldname) const
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::iterator iter2;

    for (iter = m_eptreemodel->children().begin(); iter !=m_eptreemodel->children().end(); ++iter)
        if (iter->get_value(m_epcols.m_epbrand) == _("User")) break;

    for (iter2 = iter->children().begin(); iter2 != iter->children().end(); ++iter2)
        if (oldname == iter2->get_value(m_epcols.m_epmodel))
            break;

    if(iter2)
    {
        auto row = *iter2;
        if(row)
        {
            row[m_epcols.m_epmodel] = std::get<1>(epdata);
            row[m_epcols.m_epfov] = std::get<2>(epdata);
            row[m_epcols.m_epflen] = std::get<3>(epdata);
            row[m_epcols.m_epfstop] = std::get<4>(epdata);
            row[m_epcols.m_eprelief] = std::get<5>(epdata);
            row[m_epcols.m_eptrans] = std::get<6>(epdata);
            row[m_epcols.m_epbarrel] = std::get<7>(epdata);
            row[m_epcols.m_eptype] = std::get<8>(epdata);
            row[m_epcols.m_epgroups] = std::get<9>(epdata);
            row[m_epcols.m_epelements] = std::get<10>(epdata);
            row[m_epcols.m_epweight] = std::get<11>(epdata);
            row[m_epcols.m_epcoating] = std::get<12>(epdata);
            row[m_epcols.m_epmaterial] = std::get<13>(epdata);
        }
    }

    // Update completion model
    bool updated = false;

    for (auto &i : m_eplistmodel->children())
    {
        if (oldname == i.get_value(m_epcompletioncols.m_epmodel))
        {
            auto row = i;
            row[m_epcompletioncols.m_epmodel] =  std::get<1>(epdata);
            updated = true;
        }
        
        if(updated) break;
    }
}
