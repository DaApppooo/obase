#include "timeline.hpp"
#include <qt6/QtWidgets/QApplication>
#include "common.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  palette = app.palette();
  Timeline tl;
  tl.show();
  return app.exec();
}



