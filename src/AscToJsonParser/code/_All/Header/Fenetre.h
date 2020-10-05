#ifndef FENETRE_H
#define FENETRE_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/alignment.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include "Dialogue.h"


class Fenetre : public Gtk::Window
{

public: Fenetre();
  virtual ~Fenetre();

protected:
  //Signal handlers:
  void on_button_clicked();
  void afficherDialogue1();
  void afficherDialogue2();
  void afficherDialogue3();
  void Algo3();

  //Member widgets:
  Gtk::Button m_button;
  Gtk::VBox boiteV;
  Gtk::HBox boiteH;
  
  Gtk::Button bouton1;
  Gtk::Button bouton2;
  Gtk::Button bouton3;
  Gtk::Button Valider;
  
  Gtk::Label ForceVent;
  Gtk::Label DirectionVent;
  Gtk::Label Temps_algo;
  Gtk::Label test;
  
  
};



#endif // GTKMM_EXAMPLE_HELLOWORLD_H
