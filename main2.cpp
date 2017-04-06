#include "mainwindow.h"
#include <QApplication>

#include "RayTracer.h"

int main(int argc, char *argv[]) {
    Scene myscn(Colour(MAX_RGB, MAX_RGB, MAX_RGB));
    Camera camera(Point(0, 0.5, 0.5), Point(1, 0, 0), Point(1,1,0), Point(1,1,1), Point(1,0, 1));
    Screen scr(480, 480);
    RayTracer RT(scr, myscn, camera);
    QApplication a(argc, argv);
    MainWindow w(scr);
    /*int weight = 640;
    int height = 480;
    std::vector<Colour> ans = RT.render(weight, height);
    for (int i = 0; i<weight; ++i) {
        for(int j = 0; j < height; j++) {
            Colour np = ans[i*weight + j];
#ifdef QTRUN
            scr.putPixel({i, j}, {np.r, np.g, np.b});
#endif
        }
    }*/

    RT.render();

    w.show();
    return a.exec();

}
