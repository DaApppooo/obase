#include "app.hpp"
#include "layout.hpp"
#include "push_button.hpp"
#include "text.hpp"

int main()
{
  App app;
  app.init(
    (new Layout("window"))
    ->add(
      (new PushButton("btn"))
      ->set_text("Click me !")
      ->
    )
  );
  app.request("Hello world !")->fill_parent(10);

  app.run("obase test");
  return 0;
}

