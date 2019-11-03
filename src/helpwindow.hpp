#pragma once
#include <webkit2/webkit2.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>

class WebView : public Gtk::Widget
{
public:

    WebView () : Gtk::Widget(webkit_web_view_new()){}
    virtual ~WebView () {};

    operator WebKitWebView * () { return WEBKIT_WEB_VIEW(gobj());}

    void load_uri (const gchar *uri)
    {  
        webkit_web_view_load_uri(*this, uri);
    }
};

class HelpWindow : public Gtk::ApplicationWindow
{
    public:
        HelpWindow(const Glib::RefPtr<Gtk::Application> app);
        virtual ~HelpWindow(){ /*delete help_page; */}
        Gtk::Entry m_entry;
        Gtk::Grid m_grid;
        Gtk::Frame m_frame;
        std::unique_ptr<WebView> help_page;
    
    private:
        bool on_key_press_event(GdkEventKey* event) override;

};