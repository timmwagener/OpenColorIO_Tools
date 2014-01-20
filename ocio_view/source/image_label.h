



#ifndef IMAGE_LABEL_H
#define IMAGE_LABEL_H

//Description
//-----------------------------------------------
/*
Declaration of Image_label
*/



//Constants
//-----------------------------------------------





//Include
//-----------------------------------------------

//STL
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
//Qt
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QPoint>
//Boost
//Own
//OCIO



//Image_label
//-----------------------------------------------

class Image_label : public QLabel
{
	
	Q_OBJECT


public: //Constructor / Destructor
	Image_label(QWidget* parent = 0, Qt::WindowFlags flags = 0, int logging = 0);
    ~Image_label();

	
public: //Methods
	void setup_ui();
	
	
signals: //Signals
	void inspect_begin(int, int);
	void inspect_move(int, int);
	void inspect_end();
	

private slots: //Slots


public: //Event Handler
	bool event(QEvent*);


protected: //Variables


private: //Variables
	int logging;
	
};

#endif