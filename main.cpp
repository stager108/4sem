#include "mainwindow.h"
#include <QApplication>
#include "RayTracer.h"

int main(int argc, char *argv[])
{
	Scene myscn(Colour(MAX_RGB, MAX_RGB, MAX_RGB));
	Camera camera(Point(0,0,0), Point(0,0,0));
    Screen scr(640, 480);
	RayTracer RT(scr, myscn, camera, Point(1,0,0), 20 );
	QApplication a(argc, argv);
    MainWindow w(scr);
	int weight = 640;
	int height = 480;
	std::vector<Colour> ans = RT.render(weight, height);
    for (int i = 0; i<weight; ++i) {
		for(int j = 0; j < height; j++) {
			Colour np = ans[i*weight + j];
			scr.putPixel({i, j}, {np.r, np.g, np.b});
		}
    }
    w.show();

    return a.exec();
}
