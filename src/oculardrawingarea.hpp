#pragma once
#include <gdkmm/pixbuf.h>
#include <gdkmm/general.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

class Ocular : public Gtk::DrawingArea 
{
    public: 
        Ocular();
        std::string m_imagefile;
        double m_efov;
        double m_tfov; // in arc min
        double m_imagesize;
        double exitpupil;

    protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};