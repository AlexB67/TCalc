#pragma once
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <glibmm/ustring.h>
#include <glibmm/datetime.h>
#include <time.h>
#include <iostream>

namespace LogView
{
  enum {tINFO, tWARN, tERROR};
  class LoggerView : public Gtk::TextView
  {
  public:
    explicit LoggerView();
    LoggerView(const LoggerView &) = delete;
    LoggerView(LoggerView &&) = delete;
    LoggerView &operator=(const LoggerView &other) = delete;
    LoggerView &operator=(LoggerView &&) = delete;
    virtual ~LoggerView() {}
    
    Gtk::Frame &create_logger();
    void setlogtext(const bool flag, const int level, const Glib::ustring &logtext);
    void clear_log() { m_logtext->set_text(""); }
    void set_log_colour(const bool usecolour);
    void set_log_timestamp(const bool usetimestamp);
    void set_use_monospace(const bool usemonospace);
    
  private:
      Gtk::Label                              m_loggerframelabel;
      Gtk::Frame                              m_loggerframe;
      Gtk::Frame                              m_loggerinnerframe; //Add a borderline around TextView or else it looks ugly .. IMO.
      Gtk::ScrolledWindow                     m_scrollwin;                                         
      Glib::RefPtr<Gtk::TextBuffer>           m_logtext;
      Glib::RefPtr<Gtk::TextBuffer::Tag> tag;
      Glib::RefPtr<Gtk::TextBuffer::TagTable> tagtable;
      bool m_usetimestamp = false;
      Glib::ustring get_time_now() const;
      void create_text_buffer();
  };
}
