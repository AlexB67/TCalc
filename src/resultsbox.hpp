#pragma once
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/widget.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/tooltip.h>
#include <glibmm/i18n.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/combobox.h>
#include <gtkmm/menu.h>
#include "loggerbox.hpp"
#include "eyepiecebox.hpp"
#include "telescopebox.hpp"
#include "gtkmmcustomutils.hpp"

namespace ResultsBox
{
    struct ModelCols : public Gtk::TreeModel::ColumnRecord
    {
        Gtk::TreeModelColumn<Glib::ustring> m_results_property;
        Gtk::TreeModelColumn<Glib::ustring> m_results_set1;
        Gtk::TreeModelColumn<Glib::ustring> m_results_set2;

        ModelCols()
        {
            add(m_results_property);
            add(m_results_set1);
            add(m_results_set2);
        }
    };

    class Resultsbox
    {
    public:
        Resultsbox(){}
        Resultsbox(const Resultsbox &) = delete;
        Resultsbox(Resultsbox &&) = delete;
        Resultsbox &operator=(const Resultsbox &other) = delete;
        Resultsbox &operator=(Resultsbox &&) = delete;

        Gtk::ButtonBox      m_buttonbox;
        Gtk::Button         m_buttonleft;
        Gtk::Button         m_buttonright;
        
        Gtk::Frame &create_results_grid();
        void append_row(const Glib::ustring &propertyname, const double value, 
                        const int precision,  const Glib::ustring &postfix, const int resultsset) const;

        void append_row(const Glib::ustring &propertyname, const double value,  const int precision,  
                        const Glib::ustring &prefix, const Glib::ustring &postfix, const int resultsset) const;
        
        void append_row(const Glib::ustring &propertyname, const Glib::ustring &text, const int resultsset) const;
        void clear(bool reset = false) const;
        void get_ep_data(const std::shared_ptr<EpBox::Eyepiecebox>& epbox, const int resultsset) const;
        void get_scope_data(const std::shared_ptr<ScopeBox::Telescopebox>& scopebox, const int resultsset) const;

    private:
        Gtk::ScrolledWindow m_scrollwin;
        Gtk::Grid           m_resultsgrid;
        Gtk::Grid           m_resultsviewgrid;
        Gtk::Frame          m_resultsframe;
        Gtk::Frame          m_resultsviewframe;
        Gtk::TreeView       m_resultsview;
        Gtk::TreeView       m_epview;
        Gtk::TreeView       m_scopeview;
        ModelCols           m_resultCols;
        ModelCols           m_epCols;
        ModelCols           m_scopeCols;
        Glib::RefPtr<Gtk::ListStore>    m_resultsModel;
        Glib::RefPtr<Gtk::ListStore>    m_epModel;
        Glib::RefPtr<Gtk::ListStore>    m_scopeModel;
        Gtk::Label  m_resultsframelabel;
        Gtk::Label  m_selectset1{_("Set 1"), Gtk::ALIGN_END};
        Gtk::Label  m_selectset2{_("Set 2"), Gtk::ALIGN_START};
        Gtk::CellRendererText           m_renderertext;
        Gtk::CellRendererText           m_renderertext2; // useful for wrapping
        std::vector<Glib::ustring>      m_proplistnames;
        std::vector<Glib::ustring>      m_eplistnames;
        std::vector<Glib::ustring>      m_scopelistnames;
        void init_property_names();
        void create_model_view( const Glib::ustring& header, Gtk::TreeView& view, 
                                Glib::RefPtr<Gtk::ListStore>& model, const ModelCols& cols, const bool set_sort_column);

        bool set_results_row_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
        bool set_scope_row_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
        bool set_ep_row_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
        size_t get_index(const Glib::ustring &propertyname, const Glib::RefPtr<Gtk::ListStore>& liststore) const;
    };

} // namespace ResultsBox