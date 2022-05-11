//$Id: dndwindow.cc 836 2007-05-09 03:02:38Z jjongsma $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "dndwindow.hpp"
#include <iostream>

DnDWindow::DnDWindow()
: m_Button_Drag("Drag Here\n"),
  m_Label_Drop("Drop here\n"),
  other_button("Label to drag\n")
{
  set_title("DnD example");

  add(m_HBox);

  //Targets:
  std::vector<Gtk::TargetEntry> listTargets;
  listTargets.push_back( Gtk::TargetEntry("STRING") );
  listTargets.push_back( Gtk::TargetEntry("text/plain") );

  //
  // Drag site:
  //

  // Make m_Button_Drag a DnD drag source:
  m_Button_Drag.drag_source_set(listTargets);

  // Connect signals:
  m_Button_Drag.signal_drag_data_get()
    .connect(sigc::mem_fun(*this, &DnDWindow::on_button_drag_data_get));

  m_HBox.pack_start(m_Button_Drag);

  // Make other_button a DnD drag source:
  other_button.drag_source_set(listTargets);

  // Connect signals:
  other_button.signal_drag_data_get()
    .connect(sigc::mem_fun(*this, &DnDWindow::on_other_button_drag_data_get));

  m_HBox.pack_start(other_button);

  // Drop site:

  // Make m_Label_Drop a DnD drop destination:
  m_Label_Drop.drag_dest_set(listTargets);

  // Connect signals:
  m_Label_Drop.signal_drag_data_received().connect(sigc::mem_fun(*this,
              &DnDWindow::on_label_drop_drag_data_received) );

  m_HBox.pack_start(m_Label_Drop);

  show_all();
}

DnDWindow::~DnDWindow()
{
}

void DnDWindow::on_button_drag_data_get(
        const Glib::RefPtr<Gdk::DragContext>& dc,
        Gtk::SelectionData& selection_data, guint, guint)
{
  std::stringstream s;
  int i { 1 };

  s << "Data " << i;
  std::string ss { s.str() };

  selection_data.set(selection_data.get_target(), 8 /* 8 bits format */,
          (const guchar*) ss.c_str(),
          ss.length() /* the length of I'm Data! in bytes */);
}

void DnDWindow::on_other_button_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &,
                                        Gtk::SelectionData &selection_data,
                                        guint, guint) {
  selection_data.set(selection_data.get_target(), 8 /* 8 bits format */,
                     (const guchar *) "I'm Data other button!",
                     22 /* the length of I'm Data! in bytes */);
}

void DnDWindow::on_label_drop_drag_data_received(
    const Glib::RefPtr<Gdk::DragContext> &context, int, int,
    const Gtk::SelectionData &selection_data, guint, guint time) {
  const int length = selection_data.get_length();
  if((length >= 0) && (selection_data.get_format() == 8))
  {
    std::cout << "Received \"" << selection_data.get_data_as_string()
        << "\" in label " << std::endl;
  }

  context->drag_finish(false, false, time);
}
