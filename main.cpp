#include "cairomm/context.h"
#include "cairomm/enums.h"
#include "cairomm/refptr.h"
#include "cairomm/surface.h"
#include "gdkmm/general.h"
#include "glibmm/refptr.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/checkbutton.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/hvbox.h"
#include "gtkmm/image.h"
#include "gdkmm/pixbuf.h"

#include "gtkmm/label.h"
#include "gtkmm/listbox.h"
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "sigc++/functors/ptr_fun.h"
#include <gtkmm.h>
#include <iostream>

//
// Default pixbuf dimensions
//
int picture_width = 600;
int picture_height = 300;

bool draw1_draw(const Cairo::RefPtr<Cairo::Context> & cr, const Glib::RefPtr<Gdk::Pixbuf> &pixbuf_ref) {
  int width = pixbuf_ref->get_width();
  int height = pixbuf_ref->get_height();
  Gdk::Cairo::set_source_pixbuf(cr, pixbuf_ref, 0, 0);
  cr->paint();
  std::cout << "draw1_draw: w=" << width << " h=" << height << std::endl;
  //  cr->set_source_rgb(1, 1, 0);
  //cr->rectangle(width * .25, height * .25, width * .25, height * .25);
  //cr->fill();
  return false;
}

bool draw1_button_press(GdkEventButton *event) {
  std::cout << "button press on draw1: " << event->x << " " << event->y
            << std::endl;
  return true;
}

void on_draw1_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
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

class MyListRow : public Gtk::ListBoxRow {
public:
  MyListRow(const std::string text) : label{text}, box {false, 1} {
    box.pack_start(label);
    box.pack_start(check);
    set_halign(Gtk::Align::ALIGN_START);
    label.set_size_request(150);
    check.set_label("active");
    check.activate();
    check.set_size_request(50);
    show_all_children();
    label.show();
    check.show();
    box.show();
  }

protected:
  Gtk::HBox box;
  Gtk::Label label;
  Gtk::CheckButton check;
};


int main(int argc, char *argv[]) {
  Gtk::Main gtkmain(argc, argv);

  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("oscilos.glade");


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

  Gtk::DrawingArea *draw1;
  builder->get_widget("draw1", draw1);
  draw1->set_size_request(picture_width, picture_height);
  draw1->signal_button_press_event()
    .connect(sigc::ptr_fun(&draw1_button_press));
  draw1->signal_draw()
    .connect(
      [pixbuf_ref](Cairo::RefPtr<Cairo::Context> cr) -> bool {
        draw1_draw(cr, pixbuf_ref);
        return false;
      });

  Gtk::Button *btn1;
  builder->get_widget("button1", btn1);
  btn1->signal_clicked()
    .connect([cr]() {
      on_draw1_draw(cr);
    });

  Gtk::ListBox *list1;
  builder->get_widget("list1", list1);

  // Gtk::Box *box1;
  // builder->get_widget("box1", box1);

  for (int i = 0; i < 40; i++) {
    auto hbox1 = Gtk::manage(new Gtk::HBox);

    auto label1 = Gtk::manage(new Gtk::Label {"teste 1 criativo"});
    label1->show();
    hbox1->pack_start(*label1);

    auto chk1 = Gtk::manage(new Gtk::CheckButton {""});
    chk1->set_active(true);
    chk1->show();
    hbox1->pack_start(*chk1);
    hbox1->show();
    list1->append(*hbox1);
    // box1->pack_start(*hbox1);
  }

    Gtk::Window *win;
    builder->get_widget("win_main", win);
    win->set_title("Oscilos");
    Gtk::Main::run(*win);

    return EXIT_SUCCESS;
}

