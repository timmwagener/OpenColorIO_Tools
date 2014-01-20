



#ifndef INSPECT_VIEW_H
#define INSPECT_VIEW_H

//Description
//-----------------------------------------------
/*
Declaration of Inspect_view
*/



//Constants
//-----------------------------------------------
#define COLORBAR_SCALEFACTOR 0.1960784




//Include
//-----------------------------------------------

//STL
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
//Qt
#include <QtWidgets/QWidget>
#include <QtGui/QColor>
#include <QtCore/QString>
//Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
//Own
//OCIO
//From .ui
#include "ui_inspect_view.h"




//Inspect_view
//-----------------------------------------------

class Inspect_view : public QWidget
{
	Q_OBJECT


public: //Constructor / Destructor
	explicit Inspect_view(QWidget* parent = 0);
    ~Inspect_view();

	
public: //Methods
	void set_initial_pointer_addresses();
	void setup_ui();
	void connect_ui();
	void set_color(QColor&, std::string);
	void set_color_bar(QColor&, QWidget*&, std::string);
	void set_color_label(QColor&, QLabel*&, std::string);
	void set_color_label_top(QColor&, QLabel*&);

signals: //Signals
	

private slots: //Slots
	

public: //Event Handler


protected: //Variables


private: //Variables
	//ui
	Ui::Inspect_view* ui;
	bool dev;
	int logging;
};

#endif