// testOpencv.cpp: определяет точку входа для приложения.
//

#include "gui.h";


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    ClasterWindow main;
    //main.resize(1400, 800);
    main.show();
    
    return app.exec();


}
