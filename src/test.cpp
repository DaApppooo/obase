#include "app.hpp"
#include "layout.hpp"
#include "text.hpp"

int main()
{
  App app;
  app.init(
    (new Layout("window"))
    ->add(
      (new Text("Hello world !"))
      ->anchor(CENTER)
      ->fill_parent()
    )
  );

  app.run("obase test");
  return 0;
}

