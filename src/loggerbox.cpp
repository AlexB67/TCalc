#include "loggerbox.hpp"
#include <glibmm/i18n.h>
#include <iostream>
#include "gtkmmcustomutils.hpp"
#include "appglobals.hpp"

LogView::LoggerView::LoggerView()
{
    m_loggerframe.set_label_widget(m_loggerframelabel);
    m_loggerframelabel.set_markup(_("<b>Information log</b>"));
    m_loggerframe.set_label_align( Gtk::ALIGN_CENTER,  Gtk::ALIGN_CENTER);
    set_editable(false);
    set_vexpand(true);
    set_hexpand(false);
    set_halign(Gtk::ALIGN_FILL);
    set_valign(Gtk::ALIGN_FILL);
    m_loggerframe.set_hexpand(false);
    m_loggerinnerframe.set_hexpand(false);
    m_loggerframe.set_vexpand(true);
    m_loggerinnerframe.set_vexpand(true);
    m_loggerinnerframe.set_border_width(Uidefs::BORDER_WIDTH_SMALL);
    set_border_width(Uidefs::BORDER_WIDTH_SMALL);
}

Gtk::Frame &LogView::LoggerView::create_logger()
{
    m_loggerframe.add(m_loggerinnerframe);
    m_loggerinnerframe.add(m_scrollwin);
    m_scrollwin.add(*this);

    create_text_buffer();

    AppGlobals::log_msg.connect(sigc::mem_fun(*this, &LogView::LoggerView::setlogtext));
    AppGlobals::get_keyfile_config(m_loggerframe);
    AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(m_loggerframe);});

    return m_loggerframe;
}

void LogView::LoggerView::setlogtext(const bool flag, const int level, const Glib::ustring& logtext)
{
  if(false == flag)
    return;

  if (true == m_usetimestamp)  
    m_logtext->insert_at_cursor(get_time_now());

  if ( LogView::tINFO == level )
    m_logtext->insert_at_cursor(logtext);
  else
    m_logtext->insert_with_tag(m_logtext->end(), logtext, tag);

  m_logtext->insert_at_cursor("\n");
  auto  pos = m_logtext->create_mark("end_mark", m_logtext->end(), false);
  scroll_to(pos, 0.0f);
}

Glib::ustring LogView::LoggerView::get_time_now() const
{
    auto timenow =  Glib::DateTime::create_now_local();	
    Glib::ustring ustringtime = timenow.format("%d/%m/%y,%H:%M:%S: ");
    return ustringtime;
}

void LogView::LoggerView::create_text_buffer()
{
    tag = Gtk::TextBuffer::Tag::create();
    tag->property_style_set().set_value(true);
    tag->property_style().set_value(Pango::STYLE_ITALIC);
    tagtable = Gtk::TextBuffer::TagTable::create();
    tagtable->add(tag);
    m_logtext = Gtk::TextBuffer::create(tagtable);
    set_buffer(m_logtext);
}

void LogView::LoggerView::set_log_colour(const bool usecolour)
{
   (true == usecolour) ? tag->property_foreground().set_value("red") : tag->property_foreground().reset_value();
}

void LogView::LoggerView::set_log_timestamp(const bool usetimestamp)
{
    m_usetimestamp = usetimestamp;
}

void LogView::LoggerView::set_use_monospace(const bool usemonospace)
{
  set_monospace(usemonospace);
}
