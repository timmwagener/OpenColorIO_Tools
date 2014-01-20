




//Include
//-----------------------------------------------

//Qt
#include <QtWidgets/QApplication>
//Own
#include "ocio_view.h"






//main
//-----------------------------------------------

int main(int argc, char *argv[])
{
    //q_application
	QApplication* q_application;
	q_application = new QApplication(argc, argv);

	//node_view
	OCIO_view* ocio_view;
	ocio_view = new OCIO_view();
	ocio_view->setObjectName("OCIO_view");
	ocio_view->show();

	//event loop
    return q_application->exec();
}


