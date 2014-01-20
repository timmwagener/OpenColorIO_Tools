




//Description
//-----------------------------------------------
/*
Implementation of Image_label
*/





//Include
//-----------------------------------------------

#include "image_label.h"







//Constructor / Destructor
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Image_label::Image_label(QWidget* parent , Qt::WindowFlags flags, int logging):
    QLabel(parent, flags),
	logging(logging)
{
    //setup_ui
	setup_ui();
};

//Destructor
Image_label::~Image_label()
{

};






//Public Methods
//-----------------------------------------------
//-----------------------------------------------

//setup_ui
void Image_label::setup_ui()
{
	//setScaledContents
	setScaledContents(true);
	//hover
	setAttribute(Qt::WA_Hover);
};


//Signals
//-----------------------------------------------
//-----------------------------------------------



//Slots
//-----------------------------------------------
//-----------------------------------------------



//Event Handler
//-----------------------------------------------
//-----------------------------------------------

//event
bool Image_label::event(QEvent* event)
{
	//HoverEnter
	if(event->type() == QEvent::HoverEnter)
	{
		//mouse_event
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
		
		//pos_x & y
		int pos_x = mouse_event->pos().x();
		int pos_y = mouse_event->pos().y();

		//emit inspect_begin
		emit inspect_begin(pos_x, pos_y);
	}
	//HoverMove
	else if(event->type() == QEvent::HoverMove)
	{
		//mouse_event
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
		
		//pos_x & y
		int pos_x = mouse_event->pos().x();
		int pos_y = mouse_event->pos().y();

		//emit inspect
		emit inspect_move(pos_x, pos_y);

	}
	//HoverLeave
	else if(event->type() == QEvent::HoverLeave)
	{
		//emit inspect_end
		emit inspect_end();
	}
	
	return QLabel::event(event);
};