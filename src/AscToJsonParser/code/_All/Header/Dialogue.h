#ifndef DEF_DIALOGUE
#define DEF_DIALOGUE

#include <string>

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>

class Dialogue : public Gtk::Dialog {
    public :
        Dialogue(std::string titre, Gtk::Window* parent, std::string message);
        std::string get_texte();
        void set_texte(std::string texte);
        
    private :
        Gtk::Box* boiteV;
        Gtk::Label etiquette;
        Gtk::Entry zoneTexte;
};

#endif

