#include "oculardrawingarea.hpp"
#include "gtkmmcustomutils.hpp"
#include <iostream>

template <typename T>
constexpr T pi() { return std::acos(-1); }

Ocular::Ocular() 
{
    set_size_request(64, 64); // give it something to being with
    set_margin(Uidefs::BORDER_WIDTH);
    set_draw_func(sigc::mem_fun(*this, &Ocular::on_draw));
}

void Ocular::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int, int)
{
  Gtk::Allocation allocation = get_allocation();
  const int w = allocation.get_width();
  const int h = allocation.get_height();

  auto m_image = Gdk::Pixbuf::create_from_file(m_imagefile);

  if (!m_image)
    return;

  m_image->saturate_and_pixelate(m_image, 0.01, false);

  double fovscale = m_efov / 120.0;
  double scale = fovscale * m_imagesize  * h / (m_tfov * m_image->get_height());

  // approx for visual effect
  double bgcolour = 0.25 * (27 - skyscopebrightness) / 27.0; // sky in scope arbitrarily scaled for monitor

  cr->scale(w, h);
  cr->set_line_width(0);
  cr->arc(0.5, 0.5, 0.5, 0.0, 2.0 * pi<double>());
  cr->set_source_rgba(bgcolour, bgcolour, bgcolour, 1.00);
  cr->fill_preserve();
  cr->clip_preserve();

  cr->scale(scale / w, scale / h);
  Gdk::Cairo::set_source_pixbuf(cr, m_image,  (-m_image->get_width()  + w  / scale) / 2,
                                              (-m_image->get_height() + h  / scale) / 2);
 // cr->paint_with_alpha(pow(exitpupil / 7.0, 0.88)); // just for visual effect.
  double tmp = 0.5 * obscontrast; // uses treshold value for visibility (scaled for effect by 0.5).
  if (tmp < 0) tmp = 0;
  cr->paint_with_alpha(tmp);
  cr->scale(w / scale, h / scale);
  cr->begin_new_path(); // draw the fov ring
  cr->set_line_width(2.0 / w);
  cr->arc(0.5, 0.5, 0.5 * fovscale, 0.0, 2.0 * pi<double>());
  cr->set_source_rgba(0.082, 0.325, 0.619, 0.5);
  // cr->set_source_rgba(0.619, 0.082, 0.125, 0.40);
  cr->stroke();
}
