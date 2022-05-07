#include "cairomm/context.h"
#include "cairomm/enums.h"
#include "cairomm/refptr.h"
#include "cairomm/surface.h"
#include "gdkmm/general.h"
#include "glibmm/refptr.h"
#include "gtkmm/button.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/image.h"
#include "gdkmm/pixbuf.h"

#include "sigc++/functors/ptr_fun.h"
#include <gtkmm.h>
#include <iostream>

//
// Default pixbuf dimensions
//
int picture_width = 600;
int picture_height = 300;

bool draw1_draw(const Cairo::RefPtr<Cairo::Context> cr) {
  int width = 100;
  int height = 100;
  cr->set_source_rgb(1, 1, 0);
  cr->rectangle(width * .25, height * .25, width * .5, height * .5);
  cr->fill();
  return false;
}

bool draw1_button_press(GdkEventButton *event) {
  std::cout << "button press on draw1" << std::endl;
  return true;
}

bool ev1_button_press(GdkEventButton *event) {
  std::cout << "button press on img1: " << event->x << " " << event->y << std::endl;
  return true;
}

void draw_on_image(const Cairo::RefPtr<Cairo::Context> cr,
                   Glib::RefPtr<Gdk::Pixbuf> pixbuf_ref) {
  int width = pixbuf_ref->get_width();
  int height = pixbuf_ref->get_height();
  /* Draw the pixbuf */

  /* Draw the pixbuf */
  // TODO need this? because surface was created directly from pixbuf?
  //cr->set_source(pixbuf_ref);
  cr->paint();
  /* Draw a red rectangle */
  cr->set_source_rgb(1, 1, 0);
  cr->rectangle(width * .25, height * .25, width * .5, height * .5);
  cr->fill();
  std::cout << "Button1 clicked: pixbuf shown" << std::endl;
}

void on_draw1_draw(Cairo::RefPtr<Cairo::Context> cr) {
  std::cout << "on_draw1_draw: entering" << std::endl;

  // coordinates for the center of the window
  int xc, yc;
  xc = picture_width / 2;
  yc = picture_height / 2;

  cr->set_line_width(5.0);

  // // draw green lines out from the center of the window
  cr->set_source_rgb(0.0, 1.0, 0.0);
  cr->move_to(0, 0);
  cr->line_to(xc, yc);
  cr->line_to(0, picture_height);
  cr->move_to(xc, yc);
  cr->line_to(picture_width, yc);
  cr->stroke();
}

int main(int argc, char *argv[]) {
  Gtk::Main gtkmain(argc, argv);

  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("oscilos.glade");

  Gtk::DrawingArea *draw1;
  builder->get_widget("draw1", draw1);
  draw1->set_size_request(600, 300);
  draw1->signal_button_press_event().connect(sigc::ptr_fun(&draw1_button_press));
  draw1->signal_draw().connect([](Cairo::RefPtr<Cairo::Context> cr) -> bool {draw1_draw(cr); return false;
  });

  Gtk::Image *img1;
  builder->get_widget("img_1", img1);

  Gtk::EventBox *ev1;
  builder->get_widget("event1", ev1);
  ev1->signal_button_press_event().connect(sigc::ptr_fun(&ev1_button_press));

  Gdk::Colorspace format_pix = Gdk::Colorspace::COLORSPACE_RGB;
  Cairo::Format format = Cairo::FORMAT_RGB24;
  auto surface = Cairo::ImageSurface::create(format, picture_width, picture_height);
  auto cr = Cairo::Context::create(surface);

  cr->set_source(surface, 0, 0);
  cr->paint();

  /* Draw a magenta rectangle */
  cr->set_source_rgb(1, 0, 1);
  cr->rectangle(picture_width * .25, picture_height * .25, picture_width * .25, picture_height * .5);
  cr->fill();

  auto pixbuf_ref = Gdk::Pixbuf::create_from_data(surface->get_data(), Gdk::COLORSPACE_RGB, true, 8, surface->get_width(), surface->get_height(), surface->get_stride());

  img1->set(pixbuf_ref);

  // coordinates for the center of the window
  int xc, yc;
  xc = picture_width / 2;
  yc = picture_height / 2;

  cr->set_line_width(15.0);

  // // draw lines out from the center of the window
  cr->set_source_rgb(0.8, 0.4, 0.0);
  cr->move_to(0, 0);
  cr->line_to(xc, yc);
  cr->line_to(0, picture_height);
  cr->move_to(xc, yc);
  cr->line_to(picture_width, yc);
  cr->stroke();

  Gtk::Button *btn1;
  builder->get_widget("button1", btn1);
  btn1->signal_clicked()
    .connect([cr, img1, pixbuf_ref]() {
      on_draw1_draw(cr);
      // refresh GtkImage
      img1->set(pixbuf_ref);
    });

  Gtk::Window *win;
  builder->get_widget("win_main", win);
  win->set_title("Oscilos");
  Gtk::Main::run(*win);

  return EXIT_SUCCESS;
}

