#include "widget.hpp"
#include "app.hpp"
#include "layout.hpp"
#include "text.hpp"

int main()
{
  App app;
  app.init(
    (new Layout("window"))
    ->orient(HORIZONTAL)
    ->add(
      new Text("Hello world !")
    )
  );

  app.run("obase test");
  return 0;
}

